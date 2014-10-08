#include "hdrimage.h"
#include <QFile>
#include <memory>
#include <QStringList>
#include "../debug.h"
#include <cmath>

HdrImage::HdrImage() {

}

HdrImage::HdrImage(const QString &path, Format format) {
	load(path, format);
}

HdrImage::~HdrImage() {
	delete[] _data;
}

int HdrImage::sizeBytes() const {
	switch (_format) {
		case Format_RGB32F:
			return sizeof(Rgb32F)*_size.width()*_size.height();
		case Format_RGBE8:
		default:
			return sizeof(Rgba8)*_size.width()*_size.height();
	}
}


bool HdrImage::load(const QString &path, Format format) {
	_format = format;
	delete[] _data;
	_data = nullptr;

	// ----------------- open file -----------------

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "Can't open " + path + ": " + file.errorString() << __DEBUG_INFO__;
		return false;
	}

	if (file.size()>INT_MAX) {
		qWarning() << "File " + path + " is too big" << __DEBUG_INFO__;
		file.close();
		return false;
	}

	int fileSize = file.size();
	std::unique_ptr<unsigned char> fileData_(new(std::nothrow) unsigned char[fileSize]);
	unsigned char *fileData = fileData_.get();
	if (fileData==nullptr) {
		qWarning() << "Can't allocate memory for " + path << __DEBUG_INFO__;
		file.close();
		return false;
	}

	if (file.read(reinterpret_cast<char *>(fileData), fileSize)<1) {
		qWarning() << "Can't read " + path << __DEBUG_INFO__;
		file.close();
		return false;
	}
	file.close();


	// ---------------- read header ----------------

	int pos = 0;

	QString buf;
	pos = headerLine(fileData, fileSize, pos, buf);

	if (buf!="#?RADIANCE") {
		qWarning() << "Magic string in hdr file " + path + " is wrong" << __DEBUG_INFO__;
		return false;
	}

	int valid = 0;
	while (pos<fileSize) {
		pos = headerLine(fileData, fileSize, pos, buf);
		if (buf.length()<18) {
			QStringList list = buf.split(' ');
			if (list.count()==4)
				if (list[0]=="-Y" && list[2]=="+X") {
					_size.setHeight(list[1].toInt());
					_size.setWidth(list[3].toInt());
					valid++;
					break;
				}
		} else if (buf.length()<25)
			if (buf=="FORMAT=32-bit_rle_rgbe")
				valid++;
	}

	if (valid!=2) {
		qWarning() << "Format file " + path + " is not supported."
					  "Support formats: FORMAT=32-bit_rle_rgbe and "
					  "resolution string: -Y N +X M" << __DEBUG_INFO__;
		return false;
	}

	if (_size.width()<1 || _size.height()<1 || _size.width()>maxRes || _size.height()>maxRes) {
		qWarning() << "Hdr resolution for " + path + " is not supported. Max resolution " +
					  QString::number(maxRes) + "x" + QString::number(maxRes) << __DEBUG_INFO__;
		return false;
	}


	// ----------------- read data -----------------

	int pixSize;
	switch (format) {
		case Format_RGB32F:
			pixSize = sizeof(Rgb32F);
			break;
		case Format_RGBE8:
		default:
			pixSize = sizeof(Rgba8);
			break;
	}
	int rgbeSize = sizeof(Rgba8);
	int scanSize = _size.width()*rgbeSize;

	std::unique_ptr<unsigned char> scanline_(new(std::nothrow) unsigned char[scanSize]);
	std::unique_ptr<unsigned char> localData_(new(std::nothrow) unsigned char[_size.width()*_size.height()*pixSize]);
	unsigned char *scanline = scanline_.get();
	unsigned char *localData = localData_.get();
	if (localData==nullptr || scanline==nullptr) {
		qWarning() << "Can't allocate memory for uncompressed data for " + path << __DEBUG_INFO__;
		return false;
	}

	for (int y=0; y<_size.height(); y++) {
		if (pos+4 > fileSize) {
			qWarning() << "Unexpected end of file " + path + ", pos: " + QString::number(pos) << __DEBUG_INFO__;
			return false;
		}

		if (fileData[pos]!=2 || fileData[pos+1]!=2 || (fileData[pos+2]&0x80)) {
			int uncompressedSize = _size.width()*(_size.height()-y);
			if (fileSize-pos < uncompressedSize*rgbeSize) {
				qWarning() << "Error file data size: " + path + ", expected data size: " +
							  QString::number(uncompressedSize*rgbeSize) + "b, real data size: " +
							  QString::number(fileSize-pos) + "b" << __DEBUG_INFO__;
				return false;
			}

			switch (format) {
				case Format_RGB32F: {
					Rgba8 *rgbe = reinterpret_cast<Rgba8*>(&fileData[pos]);
					Rgb32F *rgb32f = reinterpret_cast<Rgb32F*>(&localData[y*_size.width()*pixSize]);
					for (int i=0; i<uncompressedSize; i++)
						rgbeToFloat(rgbe[i], rgb32f[i]);
					break;
				}
				case Format_RGBE8:
				default: {
					unsigned char *from = &fileData[pos];
					unsigned char *to = &localData[y*_size.width()*pixSize];
					uncompressedSize *= rgbeSize;
					for (int i=0; i<uncompressedSize; i++)
						to[i] = from[i];
					break;
				}
			}

			return true;
		}

		if ((static_cast<int>(fileData[pos+2])<<8 | fileData[pos+3]) != _size.width()) {
			qWarning() << "Wrong scanline width: " + path << __DEBUG_INFO__;
			return false;
		}

		pos += 4;
		for (int i=0; i<4; i++) {
			int scanPos = i;
			while (scanPos<scanSize) {
				if (pos>=fileSize) {
					qWarning() << "Bad scanline data: " + path << __DEBUG_INFO__;
					return false;
				}

				if (fileData[pos]>128) {
					int count = fileData[pos]-128;
					if ((count==0) || (count>scanSize-scanPos)) {
						qWarning() << "Bad scanline data (2): " + path + ", i=" + QString::number(i) +
									  ", scanPos=" + QString::number(scanPos) + ", scanSize=" +
									  QString::number(scanSize) + ", count=" + QString::number(count) << __DEBUG_INFO__;
						return false;
					}
					pos++;
					for (int j=0; j<count; j++, scanPos+=4)
						scanline[scanPos] = fileData[pos];
					pos++;
				} else {
					int count = fileData[pos];
					if ((count==0) || (count>scanSize-scanPos)) {
						qWarning() << "Bad scanline data (3): " + path + ", i=" + QString::number(i) +
									  ", scanPos=" + QString::number(scanPos) + ", scanSize=" +
									  QString::number(scanSize) + ", count=" + QString::number(count) << __DEBUG_INFO__;
						return false;
					}
					pos++;
					for (int j=0; j<count; j++, scanPos+=4)
						scanline[scanPos] = fileData[pos++];
				}
			}
		}

		switch (format) {
			case Format_RGB32F: {
				Rgba8 *rgbe = reinterpret_cast<Rgba8*>(scanline);
				Rgb32F *rgb32f = reinterpret_cast<Rgb32F*>(&localData[y*_size.width()*pixSize]);
				for (int i=0; i<_size.width(); i++)
					rgbeToFloat(rgbe[i], rgb32f[i]);
				break;
			}
			case Format_RGBE8:
			default: {
				unsigned char *to = &localData[y*_size.width()*pixSize];
				for (int i=0; i<scanSize; i++)
					to[i] = scanline[i];
				break;
			}
		}
	}

	_data = localData_.release();
	return true;
}

int HdrImage::headerLine(const unsigned char *fileData, int fileSize, int pos, QString &buf) const {
	buf.clear();

	for (; pos<fileSize; pos++)
		if (fileData[pos]!='\n')
			buf += fileData[pos];
		else {
			pos++;
			break;
		}

	return pos;
}

inline void HdrImage::rgbeToFloat(const Rgba8 &from, Rgb32F &to) const {
	if (from.a) {
		float f = ldexp(1.0, static_cast<int>(from.a)-128-8);
		to.r = from.r*f;
		to.g = from.g*f;
		to.b = from.b*f;
	} else
		to.r = to.g = to.b = 0;
}

QVector3D HdrImage::lightDir() const {
	if (!_data) {
		qWarning() << "Hdr not loaded" << __DEBUG_INFO__;
		return QVector3D(1, 0, 0);
	}

	int n = size().width()*size().height();
	int maxId = 0;
	float max = 0;

	switch (_format) {
		case Format_RGB32F: {
			Rgb32F *rgb32f = reinterpret_cast<Rgb32F*>(_data);
			for (int i=0; i<n; i++) {
				float sum = rgb32f[i].r+rgb32f[i].g+rgb32f[i].b;
				if (sum>max) {
					max = sum;
					maxId = i;
				}
			}
			break;
		}
		case Format_RGBE8:
		default: {
			Rgba8 *rgba8 = reinterpret_cast<Rgba8*>(_data);
			Rgb32F rgb32f;
			for (int i=0; i<n; i++) {
				rgbeToFloat(rgba8[i], rgb32f);
				float sum = rgb32f.r+rgb32f.g+rgb32f.b;
				if (sum>max) {
					max = sum;
					maxId = i;
				}
			}
			break;
		}
	}

	QVector3D light;
	int section = maxId/(n/6);
	int pix = maxId - section*n/6;
	float x = pix % size().width();
	float y = pix / size().width();
	x /= size().width();
	y /= size().height()/6;

	switch (section) {
		case 0: // -x
			light = {-1, x, 1-y};
			break;
		case 1: // +x
			light = {1, x, 1-y};
			break;
		case 2: // -y
			light = {x, 1, 1-y};
			break;
		case 3: // +y
			light = {x, -1, 1-y};
			break;
		case 4: // -z
			light = {x, 1-y, 1};
			break;
		case 5: // +z
			light = {x, 1-y, -1};
			break;
	}

	return light.normalized();
}

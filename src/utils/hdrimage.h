#ifndef HDRIMAGE_H
#define HDRIMAGE_H

#include <QString>
#include <QSize>
#include <QVector3D>
#include "color.h"

class HdrImage {
public:
	enum Format {Format_RGB32F, Format_RGBE8};

	HdrImage();
	HdrImage(const QString &path, Format format=Format_RGBE8);
	~HdrImage();

	HdrImage(HdrImage const &) = delete;
	HdrImage& operator=(HdrImage const &) = delete;

	int sizeBytes() const;
	bool load(const QString &path, Format format=Format_RGBE8);
	QVector3D lightDir() const;

	inline QSize size() const {
		if (_data==nullptr)
			return QSize(0, 0);
		return _size;
	}

	inline unsigned char *data() const {
		return _data;
	}

	inline HdrImage::Format format() const {
		return _format;
	}

private:
	QSize _size = QSize(0, 0);
	unsigned char *_data = nullptr;
	Format _format = Format_RGBE8;

	const int maxRes = 0x8000;

	int headerLine(const unsigned char *fileData, int fileSize, int pos, QString &buf) const;
	void rgbeToFloat(const Rgba8 &from, Rgb32F &to) const;

};

#endif // HDRIMAGE_H

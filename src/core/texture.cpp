#include "texture.h"
#include "debug.h"
#include <QGLWidget>

Texture::Texture() {

}

Texture::Texture(const QString &path, int format, int filter, int wrap): Texture() {
	createTexture(path, format, filter, wrap);
}

Texture::~Texture() {

}

int Texture::bind(int num) {
	glActiveTexture(GL_TEXTURE0+num);
	glBindTexture(target, _texId);
	return num;
}

bool Texture::createHdrCubemap(const QString &path,
							   HdrImage::Format hdrFormat, int filter, int wrap)
{
	HdrImage hdr;
	if (!hdr.load(path, hdrFormat)) {
		qWarning() << "Can't create HDR texture from file: " + path << __DEBUG_INFO__;
		return false;
	}

	return createHdrCubemap(hdr, filter, wrap);
}

bool Texture::createHdrCubemap(const HdrImage &hdr, int filter, int wrap) {
	_width = hdr.size().width();
	_height = hdr.size().height();
	int singleSize = ((hdr.sizeBytes()/_width)/6)*_width;
	target = GL_TEXTURE_CUBE_MAP;

	switch (hdr.format()) {
		case HdrImage::Format_RGB32F:
			internalFormat = GL_RGB16F_ARB;
			format = GL_RGB;
			type = GL_FLOAT;
			break;
		case HdrImage::Format_RGBE8:
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
		default:
			qWarning() << "Format unsupported: " << hdr.format() << __DEBUG_INFO__;
			return false;
	}

	if (_texId)
		glDeleteTextures(1, &_texId);

	glGenTextures(1, &_texId);
	glBindTexture(target, _texId);
	for (int i=0; i<6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, internalFormat,
					 _width, _height/6, 0, format, type, hdr.data()+i*singleSize);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, wrap);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
	if (filter >= GL_NEAREST_MIPMAP_NEAREST && filter <= GL_LINEAR_MIPMAP_LINEAR)
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	if (!_texId) {
		qWarning() << "Can't create cubemap" << __DEBUG_INFO__;
		return false;
	}

	return true;
}

void Texture::createTexture(int internalFormat, int width, int height, int format,
							int type, int filter, int wrap, const void *pix)
{
	target = GL_TEXTURE_2D;
	this->_width = width;
	this->_height = height;
	this->internalFormat = internalFormat;
	this->format = format;
	this->type = type;

	if (_texId)
		glDeleteTextures(1, &_texId);

	glGenTextures(1, &_texId);
	glBindTexture(target, _texId);
	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, pix);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	if (filter >= GL_NEAREST_MIPMAP_NEAREST && filter <= GL_LINEAR_MIPMAP_LINEAR)
		glGenerateMipmap(GL_TEXTURE_2D);
}

bool Texture::createTexture(const QString &path, int format, int filter, int wrap) {
	QImage img;
	if (!img.load(path)) {
		qWarning() << "wrong path: " + path << __DEBUG_INFO__;
		return false;
	}

	img = QGLWidget::convertToGLFormat(img);

	_width = img.width();
	_height = img.height();
	target = GL_TEXTURE_2D;
	this->format = format;
	type = GL_UNSIGNED_BYTE;
	unsigned char *pixels;
	bool delPixels = false;

	switch (format) {
		case GL_RGB:
			img = img.convertToFormat(QImage::Format_RGB888);
			internalFormat = GL_RGB8;
			pixels = img.bits();
			break;

		case GL_RGBA:
			img = img.convertToFormat(QImage::Format_ARGB32);
			internalFormat = GL_RGBA8;
			pixels = img.bits();
			break;

		case GL_RED: {
			internalFormat = GL_R8;
			pixels = new(std::nothrow) unsigned char[_width*_height];
			if (!pixels) {
				qWarning() << "Can't create texture: " + path << format << __DEBUG_INFO__;
				return false;
			}
			delPixels = true;
			unsigned char *bits = img.bits();
			int n = _width*_height;
			int bpp = img.byteCount()/n;
			for (int i=0; i<n; i++)
				pixels[i] = bits[i*bpp];
			break;
		}

		default:
			qWarning() << "Format unsupported: " + path << format << __DEBUG_INFO__;
			return false;
	}

	if (_texId)
		glDeleteTextures(1, &_texId);

	glGenTextures(1, &_texId);
	glBindTexture(target, _texId);
	glTexImage2D(target, 0, internalFormat, _width, _height, 0, format, type, pixels);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
	if (filter >= GL_NEAREST_MIPMAP_NEAREST && filter <= GL_LINEAR_MIPMAP_LINEAR)
		glGenerateMipmap(GL_TEXTURE_2D);

	if (delPixels)
		delete[] pixels;

	if (!_texId) {
		qWarning() << "Can't create texture from " + path << __DEBUG_INFO__;
		return false;
	}

	return true;
}

void Texture::resize(const QSize &size) {
	_width = size.width();
	_height = size.height();
	glBindTexture(target, _texId);
	glTexImage2D(target, 0, internalFormat, _width, _height, 0, format, type, nullptr);
}

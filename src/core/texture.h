#ifndef TEXTURE_H
#define TEXTURE_H

#include <qglobal.h>
#ifndef Q_OS_MAC
	#include <GL/glew.h>
#else
	#include <qgl.h>
#endif
#include "../utils/hdrimage.h"

class Texture {
	unsigned int _texId = 0;
	int target;
	int _width;
	int _height;
	int internalFormat;
	int format;
	int type;

public:
	Texture();
	Texture(const QString &path, int format, int filter, int wrap);
	~Texture();

	bool createHdrCubemap(const QString &path, HdrImage::Format hdrFormat,
						  int filter, int wrap);

	bool createHdrCubemap(const HdrImage &hdr, int filter, int wrap);

	void createTexture(int internalFormat, int width, int height, int format,
					   int type, int filter, int wrap, const void* pix=nullptr);

	bool createTexture(const QString &path, int format, int filter, int wrap);

	void resize(const QSize &size);

	int bind(int num);

	inline unsigned int texId() const {
		return _texId;
	}

	inline int height() const {
		return _height;
	}

	inline int width() const {
		return _width;
	}
};

#endif // TEXTURE_H

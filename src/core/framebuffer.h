#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "texture.h"
#include <QStringList>
#include <QVector>
#ifndef Q_OS_MAC
	#include <GL/glew.h>
#else
	#include <qgl.h>
#endif

class FrameBuffer {
	static GLuint currFbo;

	QVector<GLenum> attachment;
	QStringList texNames;
	QList<Texture *> textures;
	unsigned int fbo;
	int _scale;
	QSize size;
	QSize scaledSize;

	void updateSize();

public:
	FrameBuffer(const QSize &size, int scale=1);
	~FrameBuffer();

	void rescale(int scale);
	void resize(const QSize &size);
	bool addRenderTarget(int internalFormat, int format, int type,
						 int filter, int wrap, const QString &name);
	bool create();
	void bind();
	static void unbind(const QSize &viewportSize);
	static void unbind();

	Texture *texture(const QString &name) const;
	Texture *texture(int index=0) const;

	inline int scale() const {
		return _scale;
	}
};

#endif // FRAMEBUFFER_H

#include "framebuffer.h"
#include "../debug.h"

GLuint FrameBuffer::currFbo = 0;

FrameBuffer::FrameBuffer(const QSize &size, int scale) {
	attachment.reserve(4);
	glGenFramebuffers(1, &fbo);
	resize(size);
	rescale(scale);
}

FrameBuffer::~FrameBuffer() {
	for (Texture *t: textures)
		delete t;
}

void FrameBuffer::updateSize() {
	if (_scale>1)
		scaledSize = size/_scale;
	else
		scaledSize = size;

	bind();
	for (Texture *t: textures)
		t->resize(scaledSize);
}

void FrameBuffer::rescale(int scale) {
	this->_scale = scale;
	updateSize();
}

void FrameBuffer::resize(const QSize &size) {
	this->size = size;
	updateSize();
}

bool FrameBuffer::addRenderTarget(int internalFormat, int format, int type,
								  int filter, int wrap, const QString &name)
{
	if (texNames.contains(name)) {
		qWarning() << "Render target" << name << "already exist" << __DEBUG_INFO__;
		return false;
	}

	bind();

	Texture *tex = new(std::nothrow) Texture();
	if (tex==nullptr) {
		qWarning() << "Can't create texture for render target: " << name << __DEBUG_INFO__;
		return false;
	}

	tex->createTexture(internalFormat, scaledSize.width(), scaledSize.height(),
						  format, type, filter, wrap, nullptr);

	if (format != GL_DEPTH_COMPONENT) {
		attachment.append(GL_COLOR_ATTACHMENT0+attachment.count());
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.last(), GL_TEXTURE_2D, tex->texId(), 0);
	} else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->texId(), 0);

	texNames.append(name);
	textures.append(tex);

	return true;
}

bool FrameBuffer::create() {
	bind();

	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE) {
		qWarning() << "Can't create framebuffer:" << err << __DEBUG_INFO__;
		return false;
	}

	glDrawBuffers(attachment.count(), attachment.data());

	return true;
}

void FrameBuffer::bind() {
	if (fbo==currFbo)
		return;

	currFbo = fbo;
	glBindFramebuffer(GL_FRAMEBUFFER, currFbo);
	glViewport(0, 0, scaledSize.width(), scaledSize.height());
}

void FrameBuffer::unbind(const QSize &viewportSize) {
	unbind();
	glViewport(0, 0, viewportSize.width(), viewportSize.height());
}

void FrameBuffer::unbind() {
	currFbo = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, currFbo);
}

Texture *FrameBuffer::texture(const QString &name) const {
	int i = texNames.indexOf(name);
	if (i<0)
		return nullptr;

	return textures[i];
}

Texture *FrameBuffer::texture(int index) const {
	if (index<0 || index>=textures.count())
		return nullptr;

	return textures[index];
}

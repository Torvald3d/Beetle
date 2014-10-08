#include "downsamplematerial.h"

DownsampleMaterial::DownsampleMaterial(QGLShaderProgram *shader): QuadMaterial(shader) {
	adrSize = shader->uniformLocation("screen");
	adrAspect = shader->uniformLocation("aspect");
}

DownsampleMaterial::~DownsampleMaterial() {

}

bool DownsampleMaterial::bind() {
	if (QuadMaterial::bind()) {
		shader->setUniformValue(adrSize, size);
		shader->setUniformValue(adrAspect, aspect);
		return true;
	}
	return false;
}

void DownsampleMaterial::setSize(const QSize &size) {
	this->size = size;
}

void DownsampleMaterial::setAspect(float aspect) {
	this->aspect = aspect;

}

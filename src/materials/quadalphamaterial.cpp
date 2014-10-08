#include "quadalphamaterial.h"

QuadAlphaMaterial::QuadAlphaMaterial(QGLShaderProgram *shader): QuadMaterial(shader) {
	adrAlpha = shader->uniformLocation("alpha");
}

QuadAlphaMaterial::~QuadAlphaMaterial() {

}

bool QuadAlphaMaterial::bind() {
	if (QuadMaterial::bind()) {
		shader->setUniformValue(adrAlpha, alpha);
		return true;
	}
	return false;
}

void QuadAlphaMaterial::setAlpha(float alpha) {
	this->alpha = alpha;
}

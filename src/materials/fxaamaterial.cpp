#include "fxaamaterial.h"

FxaaMaterial::FxaaMaterial(QGLShaderProgram *shader): QuadMaterial(shader) {
	adrScreenSize = shader->uniformLocation("screenSize");
}

FxaaMaterial::~FxaaMaterial() {

}

bool FxaaMaterial::bind() {
	if (QuadMaterial::bind()) {
		shader->setUniformValue(adrScreenSize, screenSize);
		return true;
	}
	return false;
}

void FxaaMaterial::setScreenSize(const QSize &screenSize) {
	this->screenSize = screenSize;
}

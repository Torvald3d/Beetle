#include "quadmaterial.h"

QuadMaterial::QuadMaterial(QGLShaderProgram *shader): Material(shader) {
	adrTexture = shader->uniformLocation("texture");
}

QuadMaterial::~QuadMaterial() {

}

bool QuadMaterial::bind() {
	if (Material::bind()) {
		if (!texture)
			return false;

		shader->setUniformValue(adrTexture, texture->bind(0));
		return true;
	}
	return false;
}

void QuadMaterial::setTexture(Texture *texture) {
	this->texture = texture;
}

#include <qglobal.h>
#ifndef Q_OS_MAC
	#include <GL/glew.h>
#else
	#include <qgl.h>
#endif
#include "material.h"

QGLShaderProgram *Material::currShader = nullptr;

Material::Material(QGLShaderProgram *shader) {
	if (shader==nullptr)
		if (!shader->bind())
			throw 1;

	this->shader = shader;

	adrVertex = shader->attributeLocation("vertex");
	adrNormal = shader->attributeLocation("normal");
	adrTangent = shader->attributeLocation("tangent");
	adrTexCoord = shader->attributeLocation("texCoord");
}

Material::~Material() {

}

bool Material::bind() {
	if (shader==nullptr)
		return false;

	if (shader!=currShader) {
		currShader = shader;
		return currShader->bind();
	}
	return true;
}

void Material::unbind() {
	if (currShader==nullptr)
		return;

	currShader->release();
	currShader = nullptr;
}

int Material::adrAtrVertex() {
	return adrVertex;
}

int Material::adrAtrNormal() {
	return adrNormal;
}

int Material::adrAtrTangent() {
	return adrTangent;
}

int Material::adrAtrTexCoord() {
	return adrTexCoord;
}

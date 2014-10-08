#include "envmaterial.h"
#include <QGLShaderProgram>

EnvMaterial::EnvMaterial(QGLShaderProgram *shader): Material(shader) {
	adrEnvMap = shader->uniformLocation("envMap");
	adrFov = shader->uniformLocation("fov");
	adrAspect = shader->uniformLocation("aspect");
	adrInvViewMat = shader->uniformLocation("invViewMat");
	adrBright = shader->uniformLocation("bright");
}

EnvMaterial::~EnvMaterial() {

}

bool EnvMaterial::bind() {
	if (Material::bind()) {
		if (!(camera && envMap))
			return false;

		shader->setUniformValue(adrInvViewMat, camera->invViewMat());
		shader->setUniformValue(adrEnvMap, envMap->bind(0));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		shader->setUniformValue(adrFov, camera->fov());
		shader->setUniformValue(adrAspect, camera->aspect());
		shader->setUniformValue(adrBright, bright);

		return true;
	}
	return false;
}

void EnvMaterial::setCamera(Camera *camera) {
	this->camera = camera;
}

void EnvMaterial::setEnvMap(Texture *envMap) {
	this->envMap = envMap;
}

void EnvMaterial::setBright(float bright) {
	this->bright = bright;
}

#include "colormaterial.h"

ColorMaterial::ColorMaterial(QGLShaderProgram *shader): QuadMaterial(shader) {
	adrMatrixProj = shader->uniformLocation("matrixProj");
	adrMatrixView = shader->uniformLocation("matrixView");
	adrLight = shader->uniformLocation("light");
	adrColor = shader->uniformLocation("color");
	adrEnvMap = shader->uniformLocation("envMap");
	adrBright = shader->uniformLocation("bright");
	adrFocalDistance = shader->uniformLocation("focalDistance");
	adrFocalRange = shader->uniformLocation("focalRange");
	adrEyeDir = shader->uniformLocation("eyeDir");
	adrHueAverage = shader->uniformLocation("hueAverage");
	adrHueRange = shader->uniformLocation("hueRange");
}

ColorMaterial::~ColorMaterial() {

}

bool ColorMaterial::bind() {
	if (QuadMaterial::bind()) {
		if (!camera)
			return false;

		shader->setUniformValue(adrMatrixProj, camera->projMat());
		shader->setUniformValue(adrMatrixView, camera->viewMat());
		shader->setUniformValue(adrLight, light);
		shader->setUniformValue(adrFocalDistance, focalDistance);
		shader->setUniformValue(adrFocalRange, focalRange);

		if (adrColor>=0)
			shader->setUniformValue(adrColor, color);
		if (adrEnvMap>=0) {
			shader->setUniformValue(adrEnvMap, envMap->bind(1));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 6);
		}
		if (adrBright>=0)
			shader->setUniformValue(adrBright, bright);
		if (adrEyeDir>=0) {
			QVector4D eyeDir(0.0, 0.0, 1.0, 0.0);
			eyeDir = eyeDir*camera->viewMat();
			shader->setUniformValue(adrEyeDir, eyeDir.toVector3D());
		}
		if (adrHueAverage>=0 && adrHueRange>=0) {
			shader->setUniformValue(adrHueAverage, hueAverage);
			shader->setUniformValue(adrHueRange, hueRange);
		}

		return true;
	}
	return false;
}

void ColorMaterial::setCamera(Camera *camera) {
	this->camera = camera;
}

void ColorMaterial::setLight(const QVector3D &light) {
	this->light = light;
}

void ColorMaterial::setColor(const QColor &color) {
	this->color = color;
}

void ColorMaterial::setEnvMap(Texture *envMap) {
	this->envMap = envMap;
}

void ColorMaterial::setBright(float bright) {
	this->bright = bright;
}

void ColorMaterial::setFocalDistance(float focalDistance) {
	this->focalDistance = focalDistance;
}

void ColorMaterial::setFocalRange(float focalRange) {
	this->focalRange = focalRange;
}

void ColorMaterial::setHueAverage(float hueAverage) {
	this->hueAverage = hueAverage;
}

void ColorMaterial::setHueRange(float hueRange) {
	this->hueRange = hueRange;
}

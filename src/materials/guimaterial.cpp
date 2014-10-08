#include "guimaterial.h"
#include "../debug.h"

GuiMaterial::GuiMaterial(QGLShaderProgram *shader): Material(shader) {
	adrCamera = shader->uniformLocation("projMat");
	adrGuiMap = shader->uniformLocation("guiMap");
	adrThemeMap = shader->uniformLocation("themeMap");
	adrThemeNum = shader->uniformLocation("themeNum");
}

GuiMaterial::~GuiMaterial() {

}

bool GuiMaterial::bind() {
	if (Material::bind()) {
		if (!guiMap || !themeMap || !camera)
			return false;

		shader->setUniformValue(adrCamera, camera->projMat());
		shader->setUniformValue(adrGuiMap, guiMap->bind(0));
		shader->setUniformValue(adrThemeMap, themeMap->bind(1));
		shader->setUniformValue(adrThemeNum, themeNum);

		return true;
	}
	return false;
}

void GuiMaterial::setCamera(Camera *camera) {
	this->camera = camera;
}

void GuiMaterial::setGuiMap(Texture *guiMap) {
	this->guiMap = guiMap;
}

void GuiMaterial::setThemeMap(Texture *themeMap) {
	this->themeMap = themeMap;
	themeMap->bind(0);
	unsigned char *buff = new unsigned char[themeMap->width()*themeMap->height()*4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);
	_tone = (static_cast<int>(buff[0])+buff[1]+buff[2])/3;
	delete[] buff;
}

void GuiMaterial::setThemeNum(float themeNum) {
	if (!themeMap) {
		qWarning() << "themeMap must be init first" << __DEBUG_INFO__;
		return;
	}

	this->themeNum = 1-(themeNum+0.5)/(themeMap->height());
}

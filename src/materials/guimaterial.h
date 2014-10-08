#ifndef GUIMATERIAL_H
#define GUIMATERIAL_H

#include "../core/texture.h"
#include "../core/material.h"
#include "../core/camera.h"

class GuiMaterial: public Material {
	Camera *camera = nullptr;
	Texture *guiMap = nullptr;
	Texture *themeMap = nullptr;
	float themeNum = 0;
	unsigned char _tone = 255;

	int adrCamera;
	int adrGuiMap;
	int adrThemeMap;
	int adrThemeNum;

public:
	GuiMaterial(QGLShaderProgram *shader);
	virtual ~GuiMaterial();

	virtual bool bind() override;

	void setCamera(Camera *camera);
	void setGuiMap(Texture *guiMap);
	void setThemeMap(Texture *themeMap);
	void setThemeNum(float themeNum);

	inline unsigned char tone() const {
		return _tone;
	}
};

#endif // GUIMATERIAL_H

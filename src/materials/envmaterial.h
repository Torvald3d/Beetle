#ifndef ENVMATERIAL_H
#define ENVMATERIAL_H

#include "../core/texture.h"
#include "../core/material.h"
#include "../core/camera.h"

class EnvMaterial: public Material {
	Camera *camera = nullptr;
	Texture *envMap = nullptr;
	float bright = 1;

	int adrEnvMap;
	int adrFov;
	int adrAspect;
	int adrInvViewMat;
	int adrBright;

public:
	EnvMaterial(QGLShaderProgram *shader);
	virtual ~EnvMaterial();

	virtual bool bind() override;

	void setCamera(Camera *camera);
	void setEnvMap(Texture *envMap);
	void setBright(float bright);

};

#endif // ENVMATERIAL_H

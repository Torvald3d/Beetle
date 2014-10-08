#ifndef QUADMATERIAL_H
#define QUADMATERIAL_H

#include "../core/texture.h"
#include "../core/material.h"

class QuadMaterial: public Material {
	Texture *texture = nullptr;
	int adrTexture;

public:
	QuadMaterial(QGLShaderProgram *shader);
	virtual ~QuadMaterial();

	virtual bool bind() override;

	void setTexture(Texture *texture);
};

#endif // QUADMATERIAL_H

#ifndef QUADALPHAMATERIAL_H
#define QUADALPHAMATERIAL_H

#include "quadmaterial.h"

class QuadAlphaMaterial: public QuadMaterial {
	float alpha;
	int adrAlpha;

public:
	QuadAlphaMaterial(QGLShaderProgram *shader);
	virtual ~QuadAlphaMaterial();

	virtual bool bind() override;

	void setAlpha(float alpha);
};

#endif // QUADALPHAMATERIAL_H

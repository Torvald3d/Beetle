#ifndef FXAAMATERIAL_H
#define FXAAMATERIAL_H

#include "quadmaterial.h"

class FxaaMaterial: public QuadMaterial {
	QSize screenSize;
	int adrScreenSize;

public:
	FxaaMaterial(QGLShaderProgram *shader);
	virtual ~FxaaMaterial();

	virtual bool bind() override;
	void setScreenSize(const QSize &screenSize);
};

#endif // FXAAMATERIAL_H

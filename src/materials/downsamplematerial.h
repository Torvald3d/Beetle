#ifndef DOWNSAMPLEMATERIAL_H
#define DOWNSAMPLEMATERIAL_H

#include "quadmaterial.h"

class DownsampleMaterial: public QuadMaterial {
	QSize size;
	float aspect;

	int adrSize;
	float adrAspect;

public:
	DownsampleMaterial(QGLShaderProgram *shader);
	virtual ~DownsampleMaterial();

	virtual bool bind() override;
	void setSize(const QSize &size);
	void setAspect(float aspect);
};

#endif // DOWNSAMPLEMATERIAL_H

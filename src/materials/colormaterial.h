#ifndef COLORMATERIAL_H
#define COLORMATERIAL_H

#include "quadmaterial.h"
#include "../core/camera.h"

class ColorMaterial: public QuadMaterial {
	Camera *camera = nullptr;
	Texture *envMap = nullptr;
	QVector3D light;
	QColor color;
	float bright;
	float focalDistance;
	float focalRange;
	float hueAverage;
	float hueRange;

	int adrMatrixProj;
	int adrMatrixView;
	int adrLight;
	int adrColor;
	int adrEnvMap;
	int adrBright;
	int adrFocalDistance;
	int adrFocalRange;
	int adrEyeDir;
	int adrHueAverage;
	int adrHueRange;

public:
	ColorMaterial(QGLShaderProgram *shader);
	virtual ~ColorMaterial();

	virtual bool bind() override;
	void setCamera(Camera *camera);
	void setLight(const QVector3D &light);
	void setColor(const QColor &color);
	void setEnvMap(Texture *envMap);
	void setBright(float bright);
	void setFocalDistance(float focalDistance);
	void setFocalRange(float focalRange);
	void setHueAverage(float hueAverage);
	void setHueRange(float hueRange);

};

#endif // COLORMATERIAL_H

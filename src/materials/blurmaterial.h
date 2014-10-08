#ifndef BLURMATERIAL_H
#define BLURMATERIAL_H

#include "quadmaterial.h"

class BlurMaterial: public QuadMaterial {
	const int MAX_OFFSET_SIZE = 100;
	const float DELTA_SAMPLES = 0.0035;

	float blurSize = 4;
	Texture *lowTexture = nullptr;
	QVector<QVector2D> offsets;
	QSize screenSize = QSize(1280, 720);
	float seed;

	int adrOffsetSize;
	int adrOffsets;
	int adrLowTexture;
	int adrSeed;

	void updateOffsets();

public:
	BlurMaterial(QGLShaderProgram *shader);
	virtual ~BlurMaterial();

	virtual bool bind() override;
	void setBlurSize(float blurSize);
	void setScreenSize(const QSize &screenSize);
	void setLowTexture(Texture *lowTexture);
	void setSeed(float seed);

};

#endif // BLURMATERIAL_H

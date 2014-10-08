#include "blurmaterial.h"
#include <math.h>
#include "utils/math.h"

BlurMaterial::BlurMaterial(QGLShaderProgram *shader): QuadMaterial(shader) {
	adrOffsetSize = shader->uniformLocation("offsetSize");
	adrOffsets = shader->uniformLocation("offsets");
	adrLowTexture = shader->uniformLocation("lowTexture");
	adrSeed = shader->uniformLocation("seed");
	offsets.reserve(MAX_OFFSET_SIZE);
}

BlurMaterial::~BlurMaterial() {

}

void BlurMaterial::updateOffsets() {
	offsets.clear();
	float aspect = static_cast<float>(screenSize.width())/screenSize.height();
	float radius = blurSize*screenSize.width()/1000000.0;
	if (radius/DELTA_SAMPLES>sqrt(MAX_OFFSET_SIZE/M_PI))
		radius = sqrt(MAX_OFFSET_SIZE/M_PI)*DELTA_SAMPLES;
	float yMax = radius;
	float yMin = -radius;
	yMin += fmod(yMax-yMin, DELTA_SAMPLES)/2;
	for (float y=yMin; y<yMax; y+=DELTA_SAMPLES) {
		float xMax = cos(asin(y/radius))*radius;
		float xMin = -xMax;
		xMin += fmod(xMax-xMin, DELTA_SAMPLES)/2;
		for (float x=xMin; x<xMax; x+=DELTA_SAMPLES)
			offsets.append(QVector2D((x+Math::realRand(-DELTA_SAMPLES/4, DELTA_SAMPLES/4)),
									 (y+Math::realRand(-DELTA_SAMPLES/4, DELTA_SAMPLES/4))*aspect));
	}

	qDebug() << "Blur radius:" << radius << "samples count:" << offsets.count();
}

bool BlurMaterial::bind() {
	if (QuadMaterial::bind()) {

		shader->setUniformValue(adrOffsetSize, offsets.count());
		shader->setUniformValueArray(adrOffsets, offsets.data(), offsets.count());
		shader->setUniformValue(adrLowTexture, lowTexture->bind(1));
		shader->setUniformValue(adrSeed, seed);

		return true;
	}
	return false;
}

void BlurMaterial::setBlurSize(float blurSize) {
	this->blurSize = blurSize;
	updateOffsets();
}

void BlurMaterial::setScreenSize(const QSize &screenSize) {
	this->screenSize = screenSize;
	updateOffsets();
}

void BlurMaterial::setLowTexture(Texture *lowTexture) {
	this->lowTexture = lowTexture;
}

void BlurMaterial::setSeed(float seed) {
	this->seed = seed;
}

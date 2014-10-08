#ifndef MATERIAL_H
#define MATERIAL_H

#include <QGLShaderProgram>

class Material {
	static QGLShaderProgram *currShader;

protected:
	QGLShaderProgram *shader;

	int adrVertex;
	int adrNormal;
	int adrTangent;
	int adrTexCoord;

public:
	Material(QGLShaderProgram *shader);
	virtual ~Material();

	virtual bool bind();
	static void unbind();

	int adrAtrVertex();
	int adrAtrNormal();
	int adrAtrTangent();
	int adrAtrTexCoord();
};

#endif // MATERIAL_H

#ifndef MODEL_H
#define MODEL_H

#include <qglobal.h>
#ifndef Q_OS_MAC
	#include <GL/glew.h>
#endif
#include <QGLBuffer>
#include "mesh.h"
#include "material.h"

class AbstractModel {
protected:
	template<typename T> bool createBuffer(QGLBuffer &buff, QVector<T> &vec);
	Material *material = nullptr;

public:
	virtual bool updateBuffers() = 0;
	virtual void render() = 0;
	virtual void setMaterial(Material *material);
};


class Model: public Mesh, public AbstractModel {
protected:
	QGLBuffer vertBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer normBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer tangBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer tcBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer indBuff = QGLBuffer(QGLBuffer::IndexBuffer);

public:
	Model();
	Model(Material *material);
	Model(const QString &path, Material *material);
	virtual ~Model();

	virtual bool updateBuffers() override;
	virtual void render() override;
	bool load(const QString &path);
};

#endif // MODEL_H

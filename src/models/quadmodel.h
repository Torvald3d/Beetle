#ifndef QUADMESH_H
#define QUADMESH_H

#include "../core/model.h"
#include "../utils/geometry.h"

// TODO: change to 2D and derive from AbstractModel

class QuadModel: public Mesh2D, public AbstractModel, public Geometry {
protected:
	QGLBuffer vertBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer tcBuff = QGLBuffer(QGLBuffer::VertexBuffer);
	QGLBuffer indBuff = QGLBuffer(QGLBuffer::IndexBuffer);
	bool useVbo;

public:
	QuadModel(bool useVbo=true);
	QuadModel(Material *material, bool useVbo=true);
	QuadModel(const QRectF &rect, Material *material=nullptr, bool useVbo=true);
	virtual ~QuadModel();

	void updateGeometry();
	void resize(const QRectF &rect);
	void setPosition(const QPointF &position);
	virtual bool updateBuffers() override;
	virtual void render() override;
};

#endif // QUADMESH_H

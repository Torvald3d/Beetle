#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

class Mesh2D {
protected:
	QVector<QVector2D> vertices;
	QVector<QVector2D> texCoord;
	QVector<unsigned int> indices;

};


class Mesh {
protected:
	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
	QVector<QVector3D> tangetns;
	QVector<QVector2D> texCoord;
	QVector<unsigned int> indices;

};

#endif // MESH_H

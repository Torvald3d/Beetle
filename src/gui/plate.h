#ifndef PLATE_H
#define PLATE_H

#include <qglobal.h>
#ifndef Q_OS_MAC
	#include <GL/glew.h>
#endif
#include <QRectF>
#include <QVector>
#include <QVector2D>
#include <QGLBuffer>
#include "../utils/geometry.h"
#include "../core/material.h"

// TODO: move to models folder and derive from AbstractModel

class Plate: public Geometry {
	QVector <QVector2D> vertices;
	static QVector <QVector2D> texCoord;
	static QVector <unsigned short> indices;
	static QGLBuffer tcBuff;
	static QGLBuffer indBuff;
	QGLBuffer vertBuff = QGLBuffer(QGLBuffer::VertexBuffer);

	float _borderSize;
	float _theme;
	bool modified = true;
	Material *material = nullptr;

	void updateGeometry();

public:
	Plate();
	Plate(const QRectF &geometry, float borderSize = -1);
	~Plate();

	void setBorderSize(float borderSize);
	void setSize(const QSizeF &size);
	void setGeometry(const QRectF &geometry, float borderSize = -1);
	void setGeometry(const QRectF &geometry, const QPointF &localPos, float borderSize = -1);
	void setPosition(const QPointF &position);
	void setTheme(float theme);
	void setMaterial(Material *material);
	void render();

	inline float borderSize() const {
		return _borderSize;
	}

	inline QRectF innerGeometry() const {
		return QRectF(_geometry.left()+_borderSize, _geometry.top()-_borderSize,
					  _geometry.width()-_borderSize*2, _geometry.height()-_borderSize*2);
	}

	inline QSizeF innerSize() const {
		return QSizeF(_geometry.width()-_borderSize*2, _geometry.height()-_borderSize*2);
	}

	inline float innerWidth() const {
		return _geometry.width()-_borderSize*2;
	}

	inline float innerHeight() const {
		return _geometry.height()-_borderSize*2;
	}

	inline float theme() const {
		return _theme;
	}

};
#endif // PLATE_H

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QRectF>

class Geometry {
protected:
	QRectF _geometry;

public:
	inline const QRectF &geometry() const {
		return _geometry;
	}

	inline QPointF position() const {
		return _geometry.topLeft();
	}

	inline float left() const {
		return _geometry.left();
	}

	inline float top() const {
		return _geometry.top();
	}

	inline float right() const {
		return _geometry.right();
	}

	inline float bottom() const {
		return _geometry.bottom();
	}

	inline QSizeF size() const {
		return _geometry.size();
	}

	inline float width() const {
		return _geometry.width();
	}

	inline float height() const {
		return _geometry.height();
	}

};

#endif // GEOMETRY_H

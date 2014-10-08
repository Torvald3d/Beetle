#ifndef LAYOUT_H
#define LAYOUT_H

#include <QRectF>
#include <QList>
#include "../utils/geometry.h"

class Layout: public Geometry {
public:
	enum Orientation {Horizontal, Vertical};

	Layout();
	Layout(Orientation orientation, float space=5, const QPointF &border={15, 15});
	virtual ~Layout();

	static void setDpi(int dpi);

	virtual void updateGeometry();
	virtual void addItem(Layout *layout);
	void setSize(const QSizeF &size);
	void setOrientation(Orientation orientation);
	void setBorder(const QPointF &border);
	void setSpace(float space);
	void setPosition(const QPointF &position);
	void setLocalPos(const QPointF &localPos);
	QPointF mapToRoot(const QPointF &point);
	QPointF mapFromRoot(const QPointF &point);

	template<typename T> void findElements(QList<T *> *list) {
		T *t = dynamic_cast<T *>(this);
		if (t!=nullptr)
			list->append(t);

		for (Layout *layout: childs)
			layout->findElements(list);
	}

	inline float space() const {
		return _space;
	}

	inline const QPointF &border() const {
		return _border;
	}

	inline Orientation orientation() const {
		return _orientation;
	}

	inline const QPointF &localPos() const {
		return _localPos;
	}

protected:
	static float dpiFactor;
	static const float defaultDpi;

	Orientation _orientation;
	QPointF _border;
	float _space;
	QPointF _localPos = {0, 0};

	QList<Layout*> childs;
	Layout *parent = nullptr;
};

#endif // LAYOUT_H

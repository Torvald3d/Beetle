#include "layout.h"
#include "../debug.h"

float Layout::dpiFactor = 1;
const float Layout::defaultDpi = 100; // TODO: ...

Layout::Layout(): Layout(Horizontal) {

}

Layout::Layout(Orientation orientation, float space, const QPointF &border) {
	setOrientation(orientation);
	setSpace(space);
	setBorder(border);
}

Layout::~Layout() {
	for (Layout *layout: childs)
		delete layout;
}

// TODO: add alignment, stretch
void Layout::updateGeometry() {
	QPointF currPos = _border;
	_geometry.setSize({0, 0});

	for (Layout *layout: childs) {
		layout->updateGeometry();
		layout->setLocalPos(currPos);
		if (_orientation==Horizontal) {
			currPos.rx() += _space + layout->width();
			_geometry.setHeight(qMax(layout->height(), static_cast<float>(_geometry.height())));
		} else {
			currPos.ry() += _space + layout->height();
			_geometry.setWidth(qMax(layout->width(), static_cast<float>( _geometry.width())));
		}
	}

	if (_orientation==Horizontal) {
		if (childs.count()>0)
			currPos.rx() -= _space;
		_geometry.setWidth(currPos.x()+_border.x());
		_geometry.setHeight(_geometry.height()+_border.y()*2);
	} else {
		if (childs.count()>0)
			currPos.ry() -= _space;
		_geometry.setWidth(_geometry.width()+_border.x()*2);
		_geometry.setHeight(currPos.y()+_border.y());
	}
}

void Layout::addItem(Layout *layout) {
	if (layout==nullptr) {
		qWarning() << "layout is null" << __DEBUG_INFO__;
		return;
	}

	childs.append(layout);
	layout->parent = this;
	//updateGeometry();
}

void Layout::setSize(const QSizeF &size) {
	_geometry.setSize(size);
}

void Layout::setOrientation(Layout::Orientation orientation) {
	_orientation = orientation;
	//updateGeometry();
}

void Layout::setBorder(const QPointF &border) {
	_border = border;
	//updateGeometry();
}

void Layout::setSpace(float space) {
	_space = space;
	//updateGeometry();
}

void Layout::setLocalPos(const QPointF &localPos) {
	_localPos = localPos;
}

QPointF Layout::mapToRoot(const QPointF &point) {
	if (parent==nullptr)
		return point+localPos();

	return parent->mapToRoot(point+localPos());
}

QPointF Layout::mapFromRoot(const QPointF &point) {
	if (parent==nullptr)
		return point-localPos();

	return parent->mapFromRoot(point-localPos());
}

void Layout::setDpi(int dpi) {
	dpiFactor = dpi/defaultDpi;
}

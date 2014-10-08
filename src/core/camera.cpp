#include "camera.h"
#include <cmath>

#ifndef M_PI
	#define M_PI 3.1415926535
#endif

Camera::Camera() {

}

Camera::~Camera() {

}

void Camera::initPerspective(float angle, float aspect, float near, float far) {
	_fov = tanf(M_PI/180.0f*angle/2.0f);
	_aspect = aspect;
	_near = near;
	_far = far;
	_projMat.setToIdentity();
	_projMat.perspective(angle, aspect, near, far);
}

void Camera::initOrtho(int left, int right, int bottom, int top, int near, int far) {
	_near = near;
	_far = far;
	_projMat.setToIdentity();
	_projMat.ortho(left, right, bottom, top, near, far);
}

void Camera::rotate(const QPointF &angle) {
	_rotate += QVector3D(angle.y(), angle.x(), 0);
	updateTransform();
}

void Camera::setDistance(float value) {
	_distance = value;
	updateTransform();
}

void Camera::updateTransform() {
	_viewMat.setToIdentity();
	_viewMat.translate(0, 0, _distance);
	_viewMat.rotate(_rotate.x(), 1, 0, 0);
	_viewMat.rotate(_rotate.y(), 0, 1, 0);
	_viewMat.rotate(_rotate.z(), 0, 0, 1);

	_invViewMat = _viewMat.inverted();
}

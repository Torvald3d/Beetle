#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera {
	QMatrix4x4 _projMat;
	QMatrix4x4 _viewMat;
	QMatrix4x4 _invViewMat;
	float _near;
	float _far;
	float _fov;
	float _aspect;

	QVector3D _rotate = {0, 0, 0};
	float _distance = 1;
	void updateTransform();

public:
	Camera();
	~Camera();

	void initPerspective(float angle, float aspect, float near, float far);
	void initOrtho(int left, int right, int bottom, int top, int near, int far);
	void rotate(const QPointF &angle);
	void setDistance(float value);

	inline float near() {
		return _near;
	}

	inline float far() {
		return _far;
	}

	inline float fov() {
		return _fov;
	}

	inline float aspect() {
		return _aspect;
	}

	inline QMatrix4x4 &projMat() {
		return _projMat;
	}

	inline QMatrix4x4 &viewMat() {
		return _viewMat;
	}

	inline QMatrix4x4 &invViewMat() {
		return _invViewMat;
	}
};

#endif // CAMERA_H

#ifndef TIME_H
#define TIME_H

#include <QDateTime>

class Time {
	bool _isPause = false;
	qint64 lastTime = QDateTime::currentMSecsSinceEpoch();

public:
	const int fixedUpdateInterval = 16;

	inline void tic() {
		lastTime = QDateTime::currentMSecsSinceEpoch();
	}

	inline float delta() {
		if (!_isPause)
			return (QDateTime::currentMSecsSinceEpoch()-lastTime)/1000.0f;
		else
			return 0;
	}

	inline void pause() {
		_isPause = true;
	}

	inline void resume() {
		lastTime = -fixedUpdateInterval;
		lastTime += QDateTime::currentMSecsSinceEpoch();
		_isPause = false;
	}

	inline bool isPause() {
		return _isPause;
	}
};
#endif // TIME_H

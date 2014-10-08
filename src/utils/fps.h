#ifndef FPS_H
#define FPS_H

#include <QString>
#include <QDateTime>

class Fps {
private:
	int frames = 0;
	int fps = -1;
	qint64 lastTime = QDateTime::currentMSecsSinceEpoch();

public:
	inline void tic() {
		frames++;
	}

	inline QString getFps() {
		if (QDateTime::currentMSecsSinceEpoch()-lastTime >= 1000) {
			lastTime = QDateTime::currentMSecsSinceEpoch();
			fps = frames;
			frames = 0;
		}
		return "FPS: "+QString::number(fps);
	}
};

#endif // FPS_H

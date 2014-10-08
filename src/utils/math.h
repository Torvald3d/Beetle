#ifndef MATH_H
#define MATH_H

#include <stdlib.h>

class Math {
public:
	static inline float clamp(float value, float min, float max) {
		if (value >= max)
			return max;
		if (value <= min)
			return min;
		return value;
	}

	static float realRand(float min, float max) {
		return static_cast<float>(rand())/RAND_MAX*(max-min)+min;
	}
};

#endif // MATH_H

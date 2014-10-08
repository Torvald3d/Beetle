#ifndef COLOR_H
#define COLOR_H

struct Rgb8 {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct Rgba8 {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct Rgb32F {
	float r;
	float g;
	float b;
};

struct Rgb16 {
	unsigned short r;
	unsigned short g;
	unsigned short b;
};

#endif // COLOR_H

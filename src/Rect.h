#ifndef RECT_H
#define RECT_H

struct Rect
{
public:
	Rect( float x, float y, float width, float height);
	Rect();

	void clear();

	void Set(float x, float y, float width, float height);

	float x;
	float y;
	float width;
	float height;
};


#endif
#pragma once

#include <GL/freeglut.h>

class [[maybe_unused]] Object
{
public:
	[[maybe_unused]] Object(GLfloat _x, GLfloat _y, GLfloat _z);
	[[maybe_unused]] virtual void init() = 0;

	[[maybe_unused]] virtual auto getX() -> GLfloat;
	[[maybe_unused]] virtual auto getY() -> GLfloat;
	[[maybe_unused]] virtual auto getZ() -> GLfloat;

protected:
	GLfloat x, y, z;
};

[[maybe_unused]] Object::Object(float _x, float _y, float _z) : x(_x), y(_y),
																z(_z)
{
}

[[maybe_unused]] auto Object::getX() -> GLfloat
{
	return x;
}

[[maybe_unused]] auto Object::getY() -> GLfloat
{
	return y;
}

[[maybe_unused]] auto Object::getZ() -> GLfloat
{
	return z;
}

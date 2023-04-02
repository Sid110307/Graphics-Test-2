#pragma once

#include <utility>

#include "../../../src/include/camera.h"
#include "./objects.h"

class World
{
public:
	explicit World(std::vector<Object*>);
	~World() = default;

	static void create();
	static void addObject(Object*);
	static void setGridRange(GLint);

	static bool grid;
	static bool axes;
	static bool perspectiveIcon;

private:
	static std::vector<Object*> objects;
	static bool objectCreated;
	static GLint gridRange;

	static void showGrid();
	static void showAxes();
	static void showPerspectiveIcon();
};

bool World::grid = false;
bool World::axes = false;
bool World::perspectiveIcon = false;

std::vector<Object*> World::objects;
bool World::objectCreated = false;
GLint World::gridRange = 10.0f;

World::World(std::vector<Object*> _objects)
{
	objects = std::move(_objects);
}

void World::create()
{
	glEnable(GL_DEPTH_TEST | GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.55f, 0.75f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(Camera::x, Camera::y, Camera::z, Camera::x + Camera::lx, Camera::y + Camera::ly, Camera::z + Camera::lz,
			  0.0f, 1.0f, 0.0f);

	if (grid) showGrid();
	if (axes) showAxes();
	if (perspectiveIcon) showPerspectiveIcon();

	if (!objectCreated)
	{
		for (auto object: objects) object->create();
		objectCreated = true;
	}
}

void World::addObject(Object* _object)
{
	objects.push_back(_object);
}

void World::setGridRange(GLint range)
{
	gridRange = range;
}

void World::showGrid()
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);

	for (GLint i = -gridRange; i <= gridRange; i++)
	{
		glVertex3f((GLfloat) i, 0.0f, (GLfloat) -gridRange);
		glVertex3f((GLfloat) i, 0.0f, (GLfloat) gridRange);

		glVertex3f((GLfloat) -gridRange, 0.0f, (GLfloat) i);
		glVertex3f((GLfloat) gridRange, 0.0f, (GLfloat) i);
	}

	glEnd();
}

void World::showAxes()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f((GLfloat) gridRange / 2.5f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, (GLfloat) gridRange / 2.5f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, (GLfloat) gridRange / 2.5f);
	glEnd();
}

// TODO: Implement perspective icon/gizmo in the top right corner of the screen.
void World::showPerspectiveIcon()
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
}

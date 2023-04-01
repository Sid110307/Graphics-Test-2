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
	static void display();

	static void addObject(Object*);
	static void setGridRange(GLfloat);

	static auto setGrid(bool) -> bool;
	static auto setAxes(bool) -> bool;
	static auto setPerspectiveIcon(bool) -> bool;

private:
	static std::vector<Object*> objects;
	static bool objectInitialized;
	static GLfloat gridRange;

	static bool grid;
	static bool axes;
	static bool perspectiveIcon;

	static void showGrid();
	static void showAxes();
	static void showPerspectiveIcon();
};

std::vector<Object*> World::objects;
bool World::objectInitialized = false;
GLfloat World::gridRange = 10.0f;

bool World::grid = false;
bool World::axes = false;
bool World::perspectiveIcon = false;

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

	
}

void World::display()
{
	glClearColor(0.55f, 0.75f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(Camera::x, Camera::y, Camera::z, Camera::x + Camera::lx,
		Camera::y + Camera::ly, Camera::z + Camera::lz, 0.0f, 1.0f, 0.0f);

	if (grid) showGrid();
	if (axes) showAxes();
	if (perspectiveIcon) showPerspectiveIcon();

	if (!objectInitialized)
	{
		for (auto object : objects) object->init();
		objectInitialized = true;
	}

	glutSwapBuffers();
}

void World::addObject(Object* _object)
{
	objects.push_back(_object);
	
}

void World::setGridRange(GLfloat range)
{
	gridRange = range;
}

auto World::setGrid(bool _grid) -> bool
{
	grid = _grid;
	return grid;
}

auto World::setAxes(bool _axes) -> bool
{
	axes = _axes;
	return axes;
}

auto World::setPerspectiveIcon(bool _perspectiveIcon) -> bool
{
	perspectiveIcon = _perspectiveIcon;
	return perspectiveIcon;
}

void World::showGrid()
{
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);

	for (auto i = -gridRange; i <= gridRange; i += 0.5f)
	{
		glVertex3f(i, 0.0f, -gridRange);
		glVertex3f(i, 0.0f, gridRange);

		glVertex3f(-gridRange, 0.0f, i);
		glVertex3f(gridRange, 0.0f, i);
	}

	glEnd();
	
}

void World::showAxes()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(gridRange / 2.5f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, gridRange / 2.5f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, gridRange / 2.5f);
	glEnd();

	
}

void World::showPerspectiveIcon()
{
	// TODO: Implement perspective icon/gizmo at the top right corner of the screen.
	
}

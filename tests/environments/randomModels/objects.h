#pragma once

#include <utility>
#include "../../../src/include/object.h"

class BuiltinModels
{
#define SNOWMAN_SPREAD 3
#define BUILDING_SPREAD 3
#define TREE_SPREAD 3
#define WATER_SPREAD 3
#define SNOW_SPREAD 3

public:
	class Snowman : public Object
	{
	public:
		Snowman(GLfloat _x, GLfloat _y, GLfloat _z) : Object(_x, _y, _z)
		{
		}

		void draw() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.75f, 0.0f);
			glutSolidSphere(0.75f, 20, 20);

			glTranslatef(0.0f, 0.95f, 0.0f);
			glutSolidSphere(0.25f, 20, 20);

			glPushMatrix();
			glColor3f(0.0f, 0.0f, 0.0f);
			glTranslatef(0.05f, 0.10f, 0.18f);
			glutSolidSphere(0.05f, 10, 10);
			glTranslatef(-0.1f, 0.0f, 0.0f);
			glutSolidSphere(0.05f, 10, 10);
			glPopMatrix();

			glColor3f(1.0f, 0.5f, 0.5f);
			glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
			glutSolidCone(0.08f, 0.5f, 10, 2);
		}

		void init() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLfloat i = -SNOWMAN_SPREAD; i < SNOWMAN_SPREAD; ++i)
				for (GLfloat j = -SNOWMAN_SPREAD; j < SNOWMAN_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef(i * 2.0f, 0.0f, j * 2.0f);
					draw();
					glPopMatrix();
				}

			std::string txt = "Total snowmen: ";
			txt += std::to_string(
					(GLint) (SNOWMAN_SPREAD * 2 * SNOWMAN_SPREAD * 2));

			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos3f(-1.0f, 5.0f, 0.0f);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18,
							 (const unsigned char*) txt.c_str());
		}
	};

	class Building : public Object
	{
	public:
		Building(GLfloat _x, GLfloat _y, GLfloat _z) : Object(_x, _y, _z)
		{
		}

		void draw() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glColor3f(0.121f, 0.616f, 1.0f);

			glutSolidCube(1.0f);
			glTranslatef(0.0f, 1.0f, 0.0f);
			glutSolidCube(1.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
		}

		void init() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLfloat i = -BUILDING_SPREAD; i < BUILDING_SPREAD; ++i)
				for (GLfloat j = -BUILDING_SPREAD; j < BUILDING_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef(i * 2.0f, 0.0f, j * 2.0f);
					draw();
					glPopMatrix();
				}

			std::string txt = "Total buildings: ";
			txt += std::to_string(
					(GLint) (BUILDING_SPREAD * 2 * BUILDING_SPREAD * 2));

			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos3f(-1.0f, 5.0f, 0.0f);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18,
							 (const unsigned char*) txt.c_str());
		}
	};

	class Tree : public Object
	{
	public:
		Tree(GLfloat _x, GLfloat _y, GLfloat _z) : Object(_x, _y, _z)
		{
		}

		void draw() override
		{
			// TODO: Draw tree
		}

		void init() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLfloat i = -TREE_SPREAD; i < TREE_SPREAD; ++i)
				for (GLfloat j = -TREE_SPREAD; j < TREE_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef(i * 2.0f, 0.0f, j * 2.0f);
					draw();
					glPopMatrix();
				}

			std::string txt = "Total trees: ";
			txt += std::to_string((GLint) (TREE_SPREAD * 2 * TREE_SPREAD * 2));

			glRasterPos3f(-1.0f, 5.0f, 0.0f);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18,
							 (const unsigned char*) txt.c_str());
		}
	};

	class Water : public Object
	{
	public:
		Water(GLfloat _x, GLfloat _y, GLfloat _z) : Object(_x, _y, _z)
		{
		}

		void draw() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glColor3f(0.83f, 0.95f, 0.98f);
			glutSolidCube(1.0f);
		}

		void init() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLfloat i = -WATER_SPREAD; i < WATER_SPREAD; ++i)
				for (GLfloat j = -WATER_SPREAD; j < WATER_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef(i * 2.0f, 0.0f, j * 2.0f);
					draw();
					glPopMatrix();
				}

			std::string txt = "Total water blocks: ";
			txt += std::to_string(
					(GLint) (WATER_SPREAD * 2 * WATER_SPREAD * 2));

			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos3f(-1.0f, 5.0f, 0.0f);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18,
							 (const unsigned char*) txt.c_str());
		}
	};

	class Snow : public Object
	{
	public:
		Snow(GLfloat _x, GLfloat _y, GLfloat _z) : Object(_x, _y, _z)
		{
		}

		void draw() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glColor3f(1.0f, 0.98f, 0.98f);
			glutSolidCube(1.0f);
		}

		void init() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLfloat i = -SNOW_SPREAD; i < SNOW_SPREAD; ++i)
				for (GLfloat j = -SNOW_SPREAD; j < SNOW_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef(i * 2.0f, 0.0f, j * 2.0f);
					draw();
					glPopMatrix();
				}

			std::string txt = "Total snow blocks: ";
			txt += std::to_string((GLint) (SNOW_SPREAD * 2 * SNOW_SPREAD * 2));

			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos3f(-1.0f, 5.0f, 0.0f);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18,
							 (const unsigned char*) txt.c_str());
		}
	};
};

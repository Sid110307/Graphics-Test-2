#pragma once

#include <utility>
#include "../../../src/include/utils.h"

#define SNOWMAN_SPREAD 3
#define BUILDING_SPREAD 3
#define TREE_SPREAD 3
#define WATER_SPREAD 3
#define SNOW_SPREAD 3

class BuiltinModels
{
public:
	class Snowman : public Object
	{
	public:
		Snowman(const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z) : Object(_shader, _x, _y, _z) {}

		void create() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (auto i = -SNOWMAN_SPREAD; i < SNOWMAN_SPREAD; ++i)
				for (auto j = -SNOWMAN_SPREAD; j < SNOWMAN_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef((GLfloat) i * 2.0f, 0.0f, (GLfloat) j * 2.0f);
					glColor3f(1.0f, 1.0f, 1.0f);

					glTranslatef(0.0f, 0.75f, 0.0f);
					drawSphere(0.75f, 20, 20);

					glTranslatef(0.0f, 0.95f, 0.0f);
					drawSphere(0.25f, 20, 20);

					glPushMatrix();
					glColor3f(0.0f, 0.0f, 0.0f);
					glTranslatef(0.05f, 0.10f, 0.18f);
					drawSphere(0.05f, 10, 10);
					glTranslatef(-0.1f, 0.0f, 0.0f);
					drawSphere(0.05f, 10, 10);
					glPopMatrix();

					glColor3f(1.0f, 0.5f, 0.5f);
					glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
					drawCone(0.08f, 0.5f, 10, 2);

					glPopMatrix();
				}

			glColor3f(0.0f, 0.0f, 0.0f);
			putText(shader, "Total snowmen: " + std::to_string(pow((SNOWMAN_SPREAD * 2), 2)),
					glm::vec3(-1.0f, 5.0f, 0.0f), 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	};

	class Building : public Object
	{
	public:
		Building(const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z) : Object(_shader, _x, _y, _z) {}

		void create() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLint i = -BUILDING_SPREAD; i < BUILDING_SPREAD; ++i)
				for (GLint j = -BUILDING_SPREAD; j < BUILDING_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef((GLfloat) i * 2.0f, 0.0f, (GLfloat) j * 2.0f);
					glColor3f(0.121f, 0.616f, 1.0f);

					glTranslatef(0.0f, 0.5f, 0.0f);
					drawCube(1.0f);

					glTranslatef(0.0f, 0.5f, 0.0f);
					drawCube(1.0f);

					glTranslatef(0.0f, 0.5f, 0.0f);
					drawCube(1.0f);

					glPopMatrix();
				}

			glColor3f(0.0f, 0.0f, 0.0f);
			putText(shader, "Total buildings: " + std::to_string(pow((BUILDING_SPREAD * 2), 2)),
					glm::vec3(-1.0f, 5.0f, 0.0f), 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	};

	class Tree : public Object
	{
	public:
		Tree(const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z) : Object(_shader, _x, _y, _z) {}

		void create() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLint i = -TREE_SPREAD; i < TREE_SPREAD; ++i)
				for (GLint j = -TREE_SPREAD; j < TREE_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef((GLfloat) i * 2.0f, 0.0f, (GLfloat) j * 2.0f);
					glColor3f(0.0f, 0.5f, 0.0f);

					glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
					drawCone(0.25f, 1.0f, 10, 2);

					glTranslatef(0.0f, 1.0f, 0.0f);
					drawSphere(0.5f, 10, 10);
					glPopMatrix();
				}

			glColor3f(0.0f, 0.0f, 0.0f);
			putText(shader, "Total trees: " + std::to_string(pow((TREE_SPREAD * 2), 2)),
					glm::vec3(-1.0f, 5.0f, 0.0f), 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	};

	class Water : public Object
	{
	public:
		Water(const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z) : Object(_shader, _x, _y, _z) {}

		void create() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLint i = -WATER_SPREAD; i < WATER_SPREAD; ++i)
				for (GLint j = -WATER_SPREAD; j < WATER_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef((GLfloat) i * 2.0f, 0.0f, (GLfloat) j * 2.0f);

					glColor3f(0.83f, 0.95f, 0.98f);
					drawCube(1.0f);
					glPopMatrix();
				}

			glColor3f(0.0f, 0.0f, 0.0f);
			putText(shader, "Total water blocks: " + std::to_string(pow((WATER_SPREAD * 2), 2)),
					glm::vec3(-1.0f, 5.0f, 0.0f), 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	};

	class Snow : public Object
	{
	public:
		Snow(const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z) : Object(_shader, _x, _y, _z) {}

		void create() override
		{
			glTranslatef(0.0f, 0.0f, 0.0f);
			glTranslatef(x, y / 10, z);

			for (GLint i = -SNOW_SPREAD; i < SNOW_SPREAD; ++i)
			{
				for (GLint j = -SNOW_SPREAD; j < SNOW_SPREAD; ++j)
				{
					glPushMatrix();
					glTranslatef((GLfloat) i * 2.0f, 0.0f, (GLfloat) j * 2.0f);

					glColor3f(1.0f, 0.98f, 0.98f);
					drawCube(1.0f);
					glPopMatrix();
				}
			}

			glColor3f(0.0f, 0.0f, 0.0f);
			putText(shader, "Total snow blocks: " + std::to_string(pow((SNOW_SPREAD * 2), 2)),
					glm::vec3(-1.0f, 5.0f, 0.0f), 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	};
};

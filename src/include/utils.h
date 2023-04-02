#pragma once

#include <filesystem>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "./shader.h"

struct Character
{
	GLuint textureId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

void putText(Shader &shader, std::string text, glm::vec3 pos, GLfloat scale, glm::vec3 color)
{
	shader.use();
	shader.setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xPos = pos.x + (GLfloat) ch.bearing.x * scale;
		GLfloat yPos = pos.y - (GLfloat) (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = (GLfloat) ch.size.x * scale;
		GLfloat h = (GLfloat) ch.size.y * scale;

		GLfloat vertices[6][4] = {
				{xPos,     yPos + h, 0.0f, 0.0f},
				{xPos,     yPos,     0.0f, 1.0f},
				{xPos + w, yPos,     1.0f, 1.0f},

				{xPos,     yPos + h, 0.0f, 0.0f},
				{xPos + w, yPos,     1.0f, 1.0f},
				{xPos + w, yPos + h, 1.0f, 0.0f}
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureId);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		pos.x += (GLfloat) (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawCube(GLfloat size)
{
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);
	glVertex3f(size, -size, -size);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, -size);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size);
	glVertex3f(size, -size, size);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glEnd();
}

void drawSphere(GLfloat radius, GLint slices, GLint stacks)
{
	GLUquadric* quad = gluNewQuadric();
	gluSphere(quad, radius, slices, stacks);
	gluDeleteQuadric(quad);
}

void drawCone(GLfloat base, GLfloat height, GLint slices, GLint stacks)
{
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, base, 0.0f, height, slices, stacks);
	gluDeleteQuadric(quad);
}

class Object
{
public:
	Object(const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z) : shader(_shader), x(_x), y(_y), z(_z) {}
	Shader shader;

	virtual void create() = 0;
	virtual GLfloat getX() { return x; }
	virtual GLfloat getY() { return y; }
	virtual GLfloat getZ() { return z; }

protected:
	GLfloat x, y, z;
};

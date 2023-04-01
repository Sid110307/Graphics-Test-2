#pragma once

#include <GLFW/glfw3.h>
#include <freetype2/ft2build.h>

struct Character {
	GLuint textureId;
	glm::ivec2   size;
	glm::ivec2   bearing;
	GLuint advance;
};

std::map<char, Character> Characters;

void processText(std::string text)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (auto c : text)
	{
		Character ch = Characters[c];

		GLfloat xpos = x + ch.bearing.x * 0.1f;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * 0.1f;

		GLfloat w = ch.size.x * 0.1f;
		GLfloat h = ch.size.y * 0.1f;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureId);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.advance >> 6) * 0.1f;
	}

glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
FT_Done_Face(face);
FT_Done_FreeType(ft);
}

void putText(Shader &shader, std::string text, float x, float y, float z)
{
	processText(text);
	// TODO: Draw text: https://learnopengl.com/In-Practice/Text-Rendering (Render line of text)
}

void drawCube(float size)
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

void drawSphere(float radius, int slices, int stacks)
{
	GLUquadric* quad = gluNewQuadric();
	gluSphere(quad, radius, slices, stacks);
	gluDeleteQuadric(quad);
}

void drawCone(float base, float height, int slices, int stacks)
{
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, base, 0.0f, height, slices, stacks);
	gluDeleteQuadric(quad);
}

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

#pragma once

#include <GL/freeglut.h>

#include <cmath>
#include <cstdlib>
#include <string>

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	static void processNormalKeys(unsigned char, GLint, GLint);
	static void processSpecialKeys(GLint, GLint, GLint);
	static void processMouse(GLint, GLint, GLint, GLint);
	static void changeSize(GLint, GLint);

	static GLfloat lx;
	static GLfloat ly;
	static GLfloat lz;

	static GLfloat x;
	static GLfloat y;
	static GLfloat z;

	static GLfloat angle;
	static GLfloat aspectRatio;

	static GLfloat fieldOfView;
	static GLfloat nearClip;
	static GLfloat farClip;
};

// TODO: Make camera movement smoother
GLfloat Camera::lx = 0.0f;
GLfloat Camera::ly = 0.0f;
GLfloat Camera::lz = -1.0f;

GLfloat Camera::x = 0.0f;
GLfloat Camera::y = 2.5f;
GLfloat Camera::z = 15.0f;

GLfloat Camera::angle = 0.0f;
GLfloat Camera::aspectRatio = 1.0f;

GLfloat Camera::fieldOfView = 45.0f;
GLfloat Camera::nearClip = 0.1f;
GLfloat Camera::farClip = 1000.0f;

void Camera::processNormalKeys(unsigned char key, GLint, GLint)
{
	if (key == 27) exit(EXIT_SUCCESS);
	if (key == 32)
	{
		if (glutGetModifiers() != GLUT_ACTIVE_SHIFT)
		{
			x = 0.0f;
			y = 2.5f;
			z = 15.0f;
		}

		lx = 0.0f;
		ly = 0.0f;
		lz = -1.0f;

		angle = 0.0;
		glutPostRedisplay();
	}

	GET_ERROR();
}

void Camera::processSpecialKeys(GLint key, GLint, GLint)
{
	GLfloat fraction = 0.1f;

	switch (key)
	{
		case GLUT_KEY_LEFT:
			if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
			{
				angle -= 0.01f;
				lx = std::sin(angle);
				lz = -std::cos(angle);
			}
			else
			{
				x += lz * fraction;
				z -= lx * fraction;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
			{
				angle += 0.01f;
				lx = std::sin(angle);
				lz = -std::cos(angle);
			}
			else
			{
				x -= lz * fraction;
				z += lx * fraction;
			}
			break;
		case GLUT_KEY_UP:
			glutGetModifiers() == GLUT_ACTIVE_SHIFT ? (ly += fraction * 0.1f)
													: (y += 0.1f);
			break;
		case GLUT_KEY_DOWN:
			glutGetModifiers() == GLUT_ACTIVE_SHIFT ? (ly -= fraction * 0.1f)
													: (y -= 0.1f);
			break;
		default:
			break;
	}

	glutPostRedisplay();
	GET_ERROR();
}

void Camera::processMouse(GLint btn, GLint, GLint, GLint)
{
	GLfloat fraction = 0.1f;

	if (btn == 3)
	{
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			x += lx * fraction * 2.0f;
			z += lz * fraction * 2.0f;
		}
		else
		{
			x += lx * fraction;
			z += lz * fraction;
		}
	}
	else if (btn == 4)
	{
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			x -= lx * fraction * 2.0f;
			z -= lz * fraction * 2.0f;
		}
		else
		{
			x -= lx * fraction;
			z -= lz * fraction;
		}
	}

	glutPostRedisplay();
	GET_ERROR();
}

void Camera::changeSize(GLint width, GLint height)
{
	if (height == 0) height = 1;
	aspectRatio = (GLfloat) width / (GLfloat) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fieldOfView, aspectRatio, nearClip,
				   farClip);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GET_ERROR();
}

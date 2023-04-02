#pragma once

#include <cmath>
#include <string>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	static void processKeys(GLFWwindow*, GLint, GLint, GLint, GLint);
	static void processMouse(GLFWwindow*, GLdouble, GLdouble);
	static void changeSize(GLFWwindow*, GLint, GLint);

	static GLfloat lx, ly, lz;
	static GLfloat x, y, z;
	static GLfloat angle;
	static GLfloat aspectRatio;
	static GLfloat fieldOfView;
	static GLfloat nearClip, farClip;
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

void Camera::processKeys(GLFWwindow* window, GLint key, GLint, GLint action, GLint)
{
	GLfloat fraction = 0.1f;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
		{
			x = 0.0f;
			y = 2.5f;
			z = 15.0f;
		}

		lx = 0.0f;
		ly = 0.0f;
		lz = -1.0f;

		angle = 0.0;
	}

	switch (key)
	{
		case GLFW_KEY_LEFT:
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				angle -= 0.01f;
				lx = std::sin(angle);
				lz = -std::cos(angle);
			} else
			{
				x += lz * fraction;
				z -= lx * fraction;
			}
			break;
		case GLFW_KEY_RIGHT:
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				angle += 0.01f;
				lx = std::sin(angle);
				lz = -std::cos(angle);
			} else
			{
				x -= lz * fraction;
				z += lx * fraction;
			}
			break;
		case GLFW_KEY_UP:
			glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (ly += fraction * 0.1f) : (y += 0.1f);
			break;
		case GLFW_KEY_DOWN:
			glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (ly -= fraction * 0.1f) : (y -= 0.1f);
			break;
		default:
			break;
	}

//	glfwPostRedisplay(window);
}

void Camera::processMouse(GLFWwindow* window, GLdouble _x, GLdouble _y)
{
	GLfloat fraction = 0.1f;
	static GLdouble lastX = _x;
	static GLdouble lastY = _y;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			angle += (GLfloat) (_x - lastX) * fraction;
			lx = std::sin(angle);
			lz = -std::cos(angle);
		} else
		{
			x += (GLfloat) (_x - lastX) * fraction;
			z += (GLfloat) (_y - lastY) * fraction;
		}
	}

	lastX = _x;
	lastY = _y;

//	glfwPostRedisplay(window);
}

void Camera::changeSize(GLFWwindow*, GLint w, GLint h)
{
	if (h == 0) h = 1;
	aspectRatio = (GLfloat) w / (GLfloat) h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

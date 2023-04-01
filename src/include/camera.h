/*
#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <string>

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	static void processKeys(GLFWwindow*, int, int, int, int);
	static void processMouse(GLFWwindow*, double, double);
	static void changeSize(GLFWwindow*, int, int);

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

void Camera::processKeys(GLFWwindow* window, int key, int, int action, int)
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
		glfwPostRedisplay(window);
	}

	switch (key)
	{
	case GLFW_KEY_LEFT:
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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
	case GLFW_KEY_RIGHT:
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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
	case GLFW_KEY_UP:
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (ly += fraction * 0.1f) : (y += 0.1f);
		break;
	case GLFW_KEY_DOWN:
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (ly -= fraction * 0.1f) : (y -= 0.1f);
		break;
	default:
		break;
	}

	glfwPostRedisplay(window);
}

void Camera::processMouse(GLFWwindow* window, double x, double y)
{
	static double lastX = x;
	static double lastY = y;

	GLfloat fraction = 0.1f;

	if (x > lastX)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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
	}
	else if (x < lastX)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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
	}

	if (y > lastY) glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (ly += fraction * 0.1f) : (y += 0.1f);
	else if (y < lastY) glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? (ly -= fraction * 0.1f) : (y -= 0.1f);

	lastX = x;
	lastY = y;

	glfwPostRedisplay(window);
}

void Camera::changeSize(GLFWwindow*, int w, int h)
{
	if (h == 0) h = 1;
	aspectRatio = (GLfloat)w / (GLfloat)h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
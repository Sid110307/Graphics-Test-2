#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-flp30-c"

#include <GL/freeglut.h>

#include <cmath>
#include <cstdlib>
#include <string>

#include "./include/stringUtils.h"

#define SNOWMAN_SPREAD 3

float angle = 0.0;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float x = 0.0f, y = 2.5f, z = 15.0f;

void showGrid()
{
	glBegin(GL_LINES);
	for (GLfloat i = -25; i <= 25; i += 0.25)
	{
		glVertex3f(i, -1, 25);
		glVertex3f(i, -1, -25);
		glVertex3f(25, -1, i);
		glVertex3f(-25, -1, i);
	}

	glEnd();
}

void showAxes()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();
}

void showHelp()
{
	float xPos = -1.0f, yPos = 0.0f, zPos = 5.0f;
	glColor3f(0, 0, 0);

	glRasterPos3f(xPos, yPos, zPos);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18,
		(const unsigned char*)"Arrow keys - pan (hold shift to rotate)");
	glRasterPos3f(xPos, yPos -= 0.25f, zPos);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18,
		(const unsigned char*)"Mouse wheel - move (hold shift to move faster)");
	glRasterPos3f(xPos, yPos - 0.25f, zPos);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18,
		(const unsigned char*)"Space - reset");
}

void drawSnowman()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	glTranslatef(0.0f, 1.0f, 0.0f);
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);

	showGrid();
	showAxes();
	showHelp();

	for (GLfloat i = -SNOWMAN_SPREAD; i < SNOWMAN_SPREAD; i++)
		for (GLfloat j = -SNOWMAN_SPREAD; j < SNOWMAN_SPREAD; j++)
		{
			glPushMatrix();
			glTranslatef(i * 2.0f, 0.0f, j * 2.0f);
			drawSnowman();
			glPopMatrix();
		}

	std::string txt = "Total snowmen: ";
	txt += std::to_string((int)(SNOWMAN_SPREAD * 2 * (SNOWMAN_SPREAD * 2)));

	glColor3f(0, 0, 0);

	glRasterPos3f(-1.0f, 5.0f, 5.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18,
		(const unsigned char*)trim(txt));

	glutSwapBuffers();
	glFlush();
}

void initSkybox()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.529, 0.808, 0.922, 0.0);
	glColor3f(1.0, 1.0, 1.0);
}

void changeSize(int w, int h)
{
	if (h == 0) h = 1;
	double ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);

	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void processNormalKeys(unsigned char key, __attribute__((unused)) int _x,
	__attribute__((unused)) int _y)
{
	if (key == 27) exit(EXIT_SUCCESS);
	if (key == 32)
	{
		x = 0.0f;
		y = 2.5f;
		z = 15.0f;

		lx = 0.0f;
		ly = 0.0f;
		lz = -1.0f;

		angle = 0.0;
		glutPostRedisplay();
	}
}

void processSpecialKeys(int key, __attribute__((unused)) int _x,
	__attribute__((unused)) int _y)
{
	float fraction = 0.1f;

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
}

void processMouse(int btn, __attribute__((unused)) int state,
	__attribute__((unused)) int _x,
	__attribute__((unused)) int _y)
{
	float fraction = 0.1f;

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
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Graphics Test 2");

	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutIdleFunc(display);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouse);

	initSkybox();

	glutMainLoop();
	return EXIT_FAILURE;
}

#pragma clang diagnostic pop

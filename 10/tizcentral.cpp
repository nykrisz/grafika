#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

GLsizei winWidth = 600, winHeight = 600;

GLfloat center = 10.0f, deltaCenter = 0.5f;
GLfloat delta_u = 0.1, delta_theta = pi() / 10;
GLfloat alphaZ = 0.0f, alphaX = 10.0f, alphaY = 30.0f;

mat4 M, T, rotation, w2v;
mat4 projection = perspective(center);

GLint keyStates[256];
GLint n = 5;

vec2 tmp[6] = { vec2(0,0), vec2(3,1), vec2(3,4), vec2(1,4), vec2(1,6), vec2(3,6.5) };

struct Face {
	vec3 p[4];
	vec3 middlePoint;
	float distance;

	void drawFace() {
		glLineWidth(2.0f);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 4; i++) {
			vec4 hPoint = ihToH(p[i]);
			hPoint = M * hPoint;
			vec3 ihDrawablePoint = hToIh(hPoint);
			glVertex2d(ihDrawablePoint.x, ihDrawablePoint.y);
		}
		glEnd();
	}

	void visibility() {
		middlePoint = (hToIh(T * ihToH(p[0])) + hToIh(T * ihToH(p[1])) + hToIh(T * ihToH(p[2])) + hToIh(T * ihToH(p[3]))) / 4.0;
		distance = dist(vec3(0.0f, 0.0f, center), middlePoint);
	}

};

vector<Face> faces;

int factorial(int n) {
	int r = 1;
	for (int i = n; i > 0; i--) {
		r *= i;
	}
	return r;
}

vec2 bernstein(float u) {
	vec2 P;
	for (GLint i = 0; i < n + 1; i++) {

		GLfloat bi = factorial(n) / (factorial(i) * factorial(n - i));
		bi *= pow(u, i);
		bi *= pow(1 - u, n - i);

		P += {tmp[i].x * bi, tmp[i].y * bi};
	}
	return P;
}

void initTransformations() {
	rotation = rotateX(degToRad(alphaX)) * rotateY(degToRad(alphaY));
	w2v = windowToViewport3(vec2(0,0), vec2(2 * pi(), 2 * pi()), vec2(250.0f, 70.0f), vec2(250.0f, 250.0f));

	M = w2v * projection * rotation;

	T = rotation;
}

void setPoints() {
	
	for (float u = 0; u < 1; u += delta_u) {

		vec2 point = bernstein(u);
		vec2 nextP = bernstein(u + delta_u);

		for (float theta = 0; theta <= 2*pi(); theta += delta_theta) {
			faces.resize(faces.size() + 1);
			faces.at(faces.size() - 1).p[0] = vec3(cos(theta) * point.x, point.y, -sin(theta) * point.x);
			faces.at(faces.size() - 1).p[1] = vec3(cos(theta + delta_theta) * point.x, point.y, -sin(theta + delta_theta) * point.x);
			faces.at(faces.size() - 1).p[2] = vec3(cos(theta + delta_theta) * nextP.x, nextP.y, -sin(theta + delta_theta) * nextP.x);
			faces.at(faces.size() - 1).p[3] = vec3(cos(theta) * nextP.x, nextP.y, -sin(theta) * nextP.x);

			
			/* átláthatóság kedvéért
			float p0x = cos(theta) * point.x;
			float p0y = point.y;
			float p0z = -sin(theta) * point.x;

			float p1x = cos(theta + delta_theta) * point.x;
			float p1y = point.y;
			float p1z = -sin(theta + delta_theta) * point.x;

			float p2x = cos(theta + delta_theta) * nextP.x;
			float p2y = nextP.y;
			float p2z = -sin(theta + delta_theta) * nextP.x;

			float p3x = cos(theta) * nextP.x;
			float p3y = nextP.y;
			float p3z = -sin(theta) * nextP.x;
			
			faces.resize(faces.size() + 1);
			faces.at(faces.size() - 1).p[0] = vec3(p0x, p0y, p0z);
			faces.at(faces.size() - 1).p[1] = vec3(p1x, p1y, p1z);
			faces.at(faces.size() - 1).p[2] = vec3(p2x, p2y, p2z);
			faces.at(faces.size() - 1).p[3] = vec3(p3x, p3y, p3z);
*/
		}
	}
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glLineWidth(1.0);
	initTransformations();
	setPoints();
}

bool sortFaces(Face f1, Face f2) {
	return f1.distance > f2.distance;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (unsigned int i = 0; i < faces.size(); i++) {
		faces.at(i).visibility();
	}

	sort(faces.begin(), faces.end(), sortFaces);

	for (unsigned int i = 0; i < faces.size(); i++) {
		faces.at(i).drawFace();
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		for (unsigned int j = 0; j < 4; j++) {
			vec4 tmp = ihToH(faces.at(i).p[j]);
			tmp = M * tmp;
			vec3 tmp2 = hToIh(tmp);
			glVertex2d(tmp2.x, tmp2.y);
		}
		glEnd();
	}

	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y)
{
	keyStates[key] = 0;
}

void keyOperations(int value)
{
	if (keyStates['a']) { alphaY += 1; }
	if (keyStates['d']) { alphaY -= 1; }

	if (keyStates['s']) { alphaX += 1; }
	if (keyStates['w']) { alphaX -= 1; }

	faces.resize(0);
	initTransformations();
	setPoints();

	glutTimerFunc(1, keyOperations, 0);

	glutPostRedisplay();

}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("kilenc");
	init();
	glutDisplayFunc(display);

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(1, keyOperations, 0);

	glutMainLoop();
	return 0;
}
#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>

using namespace std;

GLsizei winWidth = 400, winHeight = 400;

int N = 8;
vec3 cube[8] = { { -1,-1,-1 },{ 1,-1,-1 },{ 1,1,-1 },{ -1,1,-1 },{ -1,-1,1 },{ 1,-1,1 },{ 1,1,1 },{ -1,1,1 } };
vec2 drawableCube[8] = {};

GLint keyStates[256];

mat4 w2v, projection;
float alpha = 0;
float beta = 0;
float c = 3;

void initMatrices() {
	vec2 windowSize = { 4, 4 };
	vec2 windowPosition = { -2, -2 };
	vec2 viewportSize = { 200 , 200 };
	vec2 viewportPosition = { 100, 100 };
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
}

void transform(mat4 proj) {
	mat4 rotation = rotateX(degToRad(alpha))* rotateY(degToRad(beta));
	
	mat4 M = w2v * proj * rotation;
	
	for (int i = 0; i < N; i++) {

		vec4 pointH = ihToH(cube[i]);
		vec4 transformedPoint = M * pointH;

		if (transformedPoint.w != 0)
		{
			vec3 result = hToIh(transformedPoint);
			if (result.z == 0) {
				drawableCube[i] = { result.x, result.y };
			}
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

	initMatrices();
}

void drawFaces(GLint n1, GLint n2, GLint n3, GLint n4 ) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(drawableCube[n1].x, drawableCube[n1].y);
		glVertex2f(drawableCube[n2].x, drawableCube[n2].y);
		glVertex2f(drawableCube[n3].x, drawableCube[n3].y);
		glVertex2f(drawableCube[n4].x, drawableCube[n4].y);
	glEnd();
}

void drawCube() {
	drawFaces(0, 1, 2, 3);
	drawFaces(1, 5, 6, 2);
	drawFaces(5, 4, 7, 6);
	drawFaces(4, 0, 3, 7);
	drawFaces(4, 5, 1, 0);
	drawFaces(3, 2, 6, 7);
}

void leftSide() {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(0,0);
		glVertex2f(0,winHeight);
		glVertex2f(winWidth / 2, winHeight);
		glVertex2f(winWidth / 2, 0);
	glEnd();
	transform(perspective(c));
}

void rightSide() {
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex2f(winWidth / 2, 0);
		glVertex2f(winWidth / 2, winHeight);
		glVertex2f(winWidth, winHeight);
		glVertex2f(winWidth, 0);
	glEnd();
	//transform(ortho());
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	leftSide();
	rightSide();
	drawCube();

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
	if (keyStates['a']) { beta -= 0.5; }
	if (keyStates['d']) { beta += 0.5; }

	if (keyStates['s']) { alpha -= 0.5; }
	if (keyStates['w']) { alpha += 0.5; }

	if (keyStates['+']) { c += 0.01; }
	if (keyStates['-']) { c -= 0.01; }

	glutPostRedisplay();

	glutTimerFunc(5, keyOperations, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("nyolc");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(5, keyOperations, 0);
	glutMainLoop();
	return 0;
}

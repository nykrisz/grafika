#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

GLsizei winWidth = 400, winHeight = 400;

int N = 8;
vec3 cube[8] = { { -1,-1,-1 },{ 1,-1,-1 },{ 1,1,-1 },{ -1,1,-1 },{ -1,-1,1 },{ 1,-1,1 },{ 1,1,1 },{ -1,1,1 } };
vec2 drawableCube[8] = {};

GLint keyStates[256];
GLint dragged = -1;

mat4 w2v, projection;
float alpha = 0;
float beta = 0;
float c = 3;

vec2 viewportPosition = { 100, 100 };
vec2 windowSize = { 4, 4 };
vec2 windowPosition = { -2, -2 };
vec2 viewportSize = { 200 , 200 };

vec2 clickXY = {}, draggedXY = {}, eltolasvektor = { 0,0 }, elozoEltolasVektor;
mat3 eltolasMatrix;

vec2 felezoA = {float(winWidth / 2), float(0)};
vec2 felezoF = { float(winWidth / 2), float(winHeight) };

vec2 metszesPont;

void transform(mat4 proj) {
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
	mat4 rotation = rotateX(degToRad(alpha))* rotateY(degToRad(beta));

	eltolasMatrix = translate(eltolasvektor);

	mat4 M = w2v * proj * rotation;

	for (int i = 0; i < N; i++) {

		vec4 pointH = ihToH(cube[i]);
		vec4 transformedPoint = M * pointH;

		if (transformedPoint.w != 0)
		{
			vec3 result = hToIh(transformedPoint);
			if (result.z == 0) {
				drawableCube[i] = { result.x, result.y };
				vec3 homogenCube = ihToH(drawableCube[i]);
				homogenCube = eltolasMatrix * homogenCube;
				drawableCube[i] = hToIh(homogenCube);
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
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
}

//void drawFaces(GLint n1, GLint n2, GLint n3, GLint n4) {
//	glColor3f(0.0, 0.0, 0.0);
//	glBegin(GL_LINE_LOOP);
//	glVertex2f(drawableCube[n1].x, drawableCube[n1].y);
//	glVertex2f(drawableCube[n2].x, drawableCube[n2].y);
//	glVertex2f(drawableCube[n3].x, drawableCube[n3].y);
//	glVertex2f(drawableCube[n4].x, drawableCube[n4].y);
//	glEnd();
//}
//
//void drawCube() {
//	drawFaces(0, 1, 2, 3);
//	drawFaces(1, 5, 6, 2);
//	drawFaces(5, 4, 7, 6);
//	drawFaces(4, 0, 3, 7);
//	drawFaces(4, 5, 1, 0);
//	drawFaces(3, 2, 6, 7);
//}

void leftSide() {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(0, winHeight);
	glVertex2f(winWidth / 2, winHeight);
	glVertex2f(winWidth / 2, 0);
	glEnd();
}

void rightSide() {
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(winWidth / 2, 0);
	glVertex2f(winWidth / 2, winHeight);
	glVertex2f(winWidth, winHeight);
	glVertex2f(winWidth, 0);
	glEnd();
}

double pointWhichSide(vec2 vizsgalt) {
	vec2 v = felezoF - felezoA;
	vec2 n = { -v.y, v.x };
	double a = n.x;
	double b = n.y;
	double c = -a * felezoF.x - b * felezoF.y;

	return a * vizsgalt.x + b * vizsgalt.y + c;
}


vec2 calcMetszesPont(vec2 p1, vec2 p2) {
	vec3 felezoAlso = ihToH(felezoA);
	vec3 felezoFelso = ihToH(felezoF);
	vec3 felezoVektorialis = cross(felezoAlso, felezoFelso);

	vec3 egyik = ihToH(p1);
	vec3 masik = ihToH(p2);

	vec3 egyenesFelezoVektorialis = cross(egyik, masik);

	vec3 metsz = cross(felezoVektorialis, egyenesFelezoVektorialis);

	return metszesPont = hToIh(metsz);
}

void elek(GLint n1, GLint n2) {
	
	glLineWidth(2.0);
	
	glBegin(GL_LINES);
	transform(ortho());

		if ((pointWhichSide(drawableCube[n1]) > 0) && pointWhichSide(drawableCube[n2]) < 0) {

			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(drawableCube[n1].x, drawableCube[n1].y);
			glVertex2f(calcMetszesPont(drawableCube[n1], drawableCube[n2]).x, calcMetszesPont(drawableCube[n1], drawableCube[n2]).y);

		}
		else if((pointWhichSide(drawableCube[n1]) > 0) && pointWhichSide(drawableCube[n2]) > 0){

			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(drawableCube[n1].x, drawableCube[n1].y);
			glVertex2f(drawableCube[n2].x, drawableCube[n2].y);
		}
	glEnd();

	////bal
	glBegin(GL_LINES);
	transform(perspective(c));

	if ((pointWhichSide(drawableCube[n1]) < 0) && pointWhichSide(drawableCube[n2]) > 0) {

		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(drawableCube[n1].x, drawableCube[n1].y);
		glVertex2f(calcMetszesPont(drawableCube[n1], drawableCube[n2]).x, calcMetszesPont(drawableCube[n1], drawableCube[n2]).y);

	}
	else if ((pointWhichSide(drawableCube[n1]) < 0) && pointWhichSide(drawableCube[n2]) < 0) {

		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(drawableCube[n1].x, drawableCube[n1].y);
		glVertex2f(drawableCube[n2].x, drawableCube[n2].y);
	}
	glEnd();

}

void drawcumo() {
	elek(0, 4);
	elek(0, 1);
	elek(0, 3);

	elek(1, 0);
	elek(1, 2);
	elek(1, 5);

	elek(2, 3);
	elek(2, 6);
	elek(2, 1);

	elek(3, 0);
	elek(3, 2);
	elek(3, 7);

	elek(4, 0);
	elek(4, 5);
	elek(4, 7);

	elek(5, 1);
	elek(5, 4);
	elek(5, 6);

	elek(6, 2);
	elek(6, 5);
	elek(6, 7);

	elek(7, 3);
	elek(7, 4);
	elek(7, 6);

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	leftSide();
	rightSide();
	//drawCube();
	drawcumo();

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

	if (keyStates['p']) {
		
	} 

	glutPostRedisplay();

	glutTimerFunc(5, keyOperations, 0);
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		clickXY = { float(xMouse), float(winHeight - yMouse) };
		dragged = 1;
	}
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
		elozoEltolasVektor = eltolasvektor;
		dragged = -1;
	}
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) {
		draggedXY = { float(xMouse), float(winHeight - yMouse) };

		eltolasvektor = (draggedXY - clickXY) + elozoEltolasVektor;

		glutPostRedisplay();
	}
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

	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);

	glutMainLoop();
	return 0;
}

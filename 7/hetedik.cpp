#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <math.h>
#include <iostream>

using namespace std;

GLfloat winWidth = 640, winHeight = 640;

vec2 viewportSize = { 300, 300 };

mat3 wTv;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glLineWidth(1.2f);
}

void coordSystem(vec2 viewportPos) {
	vec2 coordSystemPoints[8] = { { 0, winHeight / 2 },{ winWidth, winHeight / 2 },{ winWidth / 2, 0 },{ winWidth / 2, winHeight }
									,{ 0, 0 },{ winWidth, 0 },{ winWidth, winHeight },{ 0, winHeight } };
	vec2 drawPoints[8] = {};

	mat3 wTv2 = windowToViewport2(vec2(0, 0), vec2(winWidth, winHeight), viewportPos, viewportSize);

	for (int i = 0; i < 8; i++) {
		vec3 pointH = ihToH(coordSystemPoints[i]);
		vec3 rotatedPointH = wTv2 * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			drawPoints[i] = { result.x, result.y };
		}
	}

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2f(drawPoints[0].x, drawPoints[0].y);
	glVertex2f(drawPoints[1].x, drawPoints[1].y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(drawPoints[2].x, drawPoints[2].y);
	glVertex2f(drawPoints[3].x, drawPoints[3].y);
	glEnd();

	glBegin(GL_LINE_LOOP);
	for (int i = 4; i < 8; i++)
		glVertex2f(drawPoints[i].x, drawPoints[i].y);
	glEnd();
}


void firstFunction() {
	coordSystem(vec2(10, winHeight / 2 + 10 ));
	//
	wTv = windowToViewport2(vec2(-1,-1), vec2(2,2), vec2(10, winHeight / 2 + 10), viewportSize);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (float i = -1; i < 1; i += 0.01) {
		vec3 pointH = ihToH(vec2(i,i*i*i));
		vec3 rotatedPointH = wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			glVertex2f(result.x, result.y);
		}
	}
	glEnd();
}

void secondFunction() {
	coordSystem(vec2(winWidth / 2 + 10, winHeight / 2 + 10));
	//
	wTv = windowToViewport2(vec2(-5,-5), vec2(10,10), vec2(winWidth / 2 + 10, winHeight / 2 + 10), viewportSize);
	glColor3f(0.75, 0.75, 0.0);
	glBegin(GL_LINE_STRIP);
	for (GLfloat i = -5; i < -0.19; i += 0.01) {
		
		vec3 pointH = ihToH(vec2(i, 1 / i));
		vec3 rotatedPointH = wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			glVertex2f(result.x, result.y);
		}
		
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (GLfloat i = 0.2; i < 5; i += 0.01) {
		vec3 pointH = ihToH(vec2(i, 1 / i));
		vec3 rotatedPointH = wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			glVertex2f(result.x, result.y);
		}
	}
	glEnd();
}

void thirdFunction() {
	coordSystem(vec2(10, 10));
	//
	wTv = windowToViewport2(vec2(-2 * pi(), -2 * pi()), vec2(4 * pi(), 4 * pi()), vec2(10, 10), viewportSize);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (float i = -2 * pi(); i < 2 * pi(); i += 0.01) {
		vec3 pointH = ihToH(vec2(i, sin(i)));
		vec3 rotatedPointH = wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			glVertex2f(result.x, result.y);
		}
	}
	glEnd();

	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (float i = -2 * pi(); i < 2 * pi(); i += 0.01) {
		vec3 pointH = ihToH(vec2(i, cos(i)));
		vec3 rotatedPointH = wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			glVertex2f(result.x, result.y);
		}
	}
	glEnd();
}

void fourthFunction() {
	/*
		wTv = windowToViewport2(windowPosition, windowSize, viewportPosition, viewportSize);
	
		vec2 points[4] = { {100, 100}, {-100, 100}, {-100, -100}, {100, -100} };

		vec2 windowPosition = { -100, -100 };
		vec2 windowSize = {200, 200};

		windowPosition: bal alsó sarok
		windowSize: két sarok távolsága 
	*/
	
	coordSystem(vec2(winWidth / 2 + 10, 10));
	//
	wTv = windowToViewport2(vec2(-1.5*pi() + -2.5 * pi(), -1.5*pi()+ -2.5 * pi()), vec2(2 * (1.5 * pi() + 2.5 * pi()), 2 * (1.5 * pi() + 2.5 * pi())), vec2(winWidth / 2 + 10, 10), viewportSize);
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (float i = -1.5 * pi(); i < 2.5 * pi(); i += 0.01) {
		vec3 pointH = ihToH(vec2(i - 1.6 * cos(24*i), i - 1.6 * sin(25*i)));
		vec3 rotatedPointH = wTv * pointH;
		if (rotatedPointH.z != 0) {
			vec2 result = hToIh(rotatedPointH);
			glVertex2f(result.x, result.y);
		}
	}
	glEnd();

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	firstFunction();
	secondFunction();
	thirdFunction();
	fourthFunction();
	glutSwapBuffers();
}

void update(int v) {

	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hetedik");

	init();
	glutDisplayFunc(display);
	glutTimerFunc(10, update, 0);

	glutMainLoop();
	return 0;
}

#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

GLsizei winWidth = 600, winHeight = 600;

mat4 projection = ortho();

GLfloat u_plus = pi() / 8, v_plus = pi() / 10;
GLfloat alphaZ = 0.0f, alphaX = 0.0f, alphaY = 0.0f;
mat4 M, T, rotation, w2v;

struct Face {
	vec3 p[4];
	vec3 middlePoint;
	bool isVisible = true;

	void drawFace() {
		glPointSize(3.0f);
		glLineWidth(2.0f);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);

		for (int i = 0; i < 4; i++) {
			vec4 hToruPoint = ihToH(p[i]);
			hToruPoint = M * hToruPoint;
			vec3 ihDrawableToruPoint = hToIh(hToruPoint);
			glVertex2d(ihDrawableToruPoint.x, ihDrawableToruPoint.y);
		}
		glEnd();
	}

	void setWeight() {
		middlePoint = (hToIh(T * ihToH(p[0])) + hToIh(T * ihToH(p[1])) + hToIh(T * ihToH(p[2])) + hToIh(T * ihToH(p[3]))) / 4.0;
	}

};

vector<Face> faces;

void initTransformations() {

	rotation = rotateX(degToRad(alphaX)) * rotateY(degToRad(alphaY));
	w2v = windowToViewport3(vec2(-2 * pi(), -2 * pi()), vec2(4 * pi(), 4 * pi()), vec2(100.0f, 100.0f), vec2(300.0f, 300.0f));

	M = w2v * projection * rotation;

	T = rotation;
}

void setPoints() {
	for (float u = 0.0f; u < 5; u += u_plus) {
		for (float theta = -2 * pi(); theta < 2 * pi() + 0.01; theta += v_plus) {

			faces.resize(faces.size() + 1);
			faces.at(faces.size() - 1).p[0] = (vec3(u * cos(1 * theta), u * sin(1 * theta), theta));
			faces.at(faces.size() - 1).p[1] = (vec3((u + u_plus) * cos(1 * theta), (u + u_plus) * sin(1 * theta), theta));
			faces.at(faces.size() - 1).p[2] = (vec3((u + u_plus) * cos(1 * theta + v_plus), (u + u_plus) * sin(1 * theta + v_plus), theta + v_plus));
			faces.at(faces.size() - 1).p[3] = (vec3(u * cos(1 * theta + v_plus), u * sin(1 * theta + v_plus), theta + v_plus));
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

bool sortedByOrtho(Face f1, Face f2) {
	return f1.middlePoint.z < f2.middlePoint.z;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (unsigned int i = 0; i < faces.size(); i++) {
		faces.at(i).setWeight();
	}

	std::sort(faces.begin(), faces.end(), sortedByOrtho);

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

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:      exit(0);     break;
	case 's':     alphaX += 1; break;
	case 'w':     alphaX -= 1; break;
	case 'a':     alphaY += 1; break;
	case 'd':     alphaY -= 1; break;
	}
	faces.resize(0);
	initTransformations();
	setPoints();
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
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

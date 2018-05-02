#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

GLsizei winWidth = 600, winHeight = 600;

mat4 projection = ortho();

GLfloat delta_u = 0.3, delta_theta = 0.2;
GLfloat alphaZ = 0.0f, alphaX = 10.0f, alphaY = 30.0f;
mat4 M, T, rotation, w2v;

GLfloat u_param = 0.0f;
GLfloat theta_param = 0.0f;
GLfloat alpha = 0.5f;

GLint keyStates[256];
GLint dragged = -1;

struct Face {
	vec3 p[4];
	vec3 middlePoint;
	
	void drawFace() {
		glLineWidth(2.0f);
		glColor3f(1.0, 0.0, 0.0);
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
	}

};

vector<Face> faces;

void initTransformations() {

	rotation = rotateX(degToRad(alphaX)) * rotateY(degToRad(alphaY));
	w2v = windowToViewport3(vec2(-2 * pi(), -2 * pi()), vec2(4 * pi(), 4 * pi()), vec2(100.0f, 100.0f), vec2(400.0f, 400.0f));

	M = w2v * projection * rotation;

	T = rotation;
}

void setPoints() {
	for (float u = u_param; u < u_param + 5; u += delta_u) {
		for (float theta = -2*pi() - theta_param; theta < 2*pi() + theta_param; theta += delta_theta) {
			
			float f0x = u * cos(alpha * theta);
			float f0y = u * sin(alpha * theta);
			float f0z = theta;

			float f1x = (u + delta_u) * cos(alpha * theta);
			float f1y = (u + delta_u) * sin(alpha * theta);
			float f1z = theta;
			
			float f2x = (u + delta_u) * cos(alpha * (theta + delta_theta));
			float f2y = (u + delta_u) * sin(alpha * (theta + delta_theta));
			float f2z = theta + delta_theta;

			float f3x = u * cos(alpha * (theta + delta_theta));
			float f3y = u * sin(alpha * (theta + delta_theta));
			float f3z = theta + delta_theta;

			faces.resize(faces.size() + 1);
			faces.at(faces.size() - 1).p[0] = vec3(f0x, f0y, f0z);
			faces.at(faces.size() - 1).p[1] = vec3(f1x, f1y, f1z);
			faces.at(faces.size() - 1).p[2] = vec3(f2x, f2y, f2z);
			faces.at(faces.size() - 1).p[3] = vec3(f3x, f3y, f3z);

			/*faces.at(faces.size() - 1).p[0] = vec3(u * cos(alpha * theta), u * sin(alpha * theta), theta);
			faces.at(faces.size() - 1).p[1] = vec3((u + delta_u) * cos(alpha * theta), (u + delta_u) * sin(alpha * theta), theta);
			faces.at(faces.size() - 1).p[2] = vec3((u + delta_u) * cos(alpha * (theta + delta_theta)), (u + delta_u) * sin(alpha * (theta + delta_theta)), theta + delta_theta);
			faces.at(faces.size() - 1).p[3] = vec3(u * cos(alpha * (theta + delta_theta)), u * sin(alpha * (theta + delta_theta)), theta + delta_theta);*/
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
		faces.at(i).visibility();
	}

	sort(faces.begin(), faces.end(), sortedByOrtho);

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

	if (keyStates['+']) { 
		if (alpha < 5) {
			alpha += 0.01;
		}
	}
	if (keyStates['-']) { 
		if (alpha > -5) {
			alpha -= 0.01;
		}
	}

	if (keyStates['u']) {
		if (u_param < 2.5) {
			u_param += 0.1;
		}
	}
	if (keyStates['i']) {
		if (u_param > -2.5) {
			u_param -= 0.1;
		}
	}
	
	if (keyStates['t']) {
		if (theta_param < 5) {
			theta_param += 0.1;
		}
	}
	if (keyStates['z']) {
		if (theta_param > -5) {
			theta_param -= 0.1;
		}
	}

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
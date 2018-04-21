#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

GLsizei winWidth = 600, winHeight = 600;

GLint keyStates[256];
GLint dragged = -1;

mat4 w2v;
float alpha = 0;
float beta = 0;
float delta = 10;
float delta2 = 3;
//alfa
float a = 1;

mat4 projection = ortho();

vec2 viewportPosition = { 100, 100 };
vec2 windowSize = { 4 * pi(), 4 * pi() };
vec2 windowPosition = { -2 * pi(), -2 * pi() };
vec2 viewportSize = { 400 , 400 };

struct Face {
	vec3 p[4];
};

Face boltface = {};

vector<Face> faceVector;

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

void transform() {
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
	
	mat4 rotation = rotateX(degToRad(alpha))* rotateY(degToRad(beta));
	mat4 M = w2v * projection * rotation;

	glColor3f(0.0, 0.0, 0.0);
	
	for (float i = 0; i < 5; i += 0.1) {
		for (float j = -2 * pi(); j < 2 * pi(); j += 0.05) {
			vec4 pointH = ihToH(vec3( i * cos(a * j), i * sin(a * j), j ));
			vec4 transformedPoint = M * pointH;

			if (transformedPoint.w != 0)
			{
				vec3 result = hToIh(transformedPoint);
				if (result.z == 0) {
					boltface.p[0] =  result ;
					boltface.p[1] = { result.x + delta, result.y };
					boltface.p[2] = { result.x + delta, result.y + delta2 };
					boltface.p[3] = { result.x, result.y + delta2 };
					//glVertex2f(result.x, result.y);
					faceVector.push_back(boltface);
				}
			}

		}
	}
	
}


void asd() {

	for (int i = 0; i < faceVector.size(); i++) {
		glBegin(GL_LINE_LOOP);

		glVertex2f(faceVector[i].p[0].x, faceVector[i].p[0].y);
		glVertex2f(faceVector[i].p[1].x, faceVector[i].p[1].y);
		glVertex2f(faceVector[i].p[2].x, faceVector[i].p[2].y);
		glVertex2f(faceVector[i].p[3].x, faceVector[i].p[3].y);

		glEnd();
	}

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	transform();
	asd();
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
	if (keyStates['a']) { beta -= 0.5; faceVector.clear(); }
	if (keyStates['d']) { beta += 0.5; faceVector.clear(); }

	if (keyStates['s']) { alpha -= 0.5; faceVector.clear(); }
	if (keyStates['w']) { alpha += 0.5; faceVector.clear(); }

	if (keyStates['c']) { asd(); }


	glutPostRedisplay();

	glutTimerFunc(5, keyOperations, 0);
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
	glutTimerFunc(5, keyOperations, 0);

	glutMainLoop();
	return 0;
}

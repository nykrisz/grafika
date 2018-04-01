#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;
GLint dragged = -1;
GLint draw = 0;
GLfloat radius = 15;
GLint outxMouse, outyMouse;

vec2 s = { 1, 1 };
float t = 0;
vec2 eltol = { 0,0 };
int sizeTenTimes = 1;
int counter = 0;
vector<vec3> todraw;
vector<vector<vec3>> finalform;
vector<vec3> rgb;

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	srand(time(0));
	glPointSize(7.0);
	glLineWidth(1.4f);
}

void calc(vec2 O, GLfloat r) {
	vec3 firstSquareFirstPoint = { O.x + r * cos(pi() / 4), O.y + r * sin(pi() / 4), 0 };
	vec3 firstSquareSecondPoint = { O.x + r * cos(pi() / 4 + 2 * pi() / 4), O.y + r * sin(pi() / 4 + 2 * pi() / 4), 0 };

	todraw.clear();
	if (sizeTenTimes == 0) {

		//origoba tol
		mat3 transToO = translate(-1 * O + eltol);
		//forgatas
		mat3 rotated = rotate(t);
		//skalazas
		mat3 scaled = scale(s);
		//visszatol
		mat3 transToC = translate(O + eltol);
		//transzformacios matrix
		mat3 M = transToC * rotated * scaled * transToO;

		for (GLfloat t = pi() / 4; t <= 2 * pi() + pi() / 4; t += 2 * pi() / 4) {
			vec2 temp = { O.x + r * cos(t), O.y + r * sin(t) };
			vec3 pointH = ihToH(temp);
			vec3 rotatedPointH = M * pointH;
			if (rotatedPointH.z != 0) {
				vec2 result = hToIh(rotatedPointH);
				vec3 rs = { result.x, result.y, float(counter) };
				todraw.push_back(rs);
			}
		}
		if (dist(firstSquareFirstPoint, firstSquareSecondPoint) * 10 > dist(todraw[0], todraw[1])) {
			finalform.push_back(todraw);
			t += 0.25;
			s += 0.25;
			eltol += 0.01;

			rgb.push_back(vec3((float) (rand()) / (float) (RAND_MAX), (float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX)));
		}
		else {
			sizeTenTimes = 1;
		}

		sort(finalform.begin() + counter, finalform.end(), [](vector<vec3> a, vector<vec3> b) {
			return (dist(a.at(0), a.at(1)) > dist(b.at(0), b.at(1)));
		});
	}
}

void drawSquare() {
	for (int i = counter; i < finalform.size(); i++) {

		glBegin(GL_POLYGON);	
		for (int j = 0; j < finalform[i].size(); j++) {
			glColor3f(rgb.at(finalform[i][j].z).x, rgb.at(finalform[i][j].z).y, rgb.at(finalform[i][j].z).z);
			glVertex2f(finalform[i][j].x, finalform[i][j].y);
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0.0, 0.0, 0.0);
		for (int j = 0; j < finalform[i].size(); j++) {
			glVertex2f(finalform[i][j].x, finalform[i][j].y);
		}
		glEnd();
	}
}

void display()
{
	if (draw == 1) {
		drawSquare();
	}
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();

}


GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y)
{
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		s = { 1, 1 };
		t = 0;
		eltol = { 0,0 };
		sizeTenTimes = 0;
		outxMouse = xMouse;
		outyMouse = winHeight - yMouse;
		draw = 1;
		counter = finalform.size();
	}

	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
		dragged = -1;
	}
}

void update(int n) {
	glutPostRedisplay();
	if (draw == 1) {
		calc(vec2(outxMouse, outyMouse), radius);
	}
	glutTimerFunc(5, update, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hatodik");
	init();
	glutMouseFunc(processMouse);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}

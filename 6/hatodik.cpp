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
GLint pressed = 0;
GLint limit = 0;
GLfloat radius = 15;
GLint outxMouse, outyMouse;

vec2 s = { 1, 1 };
float scaleChange = 0.025;
float t = 0;
vec2 eltol = { 0,0 };
int flag = 0;
int counter = 0;
vector<vec2> todraw;
vector<vector<vec2>> finalform;

float randomA = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
float randomB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
float randomC = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(7.0);
	glLineWidth(1.4f);
	glLineStipple(1, 0xFF00);
}

void calc(vec2 O, GLfloat r) {
	vec2 firstSquareFirstPoint = { O.x + r * cos(pi() / 4), O.y + r * sin(pi() / 4) };
	vec2 firstSquareSecondPoint = { O.x + r * cos(pi() / 4 + 2 * pi() / 4), O.y + r * sin(pi() / 4 + 2 * pi() / 4) };

	todraw.clear();
	if(flag == 1) {
		
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

		for (GLfloat t = pi() / 4; t <= 2 * pi() + pi() / 4 ; t += 2 * pi() / 4) {
			vec2 temp = { O.x + r * cos(t), O.y + r * sin(t) };
			vec3 pointH = ihToH(temp);
			vec3 rotatedPointH = M * pointH;
			if (rotatedPointH.z != 0) {
				vec2 result = hToIh(rotatedPointH);
				todraw.push_back(result);
			}
		}

		if (dist(firstSquareFirstPoint, firstSquareSecondPoint) * 10 > dist(todraw[0], todraw[1])) {
			finalform.push_back(todraw);
			t += 0.25;
			s += 0.25;
			eltol += 0.01;
		}else{
			flag = 0;
		}

		sort(finalform.begin() + counter, finalform.end(), [](vector<vec2> a, vector<vec2> b) {
			return (dist(a.at(0), a.at(1)) > dist(b.at(0), b.at(1)));
		});

	}
	
}

void drawSquare(){
	for (int i = 0; i < finalform.size(); i++) {
		glBegin(GL_POLYGON);
		glColor3f(randomA, randomB, randomC);
		for (int j = 0; j < finalform[i].size(); j++) {
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
	glClear(GL_COLOR_BUFFER_BIT);
	if (pressed == 1) {
		calc(vec2(outxMouse, outyMouse), radius);
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
		flag = 1;
		outxMouse = xMouse;
		outyMouse = winHeight - yMouse;
		pressed = 1;
		counter = finalform.size();
		
		randomA = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		randomB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		randomC = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
		
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
		dragged = -1;
	}
}

void update(int n) {
	glutPostRedisplay();
	/*if (pressed == 1) {
		calc(vec2(outxMouse, outyMouse), radius);
	}*/
	glutTimerFunc(5, update, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand(time(0));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hatodik");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}

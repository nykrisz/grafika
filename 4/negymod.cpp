#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <iostream>
#include <vector>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;
GLint dragged = -1;

vec2 P0 = {};
vec2 P1 = {};
vec2 P2 = {};
vec2 P3 = {};

vec2 points[4] = { P0, P1, P2, P3 };
GLfloat t0 = -1, t1 = 0, t2 = 1.0, t3 = 2.0;
GLfloat ny = 0, red = 0.15, gre = 0.75, blue = 0.75;
mat24 G;

mat4 M = { t0*t0*t0, t1*t1*t1, t2*t2*t2, t3*t3*t3,
t0 * t0, t1 * t1 , t2 * t2 ,        t3*t3,
t0     , t1      , t2      ,             t3,
1      ,        1,        1,             1 };

int psize = 0, go = 0, dw = 0;
vector<vec2> curveP;
vector<vec2> torzsVec;
vector<vec2> starVector;
vector<vec2> nVec;
GLfloat maxYPoint = 0;

void init()
{
	glClearColor(red, gre, blue, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(7.0);
	glLineWidth(2.0);
	glLineStipple(1, 0xFF00);
}

void h_dots() {
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 1.0);
	for (int i = 0; i < 4; i++) {
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void drawPeople() {
	if (torzsVec[ny].x < curveP[295].x) {
		//torzs
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2f(curveP[ny].x, curveP[ny].y);
		glVertex2f(torzsVec[ny].x, torzsVec[ny].y);
		glEnd();

		//lab
		glBegin(GL_LINES);
		vec2 n = { nVec[ny].x, nVec[ny].y };
		vec2 v = { -nVec[ny].y, nVec[ny].x };
		n = normalize(n);
		n *= 14;

		v = normalize(v);
		v *= -7;

		glVertex2f(curveP[ny].x + n.x, curveP[ny].y + n.y);
		glVertex2f(curveP[ny].x + v.x, curveP[ny].y + v.y);
		glEnd();

		//kez
		glBegin(GL_LINES);
		vec2 nn = normalize(n);
		vec2 vv = normalize(v);
		nn *= 21;
		vv *= 5;

		glVertex2f(curveP[ny].x + nn.x, curveP[ny].y + nn.y);
		glVertex2f(curveP[ny].x + vv.x + nn.x, curveP[ny].y + vv.y + nn.y);
		glEnd();

		//fej
		GLfloat r = 5;
		glBegin(GL_LINE_LOOP);
		vec2 hn = normalize(n);
		hn *= 33;
		curveP[ny] += hn;

		for (GLdouble t = 0; t <= 2 * pi(); t += 0.01) {
			glVertex2d(curveP[ny].x + r * cos(t), curveP[ny].y + r * sin(t));
		}
		glEnd();
	}
	else {
		go = 0;
	}
}

void drawWindow() {
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
		vec2 wll = { nVec[293].x, nVec[293].y };
		vec2 wlu = { nVec[293].x, nVec[293].y };
		vec2 wrl = { nVec[297].x, nVec[297].y };
		vec2 wru = { nVec[297].x, nVec[297].y };
		wll = normalize(wll);
		wll *= 10;
		wlu = normalize(wlu);
		wlu *= 20;
		wrl = normalize(wrl);
		wrl *= 10;
		wru = normalize(wru);
		wru *= 20;

		glVertex2f(curveP[293].x + wll.x, curveP[293].y + wll.y);
		glVertex2f(curveP[293].x + wlu.x, curveP[293].y + wlu.y);
		glVertex2f(curveP[297].x + wru.x, curveP[297].y + wru.y);
		glVertex2f(curveP[297].x + wrl.x, curveP[297].y + wrl.y);
		
	glEnd();
}

void drawHouse() {
	glColor3f(0.70, 0.70, 0.70);
	glBegin(GL_POLYGON);
		vec2 vl = { -torzsVec[290].y, torzsVec[290].x };
		vec2 vr = { -torzsVec[300].y, torzsVec[300].x };
		vl = normalize(vl);
		vr = normalize(vr);

		glVertex2f(vl.x + curveP[290].x, vl.y + curveP[290].y);
		glVertex2f(torzsVec[290].x, torzsVec[290].y);
		glVertex2f(torzsVec[300].x, torzsVec[300].y);
		glVertex2f(vr.x + curveP[300].x, vr.y + curveP[300].y);
	glEnd();

	//teto
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		vec2 n = { nVec[295].x, nVec[295].y };
		n = normalize(n);
		n *= 40;
		glVertex2f(torzsVec[290].x, torzsVec[290].y);
		glVertex2f(curveP[295].x + n.x, curveP[295].y + n.y);
		glVertex2f(torzsVec[300].x, torzsVec[300].y);
	glEnd();
	if (torzsVec[ny].x > curveP[295].x) {
		drawWindow();
	}
}

void hermite() {
	glClearColor(red, gre, blue, 0.0);
	h_dots();
	vec2 tmp[4] = { points[0], points[1], points[2], points[3] };

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == 0) { G[i][j] = tmp[j].x; }
			if (i == 1) { G[i][j] = tmp[j].y; }
		}
	}

	mat4 M_ = inverse(M);
	mat24 C = G * M_;

	if (psize < 4) {
		return;
	}
	else {
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		torzsVec.clear();
		curveP.clear();
		nVec.clear();
		for (GLfloat t = t0; t <= t3; t += 0.01) {
			
			vec4 Tder = { 3 * t *t, 2 * t, 1 , 0 };
			vec4 T = { t*t*t, t*t, t, 1 };
			vec2 curvePoints = C * T;
			vec2 v = C * Tder;
			vec2 n = { -v.y, v.x };
			n = normalize(n);
			n *= 30;
			
			nVec.push_back(n);
			torzsVec.push_back(n + curvePoints);
			curveP.push_back(curvePoints);
			
			glVertex2f(curvePoints.x, curvePoints.y);
		}
		glEnd();
	}
	drawPeople();
	drawHouse();
}

void stars() {
	vec2 star = {};
	starVector.clear();
	for (int i = 0; i < 25; i++) {
		GLint max = winHeight - maxYPoint;

		star = { float(rand() % winWidth), float(rand() % max + (maxYPoint + 20)) };
		starVector.push_back(star);
	}
}

void drawStars() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < starVector.size(); i++) {
		glVertex2f(starVector[i].x, starVector[i].y);
	}
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	hermite();
	drawStars();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		go = 1;
		break;
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

	if (psize < 4) {
		if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
			points[psize].x = xMouse;
			points[psize].y = winHeight - yMouse;
			psize++;
		}
	}

	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(points, 4, 8, xMouse, winHeight - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0 && go == 1) {
		curveP.clear();
		torzsVec.clear();
		nVec.clear();
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}

void update(int n) {
	if (go == 1) {
		ny += 0.1;
		red -= 0.0002;
		gre -= 0.0002;
		//blue -= 0.0002;
		for (int i = 0; i < curveP.size(); i++) {
			if (points[i].y > maxYPoint) {
				maxYPoint = points[i].y;
			}
		}

		glutPostRedisplay();
	}
	glutTimerFunc(5, update, 0);
}

void updateStars(int n) {
	if (ny > curveP.size()/2 && go == 1) {
		stars();
		glutPostRedisplay();
	}
	glutTimerFunc(1000, updateStars, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hermite");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutTimerFunc(5, update, 0);
	glutTimerFunc(5, updateStars, 0);
	glutMainLoop();
	return 0;
}

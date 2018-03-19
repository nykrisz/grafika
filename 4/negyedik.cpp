#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <iostream>
#include <vector>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;
GLint dragged = -1;

vec2 P0 = {  };
vec2 P1 = {  };
vec2 P2 = {  };
vec2 P3 = {  };

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
vector<vec2> nVec;
vector<vec2> starVector;
vector<vec2> labVec;
vector<vec2> kezVec;
vector<vec2> torzsVec;
vector<vec2> irany;
vector<vec2> haz;
vector<vec2> htop;
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
	if (labVec[ny].x < haz[295].x) {
		GLfloat r = 5;

		//torzs
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
			glVertex2f(curveP[ny].x, curveP[ny].y);
			glVertex2f(nVec[ny].x, nVec[ny].y);
		glEnd();
		
		//lab
		glBegin(GL_LINES);
		glVertex2f(labVec[ny].x, labVec[ny].y);
		glVertex2f(torzsVec[ny].x, torzsVec[ny].y);
		glEnd();

		//kez
		glBegin(GL_LINES);
		glVertex2f(kezVec[ny].x, kezVec[ny].y);
		glVertex2f(irany[ny].x, irany[ny].y);
		glEnd();
		
		
		//fej
		glBegin(GL_LINE_LOOP);
		for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
			glVertex2d(nVec[ny].x + r * cos(t), nVec[ny].y + r * sin(t));
		glEnd();
		
	}
	else {
		go = 0;
	}
}

void drawWindow() {
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(haz[293].x, haz[293].y);
	glVertex2f(nVec[293].x, nVec[293].y);
	glVertex2f(nVec[297].x, nVec[297].y);
	glVertex2f(haz[297].x, haz[297].y);
	glEnd();
	
}

void drawHouse() {
	glColor3f(0.70, 0.70, 0.70);
	glBegin(GL_POLYGON);
		glVertex2f(haz[290].x, haz[290].y);
		glVertex2f(nVec[290].x, nVec[290].y);
		glVertex2f(nVec[300].x, nVec[300].y);
		glVertex2f(haz[300].x, haz[300].y);
	glEnd();
	
	//teto
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(nVec[290].x, nVec[290].y);
		glVertex2f(htop[295].x, htop[295].y);
		glVertex2f(nVec[300].x, nVec[300].y);
	glEnd();
	
	if (labVec[ny].x > haz[295].x) {
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
		nVec.clear();
		haz.clear();
		curveP.clear();
		labVec.clear();
		kezVec.clear();
		irany.clear();
		torzsVec.clear();
		htop.clear();
		for (GLfloat t = t0; t <= t3; t += 0.01) {
			vec2 lab = {};
			vec2 kez = {};
			vec2 torzs = {};
			vec2 ht = {};

			vec4 Tder = { 3 * t *t, 2 * t, 1 , 0};
			vec4 T = { t*t*t, t*t, t, 1 };
			vec2 curvePoints = C * T;
			vec2 v = C * Tder;
			vec2 n = { -v.y, v.x };
			n = normalize(n);
			lab = normalize(v);
			lab *= 7;
			
			ht = n;
			ht *= 45;

			torzs = n;
			torzs *= 10;

			kez = n;
			kez *= 20;
			n *= 30;
			v = normalize(v);
			v *= 5;
			
			haz.push_back(v + curvePoints);
			irany.push_back(v + curvePoints+kez);
			kezVec.push_back(kez + curvePoints);
			labVec.push_back(lab + curvePoints);
			nVec.push_back(n + curvePoints);
			torzsVec.push_back(torzs + curvePoints);
			curveP.push_back(curvePoints);
			htop.push_back(ht + curvePoints);

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

		star = { float(rand() % winWidth), float(rand() % max + (maxYPoint+20)) };
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

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse){	

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
	if (dragged >= 0) {
		haz.clear();
		curveP.clear();
		nVec.clear();
		labVec.clear();
		kezVec.clear();
		torzsVec.clear();
		irany.clear();
		maxYPoint = 0;
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
	if (ny > 150 && go == 1) {
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

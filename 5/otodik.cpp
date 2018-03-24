#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <iostream>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;
GLint dragged = -1;
//b1
vec2 P0 = { 100, 120 };
vec2 P1 = { 100, 150 };
vec2 P2 = { 100, 175 };
vec2 P3 = { 150, 175 };
//b2
vec2 P4 = { 200,175 };
vec2 P5 = { 175, 215 };
vec2 P6 = { 165, 285 };
//b3
vec2 P7 = { 175, 355 };
vec2 P8 = { 300, 450 };
vec2 P9 = { 410, 430 };
//hermite
vec2 P10 = { 555, 340 };
vec2 P11 = { 750, 205 };
vec2 P12 = { 750, 120 };
//decast
vec2 P13 = { 250, 270 };
vec2 P14 = { 245, 350 };
vec2 P15 = { 300, 405 };
vec2 P16 = { 410, 400 };
vec2 P17 = { 480, 325 };

vec2 points[18] = { P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16, P17};
vec2 e0;

float tmod = 0.5;
int showP = -1, showL = -1, showDc = -1;
GLfloat t0 = -1, t1 = -0.5, t2 = 1;
mat24 G;

mat4 M = { t0*t0*t0, t1*t1*t1, t2*t2*t2, 3 * (t2 * t2),
			t0 * t0, t1 * t1 , t2 * t2 ,        2 * t2,
			t0     , t1      , t2      ,             1,
			1      ,        1,        1,             0 };

void init()
{
	glClearColor(0.6, 0.9, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(7.0);
	glLineWidth(2.0);
	glLineStipple(1, 0xFF00);
}

void drawBackground() {
	glColor3f(0.4, 1.0, 0.5);
	glBegin(GL_POLYGON);
		glVertex2f(0, 0);
		glVertex2f(0, winHeight / 4);
		glVertex2f(winWidth, winHeight / 4);
		glVertex2f(winWidth, 0);
	glEnd();
}

void bern_p() {
	glColor3f(0.0, 0.0, 1.0);
	if (showP == 1) {
		glBegin(GL_POINTS);
		for (int i = 0; i < 13; ++i) {
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
	}


	glColor3f(0.0, 0.0, 0.0);
	if (showL == 1) {
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 13; i++) {
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
	}

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		glVertex2f(points[9].x, points[9].y);
		glVertex2f(points[10].x, points[10].y);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex2f(points[12].x, points[12].y);
		glVertex2f(points[0].x, points[0].y);
	glEnd();

}

int factorial(int n) {
	int r = 1;
	for (int i = n; i>0; i--) {
		r *= i;
	}
	return r;
}

void bezier_bern_first() {
	vec2 tmp[4] = { points[0], points[1], points[2], points[3] };
	GLint n = 3;

	glColor3f(0.5, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t < 1; t += 0.01) {

		vec2 curvePointsb;
		for (GLint i = 0; i < n + 1; i++) {

			GLfloat bi = factorial(n) / (factorial(i) * factorial(n - i));
			bi *= pow(t, i);
			bi *= pow(1 - t, n - i);

			curvePointsb += {tmp[i].x * bi, tmp[i].y * bi};
		}
		glVertex2f(curvePointsb.x, curvePointsb.y);
	}

	glEnd();
}

void bezier_bern_second() {
	vec2 tmp[4] = { points[3], points[4], points[5], points[6] };
	GLint n = 3;

	glColor3f(0.9, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t < 1; t += 0.01) {

		vec2 curvePointsb;
		for (GLint i = 0; i < n + 1; i++) {

			GLfloat bi = factorial(n) / (factorial(i) * factorial(n - i));
			bi *= pow(t, i);
			bi *= pow(1 - t, n - i);

			curvePointsb += {tmp[i].x * bi, tmp[i].y * bi};
		}
		glVertex2f(curvePointsb.x, curvePointsb.y);
	}

	glEnd();
}

void bezier_bern_third() {
	vec2 tmp[4] = { points[6], points[7], points[8], points[9] };
	GLint n = 3;

	glColor3f(1.0, 0.4, 0.0);
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t < 1; t += 0.01) {

		vec2 curvePointsb;
		for (GLint i = 0; i < n + 1; i++) {

			GLfloat bi = factorial(n) / (factorial(i) * factorial(n - i));
			bi *= pow(t, i);
			bi *= pow(1 - t, n - i);

			curvePointsb += {tmp[i].x * bi, tmp[i].y * bi};
		}
		glVertex2f(curvePointsb.x, curvePointsb.y);
	}

	glEnd();

	bezier_bern_first();
	bezier_bern_second();

	bern_p();
}

void hermite() {
	e0 = { points[9].x - points[10].x, points[9].y - points[10].y };
	
	vec2 tmp[4] = { points[12], points[11], points[10], e0 };

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == 0) { G[i][j] = tmp[j].x; }
			if (i == 1) { G[i][j] = tmp[j].y; }
		}
	}

	mat4 M_ = inverse(M);
	mat24 C = G * M_;

	glColor3f(1.0, 0.8, 0.0);
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = t0; t <= t2; t += 0.01) {
		vec4 T = { t*t*t, t*t, t, 1 };
		vec2 curvePoints = C * T;
		glVertex2f(curvePoints.x, curvePoints.y);
	}
	glEnd();

}

void decast_p() {
	vec2 pts[] = { points[13], points[14], points[15], points[16], points[17] };
	//alsopont
	vec2 dlPoint = { points[17].x - points[13].x,points[17].y - points[13].y };
	dlPoint = normalize(dlPoint);

	int gen = 0;
	float t = tmod;

	for (int i = 0; i < 4; i++) {
		// Összekötjük az akutális generációhoz tartozó pontokat
		glColor3f(0.0, 0.0, 1.0);
		if (showDc == 1) {
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < 5 - gen; j++) {
				glVertex2f(pts[j].x, pts[j].y);
			}
			glEnd();
		}
		// Kiszámoljuk a következõ generációt.
		for (int j = 0; j < 5 - gen - 1; j++) {
			pts[j].x = (1 - t) * pts[j].x + t * pts[j + 1].x;
			pts[j].y = (1 - t) * pts[j].y + t * pts[j + 1].y;
		}

		gen++;
	}

	glColor3f(0.0, 0.0, 1.0);
	if (showP == 1) {
		glBegin(GL_POINTS);
		for (int i = 13; i < 18; i++) {
			glVertex2f(points[i].x, points[i].y);
		}
		glEnd();
	}
	glColor3f(1.0, 0.0, 0.0);
	if(showDc == 1){
		glBegin(GL_POINTS);
			glVertex2f(pts[0].x, pts[0].y);
		glEnd();
	}
	//alsopont
	//cout << dlPoint.x << " " << dlPoint.y << endl;
	glBegin(GL_POINTS);
	glVertex2f(pts[0].x + dlPoint.x, pts[0].y + dlPoint.y);
	glEnd();

}

void bezier_decast() {
	
	vec2 tmp[5] = { points[13], points[14], points[15], points[16], points[17] };

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (GLfloat t = 0; t < 1; t += 0.01) {
		vec2 curvePoints;
		for (GLint r = 0; r < 4; r++) {
			for (GLint i = 0; i < 4 - r; i++) {
				curvePoints = { (1 - t) * tmp[i].x + t * tmp[i + 1].x , (1 - t) * tmp[i].y + t * tmp[i + 1].y };
				tmp[i] = curvePoints;
			}
		}
		glVertex2f(tmp[0].x, tmp[0].y);
	}
	glEnd();
	decast_p();
}

void circle(vec2 O, GLdouble r) {
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_POLYGON);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}

void ten(vec2 O, GLdouble r) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (GLdouble t = 0; t <= 2 * pi(); t += 2 * pi() / 10)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}

void wheels() {
	float radius = 50;

	vec2 leftWheel = { (points[12].x - points[0].x) * 1 / 4, (points[12].y - points[0].y) * 1 / 4 };
	vec2 rightWheel = { (points[12].x - points[0].x) * 3/4, (points[12].y - points[0].y) * 3/4 };
	
	leftWheel += points[0];
	rightWheel += points[0];

	circle(leftWheel, radius);
	circle(rightWheel, radius);
	
	ten(leftWheel, 30);
	ten(rightWheel, 30);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	bezier_bern_third();
	hermite();
	bezier_decast();
	wheels();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 's':
		if (tmod < 0.99)
			tmod += 0.01;
		break;
	case 'w':
		if (tmod > 0.01)
			tmod -= 0.01;
		break;
	case 'p':
		showP *= -1;
		break;
	case 'l':
		showL *= -1;
		break;
	case 'd':
		showDc *= -1;
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

GLint getActivePoint2(vec2 *p, GLint size, GLint sens, GLint x, GLint y)
{
	GLint i;
	for (i = 0; i < size; i++)
		if (fabs((*(p + i)).x - x) < sens && fabs((*(p + i)).y - y) < sens)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(points, 18, 8, xMouse, winHeight - yMouse)) != -1)
			dragged = i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) {
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Otodik");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();
	return 0;
}

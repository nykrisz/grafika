#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <vector>

using namespace std;

GLsizei winWidth = 800, winHeight = 600;

vec2 points[4] = {};
GLint dragged = -1;
vec2 movablePoints[4] = {};
vec2 innerPoints[2] = {};
vec2 intersect = {};
vector<vec2> intersectionPoint(4);
int flag = 0;
//kor
float radius = 20;
vec2 circleCenter = { 350, 200 };
vec2 d = { 0.5, 0.5 };

void initPoints() {
	movablePoints[0] = { 100, 100 };
	movablePoints[1] = { 500, 500 };
	movablePoints[2] = { 200, 250 };
	movablePoints[3] = { 350, 350 };
	points[0] = { movablePoints[0] }, points[2] = { movablePoints[1] },
	points[1] = vec2(points[2].x, points[0].y), points[3] = vec2(points[0].x, points[2].y);
	innerPoints[0] = { movablePoints[2] };
	innerPoints[1] = { movablePoints[3] };
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_POINT_SMOOTH);
	initPoints();
	glPointSize(8.0);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
}

double pointWhichSide(vec2 E, vec2 F, vec2 vizsgalt) {
	vec2 v = F - E;
	vec2 n = { -v.y, v.x };
	double a = n.x;
	double b = n.y;
	double c = -a * E.x - b * E.y;

	return a * vizsgalt.x + b * vizsgalt.y + c;
}

void drawBackgroundPoints() {
	glBegin(GL_POINTS);
	flag = 0;
	for (int i = points[0].x; i < points[1].x; i += 35, flag++) {
		int z = 0;
		if (flag % 2 == 0) {
			z = 15;
		}
		for (int j = points[0].y + z; j < points[3].y; j += 30) {
			if ((pointWhichSide(innerPoints[0], innerPoints[1], vec2(i, j))) < 0) {
				glColor3f(0.0, 1.0, 0.0);
				glVertex2f(float(i), float(j));
				if (pow((i - circleCenter.x), 2) + pow((j - circleCenter.y), 2) - pow(radius, 2) < 0) {
					glColor3f(0.0, 0.0, 0.0);
					glVertex2f(float(i), float(j));
				}
			}
			else {
				glColor3f(1.0, 1.0, 0.0);
				glVertex2f(float(i), float(j));
				if (pow((i - circleCenter.x), 2) + pow((j - circleCenter.y), 2) - pow(radius, 2) < 0) {
					glColor3f(0.0, 0.0, 0.0);
					glVertex2f(float(i), float(j));
				}
			}
		}
	}
	glEnd();
}

void circle(vec2 O, GLdouble r) {
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}

void drawIntersection() {
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 1.0);
	for (int i = 0; i < 4; i++) {
		if ((intersectionPoint[i].x > points[0].x && intersectionPoint[i].x < points[1].x) ||
			(intersectionPoint[i].y > points[0].y && intersectionPoint[i].y < points[2].y)) {
			glVertex2f(intersectionPoint[i].x, intersectionPoint[i].y);
		}
	}
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < 4; i++) {
		if ((intersectionPoint[i].x > points[0].x && intersectionPoint[i].x < points[1].x) ||
			(intersectionPoint[i].y > points[0].y && intersectionPoint[i].y < points[2].y)) {
			glVertex2f(intersectionPoint[i].x, intersectionPoint[i].y);
		}
	}
	glEnd();
}

void drawPoints() {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; i++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		glVertex2f(movablePoints[i].x, movablePoints[i].y);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	drawPoints();
	drawIntersection();
	drawBackgroundPoints();
	circle(circleCenter, radius);

	glutSwapBuffers();
}

vec2 lineLineIntersection(vec2 A, vec2 B) {
	//AB
	double a1 = A.y - B.y;
	double b1 = B.x - A.x;
	double c1 = (B.y - A.y) * (A.x) - (B.x - A.x) * (A.y);
	//EF
	double a2 = innerPoints[0].y - innerPoints[1].y;
	double b2 = innerPoints[1].x - innerPoints[0].x;
	double c2 = (innerPoints[1].y - innerPoints[0].y) * (innerPoints[0].x) - (innerPoints[1].x - innerPoints[0].x) * (innerPoints[0].y);

	double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0) {
		return 0;
	}
	else {
		float x = (b2*(-c1) - b1 * (-c2)) / determinant;
		float y = (a1*(-c2) - a2 * (-c1)) / determinant;

		intersect = { x,y };
		return intersect;
	}
}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y) {
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePoint1(movablePoints, 4, 8, xMouse, winHeight - yMouse)) != -1)
			dragged = i;

	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
		dragged = -1;
}

double circleLineDistance() {
	GLdouble clDist = 0;
	return clDist = fabs((innerPoints[1].x - innerPoints[0].x) * (innerPoints[0].y - circleCenter.y) - (innerPoints[0].x - circleCenter.x) * (innerPoints[1].y - innerPoints[0].y))
		/ sqrt(pow((innerPoints[1].x - innerPoints[0].x), 2) + pow((innerPoints[0].y - innerPoints[1].y), 2));
}

void updatePoints() {
	points[0] = movablePoints[0];
	points[2] = movablePoints[1];
	points[1] = vec2(points[2].x, points[0].y),
	points[3] = vec2(points[0].x, points[2].y);
	innerPoints[0] = { movablePoints[2] };
	innerPoints[1] = { movablePoints[3] };
	//AB
	intersectionPoint[0] = lineLineIntersection(points[0], points[1]);
	//BC
	intersectionPoint[1] = lineLineIntersection(points[1], points[2]);
	//CD
	intersectionPoint[2] = lineLineIntersection(points[2], points[3]);
	//AC
	intersectionPoint[3] = lineLineIntersection(points[0], points[3]);
}

void rebound() {
	vec2 na = {}; vec2 va = {};
	vec2 nb = {}; vec2 vb = {};
	vec2 ne = {}; vec2 ve = {};

	va = { points[1].x - points[0].x, points[1].y - points[0].y };
	na = { -va.y, va.x };

	vb = { points[2].x - points[1].x, points[2].y - points[1].y };
	nb = { -vb.y, vb.x };

	ve = { innerPoints[1].x - innerPoints[0].x, innerPoints[1].y - innerPoints[0].y };;
	ne = { -ve.y, ve.x };

	circleCenter += d;
	if (circleCenter.x + radius > points[1].x || circleCenter.x - radius < points[0].x) {
		d = -d + 2 * dot(na, d) / length2(na) * na;
	}
	else if (circleCenter.y + radius > points[3].y || circleCenter.y - radius < points[0].y) {
		d = -d + 2 * dot(nb, d) / length2(nb) * nb;
	}
	if (circleLineDistance() < radius) {
		d = (-d + 2 * dot(ne, d) / length2(ne) * ne) * -1;
	}
}

void update(int n) {
	updatePoints();
	rebound();
	glutPostRedisplay();

	glutTimerFunc(5, update, 0);
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	if (dragged >= 0) {
		movablePoints[dragged].x = xMouse;
		movablePoints[dragged].y = winHeight - yMouse;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("masodik hazi");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}
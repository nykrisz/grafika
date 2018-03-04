#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <time.h>

const int incRadius = 2;
const int movCenter = 10;

float radius = 20;
vec2 center = { 50, 100 };
vec2 d = {0.5, 0.5};	 
vec2 randP = {};		
GLsizei winWidth = 400, winHeight = 300;
vec2 winPoints[4] = { {0,0}, {400, 0}, {400, 300}, {0, 300} };
GLdouble wdd = 0, wdr = 0, wdu = 0, wdl = 0;
int counter = 0;

void drawPoint() {
	glBegin(GL_POINTS);
	glVertex2f(randP.x, randP.y);
	glEnd();
}

void point() {
	randP = { float(rand() % (winWidth + 1)), float(rand() % (winHeight + 1)) };
	
	if(dist(randP, vec2(0, 0)) <= radius ||
		dist(randP, vec2(0, winHeight)) <= radius ||
		dist(randP, vec2(winWidth, 0)) <= radius ||
		dist(randP, vec2(winWidth, winHeight)) <= radius ||
		dist(randP, center) <= radius )	
	{
		point();
	}
}

void init(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(8.0);
	randP = {200, 150};
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
}

void circle(vec2 O, GLdouble r){
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	drawPoint();
	circle(center, radius);
	glutSwapBuffers();
}

void update(int n){
	center += d;
	if (center.x + radius > winWidth || center.x - radius < 0) {
		d.x *= -1;
	}else if (center.y + radius > winHeight || center.y - radius < 0){
		d.y *= -1;
	}
	
	wdd = fabs((winPoints[1].x - winPoints[0].x) * (winPoints[0].y - randP.y) - (winPoints[0].x - randP.x) * (winPoints[1].y - winPoints[0].y))
		/ sqrt(pow((winPoints[1].x - winPoints[0].x), 2) + pow((winPoints[0].y - winPoints[1].y), 2));

	wdr = fabs((winPoints[2].x - winPoints[1].x) * (winPoints[1].y - randP.y) - (winPoints[1].x - randP.x) * (winPoints[2].y - winPoints[1].y))
		/ sqrt(pow((winPoints[2].x - winPoints[1].x), 2) + pow((winPoints[1].y - winPoints[2].y), 2));

	wdu = fabs((winPoints[3].x - winPoints[2].x) * (winPoints[2].y - randP.y) - (winPoints[2].x - randP.x) * (winPoints[3].y - winPoints[2].y))
		/ sqrt(pow((winPoints[3].x - winPoints[2].x), 2) + pow((winPoints[2].y - winPoints[3].y), 2));

	wdl = fabs((winPoints[0].x - winPoints[3].x) * (winPoints[3].y - randP.y) - (winPoints[3].x - randP.x) * (winPoints[0].y - winPoints[3].y))
		/ sqrt(pow((winPoints[0].x - winPoints[3].x), 2) + pow((winPoints[3].y - winPoints[0].y), 2));
	
	if (dist(center, randP) <= radius) {
		if (wdd <= incRadius) {
			center.y += movCenter;
		}
		else if (wdu <= incRadius) {
			center.y -= movCenter;
		}
		else if (wdr <= incRadius) {
			center.x -= movCenter;
		}
		else if (wdl <= incRadius) {
			center.x += movCenter;
		}
		point();
		radius += incRadius;
		counter++;
	}
	if (counter == 10) {
		exit(0);
	}
	glutPostRedisplay();

	glutTimerFunc(5, update, 0);
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'a':
		d.x *= -1;
		break;
	case 'd':
		d.y *= -1;
		break;
	}
}

int main(int argc, char** argv){
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("elso hazi");
	init();
	glutDisplayFunc(display);
	glutKeyboardUpFunc(keyboard);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}
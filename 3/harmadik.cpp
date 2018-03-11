#include <GL/glut.h>
#include <bevgrafmath2017.h>
#include <iostream>
#include <vector>

using namespace std;

double radius = 20;
double ellipseWidth = 50, ellipseHeight = 17;

GLfloat winWidth = 800, winHeight = 600;
vec2 centerEllipse = { winWidth * 1/3 , winHeight * 4/5};
vec2 centerHalfCircle = { float(centerEllipse.x), float(centerEllipse.y + ellipseHeight - 1) };
vec2 nV = {};

int counter = 0;
GLfloat red = 1.0f, gr = 1.0f, bl = 1.0f;
//vec2 base[4] = { {winWidth / 2 - 20, 0}, {winWidth / 2 + 20, 0}, {winWidth / 2 + 20, 20}, {winWidth / 2 - 20, 20} };

GLfloat uSpeed = 0.8, maxSpeed = 2;
GLint mouseX, mouseY;

struct Bullets {
	vec2 dir;
	float speed = 0.03;
	vec2 coord;
	void updatePoint() {
		coord.x += dir.x * speed;
		coord.y += dir.y * speed;
	}
};

Bullets bullet;
vector<Bullets> bulletsVector;

vector<Bullets> bulletsInAir;


void genLeftSide() {
	int z = 0, p = 0;
	
	for (int i = 0; i < 7; i++, z -= 9) {
		p = 0;
		for (int j = i; j >= 0; j--, p -= 9) {
			bullet.coord = { float(j + p + 5 + -z), float(53 + i + z) };
			bulletsVector.push_back(bullet);
		}
	}
}

void genRightSide() {
	int z = 0, p = 0;

	for (int i = 0; i < 7; i++, z -= 10 ) {
		p = 0;
		for (int j = i; j >= 0; j--, p += 10) {
			bullet.coord = { float(j + p + winWidth - 5 + z), float(60 + i + z) };
			bulletsVector.push_back(bullet);
		}
	}
}

void drawAmmo() {
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < bulletsVector.size(); i++) {
		glBegin(GL_POINTS);
			glVertex2f(bulletsVector[i].coord.x, bulletsVector[i].coord.y);
		glEnd();
	}
}

void init(){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glPointSize(6.0);
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(1.0);
	genLeftSide();
	genRightSide();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
}

void semicircle(vec2 O, GLdouble r){
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();

	glColor3f(red, gr, bl);
	glBegin(GL_POLYGON);
	for (GLdouble t = 0; t <= pi(); t += 0.01)
		glVertex2d(O.x + r * cos(t), O.y + r * sin(t));
	glEnd();
}

void ellipse(vec2 O, GLdouble a, GLdouble b){
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + a * cos(t), O.y + b * sin(t));
	glEnd();

	glColor3f(red, gr, bl);
	glBegin(GL_POLYGON);
	for (GLdouble t = 0; t <= 2 * pi(); t += 0.01)
		glVertex2d(O.x + a * cos(t), O.y + b * sin(t));
	glEnd();
}

void cannon() {
	glColor3f(0.0, 0.0, 1.0);
	
	glBegin(GL_POLYGON);
		glVertex2f(winWidth/2 - 20, 0);
		glVertex2f(winWidth / 2 + 20, 0);
		glVertex2f(winWidth / 2 + 20, 20);
		glVertex2f(winWidth / 2 - 20, 20);
	glEnd();

	//iranyvektor
	vec2 v = { float(mouseX - winWidth / 2), float(mouseY - 20) };
	//normalize
	nV = normalize(v);
	//x
	nV *= 30;
	glBegin(GL_LINES);
		glVertex2f(winWidth/2, 20);
		glVertex2f(winWidth / 2 + nV.x, 20 + nV.y);
	glEnd();

}

void shot() {
	bullet.coord = { winWidth / 2 + nV.x, 20 + nV.y };
	bullet.dir = { nV.x, nV.y };
	bulletsInAir.push_back(bullet);
}

void drawBulletsInAir() {
	glBegin(GL_POINTS);
		for (int i = 0; i < bulletsInAir.size(); i++) {
			glVertex2f(float(bulletsInAir[i].coord.x), float(bulletsInAir[i].coord.y));
			bulletsInAir[i].updatePoint();
		}
	glEnd();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	semicircle(centerHalfCircle, radius);
	ellipse(centerEllipse, ellipseWidth, ellipseHeight);
	glLineWidth(3.0f);
	cannon();
	drawAmmo();
	drawBulletsInAir();
	
	glutSwapBuffers();
}

void ufoMove() {
	centerEllipse.x += uSpeed;
	centerHalfCircle.x += uSpeed;

	if (centerEllipse.x + ellipseWidth > winWidth || centerEllipse.x - ellipseWidth < 0) {
		uSpeed *= -1.0f;
		if (abs(uSpeed) < maxSpeed)
			uSpeed += uSpeed * 0.2f;
	}
}

void endGame() {
	if (counter == 10) {
		//cout << "win" << endl;
		glClearColor(0.0, 1.0, 0.0, 0.0);
	}
	else if (bulletsVector.size() == 0 && bulletsInAir.size() == 0 && counter < 10) {
		//cout << "lose" << endl;
		glClearColor(1.0, 0.0, 0.0, 0.0);
	}
}

void ufoHit() {
	for (int i = 0; i < bulletsInAir.size(); i++) {
		if ((pow((bulletsInAir[i].coord.x - centerEllipse.x), 2) / pow(ellipseWidth, 2)) + (pow((bulletsInAir[i].coord.y - centerEllipse.y), 2) / pow(ellipseHeight, 2)) - 1 < 0
			|| pow((bulletsInAir[i].coord.x - centerHalfCircle.x), 2) + pow((bulletsInAir[i].coord.y - centerHalfCircle.y), 2) - pow(radius, 2) < 0) {
			bulletsInAir.erase(bulletsInAir.begin() + i);
			//cout << "talalt" << endl;
			gr -= 0.1f;		bl -= 0.1f;
			counter++;
			//cout << counter << endl;
		}
		else if (bulletsInAir[i].coord.x > winWidth || bulletsInAir[i].coord.x < 0 || bulletsInAir[i].coord.y > winHeight ) {
			bulletsInAir.erase(bulletsInAir.begin() + i);
			//cout << "left window" << endl;
		}
	}
}

void update(int n){
	ufoMove();

	ufoHit();
	
	endGame();

	glutPostRedisplay();

	glutTimerFunc(5, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'g':
		
		break;
	}
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse){
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		if (bulletsVector.size() != 0) {
			bulletsVector.erase(bulletsVector.begin());

			shot();
			//cout << bulletsInAir.size() << endl;
			//cout << bulletsVector.size() << endl;
		}
	}

}

void processMousePassiveMotion(GLint xMouse, GLint yMouse){
	mouseX = xMouse;
	mouseY = winHeight - yMouse;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutPassiveMotionFunc(processMousePassiveMotion);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}

#include <GL/glut.h>
#include "bevgrafmath2017.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

GLsizei winWidth = 600, winHeight = 600;

GLfloat center = 10.0f;
GLfloat delta_u = 0.1, delta_theta = pi() / 10;

mat4 M, rotation, w2v, Ct;
mat4 projection = perspective(center);

GLint keyStates[256];

GLint n = 5;
vec2 tmp[6] = { vec2(1.0f, 0.0f), vec2(4,0.1), vec2(3,4), vec2(1,4), vec2(1,6), vec2(3,6.5) };

GLfloat camHeight = 0.0f, basicCircleRadius = 5.0f, alpha = 0.0f;
vec3 cameraX, cameraY, cameraZ, up = { 0.0f, 1.0f, 0.0f }, target = { 0.0f, 0.0f, 0.0f }, eye;
//eye - honnan
//target - hová

GLfloat light_x = 1.0f, light_y = 0.0f, light_z = 0.0f;
vec3 light = { light_x, light_y, light_z };

struct Face {
	vec3 p[4];
	vec3 middlePoint, normVec, rgb;
	float distance;
	bool isBottom = false;

	void drawFace(vec3 rgb) {
		glLineWidth(2.0f);
		glColor3f(rgb.x, rgb.y, rgb.z);
		if (isBottom == false) {
			glBegin(GL_POLYGON);
			for (int i = 0; i < 4; i++) {
				vec4 hPoint = ihToH(p[i]);
				hPoint = M * hPoint;
				vec3 ihDrawablePoint = hToIh(hPoint);
				glVertex2d(ihDrawablePoint.x, ihDrawablePoint.y);
			}
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLES);
			for (int i = 0; i < 3; i++) {
				vec4 hPoint = ihToH(p[i]);
				hPoint = M * hPoint;
				vec3 ihDrawablePoint = hToIh(hPoint);
				glVertex2d(ihDrawablePoint.x, ihDrawablePoint.y);
			}
			glEnd();
		}
	}

	void weights() {
		middlePoint = (hToIh(Ct * ihToH(p[0])) + hToIh(Ct * ihToH(p[1])) + hToIh(Ct * ihToH(p[2])) + hToIh(Ct * ihToH(p[3]))) / 4.0;
		distance = dist(vec3(0.0f, 0.0f, center), middlePoint);
	}
	
	void visibility() {
		vec4 hVector1 = ihToH(p[1] - p[0]), hVector2 = ihToH(p[2] - p[0]);
		normVec = cross(normalize(hToIh(hVector1)), normalize(hToIh(hVector2)));
	}

	void setColor() {
		float cl;
		cl = (dot(normalize(normVec), normalize(light)));
		cl = (cl + 1.0) / 2.0;
		rgb = vec3(cl, cl, cl);
	}

	/*void setColor() {
		vec4 lightH = vec4(light, 0);
		vec3 newLight = normalize(hToIh(transpose(inverse(Ct)) * lightH));

		vec3 newNormal = hToIh(transpose(inverse(Ct)) * vec4(normVec,0));

		if (dot(newNormal, vec3(0,0,center) ) > 0) {
			float cl;
			cl = (dot(normalize(newNormal), normalize(newLight)));
			cl = (cl + 1.0) / 2.0;
			rgb = vec3(cl, cl, cl);
		}
		else {
			float cl;
			cl = (dot(normalize(newNormal), normalize(newLight)));
			cl = (cl + 1.0) / 2.0;
			rgb = vec3(cl, cl, cl);
		}
	}*/

};

vector<Face> faces;

int factorial(int n) {
	int r = 1;
	for (int i = n; i > 0; i--) {
		r *= i;
	}
	return r;
}

vec2 bernstein(float u) {
	vec2 P;
	for (GLint i = 0; i < n + 1; i++) {

		GLfloat bi = factorial(n) / (factorial(i) * factorial(n - i));
		bi *= pow(u, i);
		bi *= pow(1 - u, n - i);

		P += {tmp[i].x * bi, tmp[i].y * bi};
	}
	return P;
}

void initTransformations() {
	
	eye = vec3(basicCircleRadius * cos(alpha), camHeight, -basicCircleRadius * sin(alpha));
	cameraZ = normalize( -(target - eye) ); //eye-target
	cameraX = normalize(cross(up, cameraZ));
	cameraY = normalize(cross(cameraZ, cameraX));
	Ct = coordinateTransform(eye, cameraX, cameraY, cameraZ);

	w2v = windowToViewport3(vec2(0,0), vec2(7, 7), vec2(300.0f, 100.0f), vec2(winWidth, winHeight));

	M = w2v * projection * Ct;
}

void setPoints() {
	vec3 bottom = bernstein(0.0f);

	for (float u = 0.0f; u < 1.0f; u += delta_u) {

		vec2 point = bernstein(u);
		vec2 nextP = bernstein(u + delta_u);

		for (float theta = 0; theta <= 2*pi(); theta += delta_theta) {
			
			faces.resize(faces.size() + 1);
			
			if (point == bottom) {

				faces.at(faces.size() - 1).p[0] = vec3(cos(theta) * bottom.x, bottom.y, -sin(theta) * bottom.x);
				faces.at(faces.size() - 1).p[1] = vec3(cos(theta + delta_theta) * bottom.x, bottom.y, -sin(theta + delta_theta) * bottom.x);
				faces.at(faces.size() - 1).p[2] = vec3(0, 0, 0);
				faces.at(faces.size() - 1).isBottom = true;
				faces.resize(faces.size() + 1);
			}
				
				faces.at(faces.size() - 1).p[0] = vec3(cos(theta) * point.x, point.y, -sin(theta) * point.x);
				faces.at(faces.size() - 1).p[1] = vec3(cos(theta + delta_theta) * point.x, point.y, -sin(theta + delta_theta) * point.x);
				faces.at(faces.size() - 1).p[2] = vec3(cos(theta + delta_theta) * nextP.x, nextP.y, -sin(theta + delta_theta) * nextP.x);
				faces.at(faces.size() - 1).p[3] = vec3(cos(theta) * nextP.x, nextP.y, -sin(theta) * nextP.x);
				
			/* átláthatóság kedvéért
			float p0x = cos(theta) * point.x;
			float p0y = point.y;
			float p0z = -sin(theta) * point.x;

			float p1x = cos(theta + delta_theta) * point.x;
			float p1y = point.y;
			float p1z = -sin(theta + delta_theta) * point.x;

			float p2x = cos(theta + delta_theta) * nextP.x;
			float p2y = nextP.y;
			float p2z = -sin(theta + delta_theta) * nextP.x;

			float p3x = cos(theta) * nextP.x;
			float p3y = nextP.y;
			float p3z = -sin(theta) * nextP.x;
			
			faces.resize(faces.size() + 1);
			faces.at(faces.size() - 1).p[0] = vec3(p0x, p0y, p0z);
			faces.at(faces.size() - 1).p[1] = vec3(p1x, p1y, p1z);
			faces.at(faces.size() - 1).p[2] = vec3(p2x, p2y, p2z);
			faces.at(faces.size() - 1).p[3] = vec3(p3x, p3y, p3z);
*/
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

bool sortFaces(Face f1, Face f2) {
	return f1.distance > f2.distance;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (unsigned int i = 0; i < faces.size(); i++) {
		faces.at(i).weights();
		faces.at(i).visibility();
	}

	sort(faces.begin(), faces.end(), sortFaces);
	
	for (unsigned int i = 0; i < faces.size(); i++) {
			faces.at(i).setColor();
			faces.at(i).drawFace(faces.at(i).rgb);
			glColor3f(0.0f, 0.0f, 0.0f);
			if (faces.at(i).isBottom == false) {
				glBegin(GL_LINE_LOOP);
				for (unsigned int j = 0; j < 4; j++) {
					vec4 tmp = ihToH(faces.at(i).p[j]);
					tmp = M * tmp;
					vec3 tmp2 = hToIh(tmp);
					glVertex2d(tmp2.x, tmp2.y);
				}
				glEnd();
			}
			else {
				glBegin(GL_LINE_LOOP);
				for (unsigned int j = 0; j < 3; j++) {
					vec4 tmp = ihToH(faces.at(i).p[j]);
					tmp = M * tmp;
					vec3 tmp2 = hToIh(tmp);
					glVertex2d(tmp2.x, tmp2.y);
				}
				glEnd();
			}
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
	if (keyStates['a']) { alpha -= 0.01; }
	if (keyStates['d']) { alpha += 0.01; }
	
	if (keyStates['s']) { camHeight -= 0.1; }
	if (keyStates['w']) { camHeight += 0.1; }

	if (keyStates['r']) { basicCircleRadius += 0.1; }
	if(keyStates['t']){ basicCircleRadius -= 0.1; }

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
	glutCreateWindow("tiz");
	init();
	glutDisplayFunc(display);

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(1, keyOperations, 0);

	glutMainLoop();
	return 0;
}
#include <GL/glut.h>					// (or others, depending on the system in use)
#include <bevgrafmath2017.h>
#include <math.h>

#include <iostream>


GLfloat winWidth = 800.0f, winHeight = 600.0f;

/**
* forgatási mátrixok
*/
mat4 Rx, Ry;

/**
* merõleges és centrális vetítések mátrixai
*/
mat4 Vo;

/**
* Wtv mátrixok
*/
mat4 Wo;

/**
* a fenti mátrixokból elõállított két transzformációs mátrix
*/
mat4 To;

/**
* X és Y tengely körüli forgatások
*/
GLfloat alphaX = 0.0f, alphaY = 0.0f, deltaAlpha = degToRad(1.0f);

/**
* nézetek koordinátái
*/
GLfloat oX = 150.0f, oY = 225.0f, oW = 300.0f, oH = 300.0f;


/**
* elõállítja a szükséges mátrixokat
*/
void initTransformations()
{
	// forgatási mátrixok
	Rx = rotateX(alphaX);
	Ry = rotateY(alphaY);

	// vetítési mátrixok
	Vo = ortho();

	// Wtv mátrixok
	Wo = windowToViewport3(vec2(-2*pi(), -2*pi()), vec2(4*pi(), 4*pi()), vec2(oX, oY), vec2(oW, oH));

	// merõleges
	To = Wo * Vo * Rx * Ry;
}

/*======================================*/

void init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, winWidth, 0.0f, winHeight, 0.0f, 1.0f);

	initTransformations();
}

void drawSphere(vec3 color, mat4 T)
{
	// beállítja a gömb éleinek vastagságát
	glLineWidth(2.0f);

	// beállítja a kocka színét
	glColor3f(color.x, color.y, color.z);

	//hosszusagi korok
	for (double u = 0; u <= 5; u += 0.2)
	{
		glBegin(GL_LINE_STRIP);
		for (double theta = -2 * pi(); theta <= 2 * pi(); theta += 0.1)
		{
			vec3 pih = vec3( u * cos( 1 * theta) , u * sin( 1 * theta) , theta);
			vec4 ph = ihToH(pih);
			vec4 pt = T * ph;
			if (pt.w != 0)
			{
				vec3 ptih = hToIh(pt);
				glVertex2f(ptih.x, ptih.y);
			}
		}
		glEnd();
	}

	//szelessegi korok
	for (double theta = -2*pi(); theta <= 2*pi() ; theta += 0.1)
	{
		glBegin(GL_LINE_STRIP);
		for (double u = 0; u <= 5; u += 0.2)
		{
			vec3 pih = vec3( u * cos( 1 * theta) , u * sin( 1 * theta) , theta);
			vec4 ph = ihToH(pih);
			vec4 pt = T * ph;
			if (pt.w != 0)
			{
				vec3 ptih = hToIh(pt);
				glVertex2f(ptih.x, ptih.y);
			}
		}
		glEnd();
	}
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// kirajzolja a merõlegesen vetített gömböt
	drawSphere(vec3(1.0f, 0.0f, 0.0f), To);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:      exit(0);               break;
	case 'a':     alphaY -= deltaAlpha;  break;
	case 'd':     alphaY += deltaAlpha;  break;
	case 'w':     alphaX -= deltaAlpha;  break;
	case 's':     alphaX += deltaAlpha;  break;
	}

	initTransformations();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowPosition(50, 100);   // Set top-left display-window position.
	glutInitWindowSize(winWidth, winHeight);      // Set display-window width and height.
	glutCreateWindow("Wonderful Sphere"); // Create display window.

	init();                         // Execute initialization procedure.
	glutDisplayFunc(draw);       // Send graphics to display window.
	glutKeyboardFunc(keyboard);
	glutMainLoop();                 // Display everything and wait.
	return 0;
}

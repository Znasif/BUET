#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double arm1, arm11, arm2, arm3, arm41, arm42;

struct point
{
	double x, y, z;
};


void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES); {
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, -100, 0);
			glVertex3f(0, 100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}glEnd();
	}
}


void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

	case '1':
		if (arm1 < -45) { ; }
		else arm1 -= 5;
		break;
	case '2':
		if (arm1 > 45) { ; }
		else arm1 += 5;
		break;
	case '3':
		if (arm2 <= -90) { ; }
		else arm2 -= 5;
		break;
	case '4':
		if (arm2 >= 0) { ; }
		else arm2 += 5;
		break;
	case '5':
		if (arm3 < -45) { ; }
		else arm3 -= 5;
		break;
	case '6':
		if (arm3 > 45) { ; }
		else arm3 += 5;
		break;
	case '7':
		if (arm41 > 45) { ; }
		else arm41 += 5;
		break;
	case '8':
		if (arm41 < -45) { ; }
		else arm41 -= 5;
		break;
	case '9':
		if (arm42 <= -90) { ; }
		else arm42 -= 5;
		break;
	case '0':
		if (arm42 >= 0) { ; }
		else arm42 += 5;
		break;
	case 'q':
		if (arm11 >= 90) { ; }
		else arm11 += 5;
		break;
	case 'w':
		if (arm11 <= 0) { ; }
		else arm11 -= 5;
		break;
	default:
		break;
	}
}


void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:		//down arrow key
		cameraHeight -= 3.0;
		break;
	case GLUT_KEY_UP:		// up arrow key
		cameraHeight += 3.0;
		break;

	case GLUT_KEY_RIGHT:
		cameraAngle += 0.03;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= 0.03;
		break;

	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		break;
	case GLUT_KEY_END:
		break;

	default:
		break;
	}
}


void mouseListener(int button, int state, int x, int y) {	//x, y is the x-y of the screen (2D)
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {		// 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//........
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}



void display() {

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(300 * cos(cameraAngle), 300 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);
	//gluLookAt(200, 200, 200, 0, 0, 0, 0, 0, 1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

	//glColor3f(1,0,0);
	//drawSquare(10);

	//drawSS();
	drawAxes();
	glRotatef(arm1, 0, 1, 0);
	glRotatef(arm11, 1, 0, 0);
	glPushMatrix();
	{
		glTranslatef(0, 0, -50);
		glScalef(1, 1, 5);
		glutWireSphere(10, 10, 10);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0, 0, -100);
		glRotatef(arm2, 0, 1, 0);
		glTranslatef(0, 0, -24);
		glScalef(1, 1, 3);
		glutWireSphere(8, 10, 10);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0, 0, -100);
		glRotatef(arm2, 0, 1, 0);
		glTranslatef(0, 0, -47);
		glRotatef(arm3, 0, 0, 1);
		glBegin(GL_TRIANGLES); {
			glVertex3f(0, 0, 0);
			glVertex3f(0, 10, -10);
			glVertex3f(0, -10, -10);
		}glEnd();
	}
	glPopMatrix();
	glTranslatef(0, 0, -100);
	glRotatef(arm2, 0, 1, 0);
	glTranslatef(0, 0, -48);
	glRotatef(arm3, 0, 0, 1);
	glPushMatrix();
	{
		glTranslatef(0, 10, -8);
		glRotatef(arm41, 0, 0, 1);
		glTranslatef(0, 0, -16);
		glScalef(1, 1, 3);
		glutWireSphere(5, 10, 10);
	}
	glPopMatrix();
	glTranslatef(0, -10, -8);
	glRotatef(arm42, 0, 1, 0);
	glTranslatef(0, 0, -16);
	glScalef(1, 1, 3);
	glutWireSphere(5, 10, 10);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate() {
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init() {
	//codes for initialization
	drawgrid = 0;
	drawaxes = 1;
	cameraHeight = 150.0;
	cameraAngle = 1.0;

	//clear the screen
	glClearColor(0, 0, 0, 0);
	arm1 = 0.0;
	arm11 = 0.0;
	arm2 = 0.0;
	arm3 = 0.0;
	arm41 = 0.0;
	arm42 = 0.0;
	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

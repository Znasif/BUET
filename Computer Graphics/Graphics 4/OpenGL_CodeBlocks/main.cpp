#include "FILE2.cpp"

#define pi (2*acos(0.0))



int drawgrid;
int drawaxes;
double angle;
double transform;
double move = 10;

struct point
{
	double x, y, z;
}pos, l, r, u;


void change(int home) {
	if (transform > 1 && home == 1) transform = transform;
	else if (transform <= 0 && home == -1) transform = transform;
	else {
		transform += 0.1*home;
	}
}

void tilt(int axis, double angle) {
	double f1 = cos(angle);
	double f2 = sin(angle);
	if (axis == 0) {
		r.x *= f1;
		r.x += u.x*f2;

		r.y *= f1;
		r.y += u.y*f2;

		r.z *= f1;
		r.z += u.z*f2;

		u.x = -l.y*r.z + l.z*r.y;
		u.y = -l.z*r.x + l.x*r.z;
		u.z = -l.x*r.y + l.y*r.x;
	}
	else if (axis == 1) {
		l.x *= f1;
		l.x += u.x*f2;

		l.y *= f1;
		l.y += u.y*f2;

		l.z *= f1;
		l.z += u.z*f2;

		u.x = -l.y*r.z + l.z*r.y;
		u.y = -l.z*r.x + l.x*r.z;
		u.z = -l.x*r.y + l.y*r.x;
	}
	else if (axis == 2) {
		r.x *= f1;
		r.x += l.x*f2;

		r.y *= f1;
		r.y += l.y*f2;

		r.z *= f1;
		r.z += l.z*f2;

		l.x = -r.y*u.z + r.z*u.y;
		l.y = -r.z*u.x + r.x*u.z;
		l.z = -r.x*u.y + r.y*u.x;
	}
	/*
	printf("l : %lf %lf %lf", l.x, l.y, l.z);
	printf("r : %lf %lf %lf", r.x, r.y, r.z);
	printf("u : %lf %lf %lf\n", u.x, u.y, u.z);
	*/
}


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


void drawGrid(point tl)
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES); {
			for (i = -8; i <= 8; i++) {

				if (i == 0)
					continue;	//SKIP the MAIN axes

								//lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS); {
		glVertex3f(a, a, 2);
		glVertex3f(a, -a, 2);
		glVertex3f(-a, -a, 2);
		glVertex3f(-a, a, 2);
	}glEnd();
}


void drawCircle(double radius, int segments)
{
	int i;
	struct point points[100];
	glColor3f(0.7, 0.7, 0.7);
	//generate points
	for (i = 0; i <= segments; i++)
	{
		points[i].x = radius * cos(((double)i / (double)segments) * 2 * pi);
		points[i].y = radius * sin(((double)i / (double)segments) * 2 * pi);
	}
	//draw segments using generated points
	for (i = 0; i<segments; i++)
	{
		glBegin(GL_LINES);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}



void drawSphere(double radius, int slices, int stacks)
{
	struct point points[100][100];
	int i, j;
	double h, r;
	//generate points
	for (i = 0; i <= stacks; i++)
	{
		h = radius * sin(((double)i / (double)stacks)*(pi / 2));
		r = radius * cos(((double)i / (double)stacks)*(pi / 2));
		for (j = 0; j <= slices; j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0; i<stacks; i++)
	{
		glColor3f((double)i / (double)stacks, (double)i / (double)stacks, (double)i / (double)stacks);
		for (j = 0; j<slices; j++)
		{
			glBegin(GL_QUADS); {
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				//lower hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);

			}glEnd();
		}
	}
}

void drawCube()
{
	glPushMatrix();
	{
		glTranslatef(0, 0, -20);
		drawSquare(20 * transform);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0, 0, 20);
		drawSquare(20 * transform);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, 0, -20);
		drawSquare(20 * transform);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, 0, -20);
		drawSquare(20 * transform);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0, 0, -20);
		drawSquare(20 * transform);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, -20);
		drawSquare(20 * transform);
	}
	glPopMatrix();
}


void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

	case '1':
		tilt(2, pi / 60);
		break;
	case '2':
		tilt(2, -pi / 60);
		break;
	case '3':
		tilt(1, pi / 60);
		break;
	case '4':
		tilt(1, -pi / 60);
		break;
	case '5':
		tilt(0, pi / 60);
		break;
	case '6':
		tilt(0, -pi / 60);
		break;
	default:
		break;
	}
}


void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		pos.x -= move * l.x;
		pos.y -= move * l.y;
		pos.z -= move * l.z;
		break;
	case GLUT_KEY_UP:
		pos.x += move * l.x;
		pos.y += move * l.y;
		pos.z += move * l.z;
		break;

	case GLUT_KEY_RIGHT:
		pos.x += move * r.x;
		pos.y += move * r.y;
		pos.z += move * r.z;
		break;
	case GLUT_KEY_LEFT:
		pos.x -= move * r.x;
		pos.y -= move * r.y;
		pos.z -= move * r.z;
		break;

	case GLUT_KEY_PAGE_UP:
		pos.x += move * u.x;
		pos.y += move * u.y;
		pos.z += move * u.z;
		break;
	case GLUT_KEY_PAGE_DOWN:
		pos.x -= move * u.x;
		pos.y -= move * u.y;
		pos.z -= move * u.z;
		break;

	case GLUT_KEY_INSERT:
	    drawgrid = 1 - drawgrid;
		break;

	case GLUT_KEY_HOME:
		change(1); //cube to sphere
		break;
	case GLUT_KEY_END:
		change(-1);
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	//glColor3f(1,0,0);
	//drawSquare(10);
	drawCube();
	//drawSS();

	//drawCircle(30,24);

	//drawCone(20,50,24);

	//drawSphere(30, 24, 20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate() {
	angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init() {
	//codes for initialization
	drawgrid = 0;
	drawaxes = 1;

	transform = 1; // middle ground
	angle = 0;

	pos.x = 100;
	pos.y = 100;
	pos.z = 30;

	l.x = -1 / sqrt(2);
	l.y = -1 / sqrt(2);
	l.z = 0;

	r.x = -1 / sqrt(2);
	r.y = 1 / sqrt(2);
	r.z = 0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	//clear the screen
	glClearColor(0, 0, 0, 0);

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

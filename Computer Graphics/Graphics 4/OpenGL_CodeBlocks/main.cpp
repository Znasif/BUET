#include "FILE2.cpp"

#define Window_width 500
#define Window_height 500

#define pi (2*acos(0.0))


int drawgrid;
int drawaxes;
double angle;
double fovy;
int Screen_Width, Screen_Height;
double mv = 10;


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
		glBegin(GL_LINES); {
		    glColor3f(0, 0, 1.0);
			glVertex3f(300, 0, 0);
			glVertex3f(-300, 0, 0);

            glColor3f(0, 1.0, 0);
			glVertex3f(0, -300, 0);
			glVertex3f(0, 300, 0);

            glColor3f(1.0, 0, 0);
			glVertex3f(0, 0, 300);
			glVertex3f(0, 0, -300);
		}glEnd();
	}
}

void loadActualData(){
    Object *temp;
    freopen("scene.txt","r",stdin);
    cin>>Recursion_level>>Screen_Width;
	Screen_Height = Screen_Width;
	int n;
	cin>>n;
	string l;
    for(int i=0; i<n; i++){
        cin>>l;
        double a, b, c, d;
        int j;
        if(l == "general"){
            double m[10];
            double clip[6];
            for(int k=0; k<10; k++) cin>>m[k];
            for(int k=0; k<6; k++) cin>>clip[k];
            cin>>a>>b>>c;
            cin>>a>>b>>c>>d;
            cin>>j;
            continue;
        }
        else if(l == "sphere"){
            cin>>a>>b>>c;
            cin>>d;
            temp = new Sphere(Point3(a, b, c), d);
        }
        else if(l == "triangle"){
            cin>>a>>b>>c;
            Point3 p(a, b, c);
            cin>>a>>b>>c;
            Point3 q(a, b, c);
            cin>>a>>b>>c;
            Point3 r(a, b, c);
            temp = new Triangle(p, q, r);
        }
        cin>>a>>b>>c;
        temp->setColor(a, b, c);
        cin>>a>>b>>c>>d;
        temp->setCoEfficients(a, b, c, d);
        cin>>j;
        temp->setShine(j);
        objects.push_back(temp);
    }
    cin>>n;
    for(int i=0; i<n; i++){
        double a, b, c;
        cin>>a>>b>>c;
        lights.push_back(Point3(a, b, c));
    }

    temp=new Floor(1000, 20);
    temp->setCoEfficients(0.4, 0.2, 0.2, 0.2);
    temp->setShine(1);
    objects.push_back(temp);
}
void loadTestData(){
    Object *temp;
    temp=new Sphere(Point3(0, 0, 10), 10); // Center(0,0,10), Radius 10
    temp->setColor(1,0,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);
    srand(33);
    for(int i=0; i<6; i++){
        int a[4];
        double b[4];
        for(int j=0; j<4; j++){
            b[j] =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if(j<3){
                a[j] = (b[j])*51;
            }
            else{
                a[j] = 10;
            }
            if(a[j]%2 && j<2){
                a[j] = -a[j];
            }
        }

        temp=new Sphere(Point3(a[0], a[1], a[2]), a[3]); // Center(0,0,10), Radius 10
        temp->setColor(b[0], b[1], b[2]);
        temp->setCoEfficients(0.4,0.2,0.2,0.2);
        temp->setShine(1);
        objects.push_back(temp);
    }

    Point3 light1(-50,50,50);
    lights.push_back(light1);

    temp = new Triangle(Point3(30, 60, 0), Point3(50, 30, 0), Point3(50, 45, 50));
    temp->setColor(1, 0, 0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);

    temp = new Triangle(Point3(70, 60, 0), Point3(30, 60, 0), Point3(50, 45, 50));
    temp->setColor(1, 0, 0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);

    temp = new Triangle(Point3(50, 30, 0), Point3(70, 60, 0), Point3(50, 45, 50));
    temp->setColor(1, 0, 0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);

    temp=new Floor(1000, 20);
    temp->setCoEfficients(0.4, 0.2, 0.2, 0.2);
    temp->setShine(1);
    objects.push_back(temp);

    Screen_Width = 768;
	Screen_Height = 768;
    Recursion_level = 3;
}

void Capture(){
    bitmap_image image(Screen_Width, Screen_Height);
    for(int i=0;i<Screen_Height;i++){
        for(int j=0;j<Screen_Width;j++){
            image.set_pixel(j, i, 0, 0, 0);
        }
    }
    double plane_distance = (Window_height/2.0)/tan(fovy*pi/360.0);
    Point3 topLeft = pos + l*plane_distance - r*(Window_width/2) + u*(Window_height/2);
    double dv = Window_height*1.0/Screen_Height;
    double du = Window_width*1.0/Screen_Width;
    for(int i=0; i<Screen_Height; i++){
        Point3 corner;
        for(int j=0; j<Screen_Width; j++){
                corner = topLeft + r*(j*du) - u*(i*dv);
                Ray ray(pos, corner - pos);
                double* addn = new double[3];
                int nearest = getColor(ray, addn, 0);
                if(nearest !=-1) image.set_pixel(j, i, addn[0]*255, addn[1]*255, addn[2]*255);
            }
        }
    printf("DONE");
    image.save_image("out.bmp");
}

void keyboardListener(unsigned char key, int x, int y) {
	switch (key) {

    case '0':
		Capture();
		break;
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
		pos.x -= mv * l.x;
		pos.y -= mv * l.y;
		pos.z -= mv * l.z;
		break;
	case GLUT_KEY_UP:
		pos.x += mv * l.x;
		pos.y += mv * l.y;
		pos.z += mv * l.z;
		break;
	case GLUT_KEY_RIGHT:
		pos.x += mv * r.x;
		pos.y += mv * r.y;
		pos.z += mv * r.z;
		break;
	case GLUT_KEY_LEFT:
		pos.x -= mv * r.x;
		pos.y -= mv * r.y;
		pos.z -= mv * r.z;
		break;
	case GLUT_KEY_PAGE_UP:
		pos.x += mv * u.x;
		pos.y += mv * u.y;
		pos.z += mv * u.z;
		break;
	case GLUT_KEY_PAGE_DOWN:
		pos.x -= mv * u.x;
		pos.y -= mv * u.y;
		pos.z -= mv * u.z;
		break;
	case GLUT_KEY_INSERT:
	    drawgrid = 1 - drawgrid;
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

    for(int i=0; i<objects.size(); i++){
        objects[i]->draw();
    }
    glPointSize(3);
    for(int i=0; i<lights.size(); i++){
        glColor3f(1.0, 1.0, 0);
        glBegin(GL_POINTS);
        {
            glVertex3f(lights[i].x, lights[i].y, lights[i].z);
        }
        glEnd();
    }

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
	angle = 0;

	pos = Point3(0, -200, 10);
	l = Point3(0, 200/sqrt(40100), -10/sqrt(40100));
	r = Point3(1, 0, 0);
	u = Point3(0, 0, 1);

	/*pos = Point3(100, 100, 10);
    l = Point3(-1/sqrt(2), -1/sqrt(2), 0);
    r = Point3(-1/sqrt(2), 1/sqrt(2), 0);
    u = Point3(0, 0, 1);*/

	fovy = 80;

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
	gluPerspective(fovy, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(Window_width, Window_height);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

    //loadTestData();
    loadActualData();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

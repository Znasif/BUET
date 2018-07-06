#include "1305030_kd_tree.cpp"
using namespace std;

double cameraHeight;
double cameraAngle;

#define pi (2*acos(0.0))

int drawaxes,draw1=1,draw2=1;

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 500,0,0);
            glVertex3f(-500,0,0);

            glVertex3f(0,-500,0);
            glVertex3f(0, 500,0);

            //glVertex3f(0,0, 500);
            //glVertex3f(0,0,-500);
        }
        glEnd();
    }
}

int step = -1;

void makeQuery(){
    if(Q[step].type) return;
    Q[step].print();
    cout<<"Result : \n";
    tr->searchRange(tr->root, plane, Q[step], 0);
    cout<<endl<<tr->members<<endl;
    tr->members = 0;
}

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:		//down arrow key
        cameraHeight -= 3.0;
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        cameraHeight += 3.0;
        break;
    case GLUT_KEY_RIGHT:
        step += 1;
        step = step%q;
        makeQuery();
        break;
    case GLUT_KEY_LEFT:
        step -= 1;
        if(step<0) step = q-1;
        makeQuery();
        break;
    case GLUT_KEY_PAGE_UP:
        break;
    case GLUT_KEY_PAGE_DOWN:
        break;
    }
}

void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
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
    gluLookAt(25,25,cameraHeight,	25,25,0,	0,1,0);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    glScalef(10,10,1);

    for(int i=0; i<n; i++)       //print
    {
        glColor3f(1.0, 0.0, 0);
        glBegin(GL_POINTS);
        {
            glVertex3f(A[i].x,A[i].y,0);
        }
        glEnd();
    }

    drawAxes();

    glPointSize(3);
    for(int i=0; i<q; i++){
        glColor3f(1.0, 1.0, 0.0);
        if(Q[i].type==0 && step==i) glRectd(Q[i].a.x, Q[i].a.y, Q[i].b.x, Q[i].b.y);
    }

    //drawAxes();



    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawaxes=1;
    cameraHeight=160.0;
	cameraAngle=1.0;


    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    input();

    glutInit(&argc,argv);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();



}

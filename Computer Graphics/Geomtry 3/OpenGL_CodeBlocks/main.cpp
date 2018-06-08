#include<vector>
#include<map>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<windows.h>
#include<algorithm>
#include<time.h>
#include<glut.h>

using namespace std;

typedef pair<int, int> edge;
typedef tuple<int, int, int> three;
#define pi (2*acos(0.0))
ifstream fin("1305030_points.txt");

int V;

struct point
{
    double x,y;
    void print(){
        cout<<" X : "<< x << " Y : "<< y << '\n';
    }
    void set_point(double a, double b){
        x = a;
        y = b;
    }
};


bool compareByY(const point &a, const point &b)
{
    return (a.y > b.y) || ((a.y==b.y) && (a.x>b.x));
}

bool lexiCo(const point &a, const point &b)
{
    return (a.y > b.y) || ((a.y==b.y) && (a.x<b.x));
}

vector<point> graph;
map<edge, edge> corners;

bool cross(point p, point q, point r){
    return (q.x-p.x)*(r.y-p.y) >= (q.y-p.y)*(r.x-p.x);
}

bool on_edge(point p, point q, point r){
    return (q.x-p.x)*(r.y-p.y) == (q.y-p.y)*(r.x-p.x);
}

bool inside_circle(int l, int m, int n, int o){
    point a, b, c, d;
    cout<<l<<m<<n;
    if(l<0 || m<0 || n<0 || o<0){
        return min(l, m) < min(n, o);
    }
    a = graph[l];
    b = graph[m];
    c = graph[n];
    d = graph[o];
    point p;
    cout<<"Check inside Circle : "<<o<<endl;
    double D = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
    p.x = (((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.y - c.y)
    -  ((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.y - c.y)) / D;

    p.y = (((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.x - c.x)
    -  ((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.x - c.x)) / D;

    return (p.x-d.x)*(p.x-d.x) + (p.y-d.y)*(p.y-d.y) < (p.x-a.x)*(p.x-a.x) + (p.y-a.y)*(p.y-a.y);
}

map<three, int> search_triangle;

struct triangles
{
    int a, b, c;
    vector<int> child;

    bool check_in(point np){
        bool l = cross(graph[a], graph[b], np);
        bool m = cross(graph[b], graph[c], np);
        bool n = cross(graph[c], graph[a], np);
        return l==m && m==n;
    }

    edge check_on(point np){
        edge ret;
        ret = make_pair(-1, -1);
        if(on_edge(graph[a], graph[b], np)) ret = make_pair(a, b);
        if(on_edge(graph[b], graph[c], np)) ret = make_pair(b, c);
        if(on_edge(graph[c], graph[a], np)) ret = make_pair(a, c);
        return ret;
    }

    void set_triangle(int p, int q, int r, int idx){
        vector<int> temp;
        temp.push_back(p);temp.push_back(q);temp.push_back(r);
        sort(temp.begin(), temp.end());

        a = temp[0]; b = temp[1]; c = temp[2];

        search_triangle[make_tuple(a, b, c)]=idx;
    }

    void set_child(int idx){
        child.push_back(idx);
    }

    void print(){
        cout<<"***** Triangle ******"<<endl;
        if(a<0) cout<<"Symbolic "<<a<<endl;
        else graph[a].print();
        if(b<0) cout<<"Symbolic "<<b<<endl;
        else graph[b].print();
        if(c<0) cout<<"Symbolic "<<c<<endl;
        else graph[c].print();
        cout<<"*********************"<<endl;
    }
};

vector<triangles> DeCheck;

three get_tuple(int a, int b, int c){
    vector<int> temp;
    temp.push_back(a);temp.push_back(b);temp.push_back(c);
    sort(temp.begin(), temp.end());

    return make_tuple(temp[0], temp[1], temp[2]);
}

int inside_triangle(int p){
    int sz = DeCheck[0].child.size();
    int r = 0, q;
    while(sz>0){
        for(int i=0; i<sz; i++){
            q = DeCheck[r].child[i];
            if(DeCheck[q].check_in(graph[p])){
                sz = DeCheck[q].child.size();
                r = q;
                break;
            }
        }
        if(DeCheck[r].child.size()==0) return r;
    }
    cout<<"Inside First Triangle"<<endl;
    return 0;
}

void legalize_edge(int p, int q, edge l){
    if(l.first>l.second) l = make_pair(l.second, l.first);
    edge s = corners[l];
    cout<<s.first<<" "<<s.second<<endl;
    edge r;
    if(s.first==q) r = s;
    else if(s.second==q) r = make_pair(s.second, s.first);
    else return;
    if(inside_circle(l.first, l.second, r.second, p)){
        cout<<"legalize : "<<p<<endl;
        int a, b, c, d;
        a = search_triangle[get_tuple(p, l.first, l.second)];
        b = search_triangle[get_tuple(r.second, l.first, l.second)];
        triangles t1, t2;
        c = DeCheck.size();
        t1.set_triangle(p, r.second, l.first, c);
        DeCheck.push_back(t1);
        d = DeCheck.size();
        t2.set_triangle(p, r.second, l.second, d);
        DeCheck.push_back(t2);
        DeCheck[a].set_child(c);
        DeCheck[a].set_child(d);
        DeCheck[b].set_child(c);
        DeCheck[b].set_child(d);
        corners[make_pair(p, r.second)] = l;
        legalize_edge(p, l.second, make_pair(r.second, l.first));
        legalize_edge(p, l.first, make_pair(r.second, l.second));
    }
}

void test(int show=1){
    for(int i=0; i<V; i++)
    {
        if(show){
            graph[i].print();

            triangles tr;
            point np;
            np.set_point(2.5, 5);
            tr.set_triangle(i%V, (i+1)%V, (i+2)%V, -1);
            edge n = tr.check_on(np);
            cout<<"Inside Triangle : "<<n.first<<" "<<n.second<<endl;

            bool m = inside_circle(i%V, (i+1)%V, (i+2)%V, (i+3)%V);
            cout<<"Inside Circle : "<<m<<endl;
        }
    }
}

edge get_pair(edge e){
    if(e.first>e.second) return make_pair(e.second, e.first);
    return e;
}

void set_corners(int a, int b, int c, int d){
    if(a>b) swap(a, b);
    if(c>d) swap(c, d);

    corners[make_pair(a, b)] = make_pair(c, d);
}

void fix_corners(int p_1, int q, int p_2, int i){
    edge e = corners[get_pair(make_pair(p_1, q))];
    if(e.first==p_2) set_corners(p_1, q, i, e.second);
    else set_corners(p_1, q, i, e.first);
}

void delaunay(){
    triangles temp;
    temp.set_triangle(-2, -1, 0, 0);
    DeCheck.push_back(temp);

    set_corners(0, -1, -2, -3);
    set_corners(0, -2, -1, -3);
    set_corners(-1, -2, 0, -3);

    int r;

    for(int i=1; i<V; i++){
        r = inside_triangle(i);
        edge p = DeCheck[r].check_on(graph[i]);

        if(make_pair(-1, -1) != p){
            graph[i].print();
            cout<<"point is on an edge of "<<endl;
            DeCheck[r].print();
            triangles l, m, n, o;
            int j = DeCheck.size(), k;
            edge q = corners[p];
            int b = q.first;
            if(!(b == DeCheck[r].a || b == DeCheck[r].b || b == DeCheck[r].c)) q = make_pair(q.second, q.first);

            cout<<"Setting triangles"<<endl;

            l.set_triangle(p.first, q.first, i, j);
            DeCheck.push_back(l);
            DeCheck[r].set_child(j);

            m.set_triangle(p.second, q.first, i, j+1);
            DeCheck.push_back(m);
            DeCheck[r].set_child(j+1);

            k = search_triangle[get_tuple(p.first, p.second, q.second)];

            n.set_triangle(p.first, q.second, i, j+2);
            DeCheck.push_back(n);
            DeCheck[k].set_child(j+2);

            o.set_triangle(p.second, q.second, i, j+3);
            DeCheck.push_back(o);
            DeCheck[k].set_child(j+3);

            fix_corners(p.first, q.first, p.second, i);
            fix_corners(q.first, p.second, p.first, i);
            fix_corners(p.second, q.second, p.first, i);
            fix_corners(p.first, q.second, p.second, i);

            set_corners(p.first, i, q.first, q.second);
            set_corners(q.second, i, p.first, p.second);
            set_corners(p.second, i, q.first, q.second);
            set_corners(q.first, i, p.first, p.second);

            legalize_edge(i, p.second, make_pair(p.first, q.first));
            legalize_edge(i, p.first, make_pair(p.second, q.first));
            legalize_edge(i, p.second, make_pair(p.first, q.second));
            legalize_edge(i, p.first, make_pair(p.second, q.second));
        }
        else{
            graph[i].print();
            cout<<"point is strictly interior to"<<endl;
            DeCheck[r].print();
            triangles l, m, n;
            int j = DeCheck.size();
            int a = DeCheck[r].a, b = DeCheck[r].b, c = DeCheck[r].c;

            cout<<"Setting triangles"<<endl;

            l.set_triangle(a, b, i, j);
            DeCheck.push_back(l);
            DeCheck[r].set_child(j);

            m.set_triangle(b, c, i, j+1);
            DeCheck.push_back(m);
            DeCheck[r].set_child(j+1);

            n.set_triangle(c, a, i, j+2);
            DeCheck.push_back(n);
            DeCheck[r].set_child(j+2);

            fix_corners(a, b, c, i);
            fix_corners(b, c, a, i);
            fix_corners(a, c, b, i);

            set_corners(a, i, b, c);
            set_corners(b, i, a, c);
            set_corners(c, i, a, b);

            legalize_edge(i, c, make_pair(a, b));
            legalize_edge(i, a, make_pair(b, c));
            legalize_edge(i, b, make_pair(c, a));
        }
    }
}


void input(int show=1)
{
    double a, b;
    point np;
    fin>>V;
    graph.clear();
    point mn, mx;
    int cnt;
    mn.set_point(INFINITY, INFINITY);
    mx.set_point(-INFINITY, -INFINITY);
    for(int i=0; i<V; i++)
    {
        fin>>a>>b;
        np.set_point(a, b);
        graph.push_back(np);
        if(compareByY(mn, graph[i])) mn = graph[i];
        if(compareByY(graph[i], mx)){
            mx = graph[i];
            cnt = i;
        }
    }
    graph[cnt]=graph[0];
    graph[0]=mx;
    srand(time(0));
    random_shuffle(graph.begin()+1, graph.end());
    //test();
    //delaunay();
}

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

            glVertex3f(0,0, 500);
            glVertex3f(0,0,-500);
        }
        glEnd();
    }
}

void drawTriangle(int tri)
{
    glColor3f(1.0, 0, 1.0);
    point a, b, c;

    if(DeCheck[tri].a<=-1 || DeCheck[tri].b<=-1 || DeCheck[tri].c<=-1) return;

    a = graph[DeCheck[tri].a];
    b = graph[DeCheck[tri].b];
    c = graph[DeCheck[tri].c];

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a.x, a.y, 0);
        glVertex3f(b.x, b.y, 0);
        glVertex3f(c.x, c.y, 0);
    }
    glEnd();
}

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {

    case GLUT_KEY_RIGHT:

        draw1=1-draw1;
        break;
    case GLUT_KEY_LEFT:
        draw2=1-draw2;

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
    gluLookAt(0,0,200,	0,0,0,	0,1,0);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects


    glScalef(10,10,1);
    glPointSize(3);
    for(int i=0; i<V; i++)       //print
    {
        point tr=graph[i];
        glColor3f(1.0, 1.0, 0);
        glBegin(GL_POINTS);
        {
            glVertex3f(tr.x,tr.y,0);
        }
        glEnd();

    }

    drawAxes();



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

    delaunay();

    glutMainLoop();



}

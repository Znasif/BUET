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

double cameraHeight;
double cameraAngle;

#define pi (2*acos(0.0))
#define dbg(s) cout<<s<<endl
#define fout if(false) cout

ifstream fin("1305030_points.txt");
//ofstream fout("1305030_debug.txt");

int V;

struct point
{
    double x,y;
    void print(){
        fout<<" X : "<< x << " Y : "<< y << '\n';
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
    return (a.y > b.y) || ((a.y==b.y) && (a.x>b.x));
}

vector<point> graph;
map<edge, edge> corners;
map<three, int> search_triangle;

three get_tuple(int a, int b, int c){
    vector<int> temp;
    temp.push_back(a);temp.push_back(b);temp.push_back(c);
    sort(temp.begin(), temp.end());

    return make_tuple(temp[0], temp[1], temp[2]);
}

bool cross(point p, point q, point r){
    return (q.x-p.x)*(r.y-p.y) >= (q.y-p.y)*(r.x-p.x);
}

bool cross(int p, int q, int np){
    if(p<1 && q<1) return true;
    else if(np>=0 && p==-2 && q>0) return lexiCo(graph[np], graph[q]);
    else if(np==-1 && p==-2 && q>0) return false;
    else if(np>=0 && p==-1 && q>0) return lexiCo(graph[q], graph[np]);
    else if(np==-2 && p==-1 && q>0) return true;
    else if(np>=0 && q==-2 && p>0) return lexiCo(graph[p], graph[np]);
    else if(np==-1 && q==-2 && p>0) return true;
    else if(np>=0 && q==-1 && p>0) return lexiCo(graph[np], graph[p]);
    else if(np==-2 && q==-1 && p>0) return false;
    else if(np<0) return cross(np, p, q);
    return cross(graph[p], graph[q], graph[np]);
}

bool on_edge(point p, point q, point r){
    return (q.x-p.x)*(r.y-p.y) == (q.y-p.y)*(r.x-p.x);
}

bool inside_circle(int l, int m, int n, int o){
    point a, b, c, d;
    fout<<"Inside Circle Check : "<<l<<" "<<m<<" "<<n<<endl;

    if(l<0 || m<0 || n<0 || o<0){
        bool flag = cross(l, n, m);
        flag = (flag==cross(n, m, o) && flag==cross(m, o, l) && flag==cross(o, l, n));
        if(flag){
            if(min(l, m) < min(n, o)) fout<<"flip :"<<l<<" & "<<m<<" into -> "<<n<<" & "<<o<<endl;
            else fout<<"don't flip :"<<l<<" & "<<m<<" into -> "<<n<<" & "<<o<<endl;
            return min(l, m) < min(n, o);
        }
        else return false;
    }
    a = graph[l];
    b = graph[m];
    c = graph[n];
    d = graph[o];

    point p;
    fout<<"Check inside Circle : "<<o<<endl;
    double D = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
    p.x = (((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.y - c.y)
    -  ((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.y - c.y)) / D;

    p.y = (((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.x - c.x)
    -  ((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.x - c.x)) / D;

    return (p.x-d.x)*(p.x-d.x) + (p.y-d.y)*(p.y-d.y) < (p.x-a.x)*(p.x-a.x) + (p.y-a.y)*(p.y-a.y);
}

struct triangles
{
    int a, b, c;
    vector<int> child;
    int printed = 1;

    bool check_in(int np){
        bool flag[3];
        int arr[3];
        arr[0]=a;arr[1]=b;arr[2]=c;
        for(int i=0; i<3; i++){
            int j = i+1;
            if(j == 3) j = 0;
            flag[i] = cross(arr[i], arr[j], np);
        }
        return flag[0] && flag[1] && flag[2];
    }

    edge check_on(point np){
        edge ret;
        ret = make_pair(-1, -1);
        if(a>=0 && b>=0 && on_edge(graph[a], graph[b], np)) ret = make_pair(a, b);
        if(b>=0 && c>=0 && on_edge(graph[b], graph[c], np)) ret = make_pair(b, c);
        if(c>=0 && a>=0 && on_edge(graph[c], graph[a], np)) ret = make_pair(a, c);
        return ret;
    }

    bool check_valid(){
        if(printed && child.size()==0 && a>=0 && b>=0 && c>=0){
            fout<<"ooooooooo "<<a<<" "<<b<<" "<<c<<endl;
            printed = 0;
        }
        if(child.size()==0 && a>=0 && b>=0 && c>=0) return true;
        return false;
    }

    void set_triangle(int p, int q, int r, int idx){
        vector<int> temp;
        temp.push_back(p);temp.push_back(q);temp.push_back(r);
        sort(temp.begin(), temp.end());

        a = p; b = q; c = r;

        search_triangle[make_tuple(temp[0], temp[1], temp[2])]=idx;
    }

    void let_triangle(int p, int q, int r, int idx, int m, int n){
        a = p; b = q; c = r;
        repoint(m, n);
        set_triangle(a, b, c, idx);
    }

    void repoint(int p, int q){
        if(a == p) a = q;
        else if(b == p) b = q;
        else if(c == p) c = q;
    }

    void set_child(int idx){
        child.push_back(idx);
    }

    void print(){
        fout<<"***** Triangle ******"<<endl;
        if(a<0) fout<<"Symbolic "<<a<<endl;
        else graph[a].print();
        if(b<0) fout<<"Symbolic "<<b<<endl;
        else graph[b].print();
        if(c<0) fout<<"Symbolic "<<c<<endl;
        else graph[c].print();
        fout<<"*********************"<<endl;
    }
};

vector<triangles> DeCheck;

int inside_triangle(int p){
    int sz = DeCheck[0].child.size();
    int r = 0, q;
    while(sz>0){
        for(int i=0; i<sz; i++){
            q = DeCheck[r].child[i];
            if(DeCheck[q].check_in(p)){
                sz = DeCheck[q].child.size();
                r = q;
                break;
            }
        }
        if(DeCheck[r].child.size()==0) return r;
    }
    fout<<"Inside First Triangle"<<endl;
    return 0;
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
            fout<<"Inside Triangle : "<<n.first<<" "<<n.second<<endl;

            bool m = inside_circle(i%V, (i+1)%V, (i+2)%V, (i+3)%V);
            fout<<"Inside Circle : "<<m<<endl;
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

void legalize_edge(int p, edge l){
    edge r = corners[get_pair(l)];
    if(r.second == p) r = make_pair(r.second, r.first);
    fout<<l.first<<" * "<<l.second<<endl;
    if(r.second == -3) return;
    if(inside_circle(l.first, l.second, p, r.second)){
        fout<<"legalize : "<<r.second<<endl;
        int a, b, c, d;
        a = search_triangle[get_tuple(p, l.first, l.second)];
        b = search_triangle[get_tuple(r.second, l.first, l.second)];

        triangles t1, t2;
        c = DeCheck.size();
        t1.let_triangle(DeCheck[a].a, DeCheck[a].b, DeCheck[a].c, c, l.second, r.second);
        DeCheck.push_back(t1);
        d = DeCheck.size();
        t2.let_triangle(DeCheck[a].a, DeCheck[a].b, DeCheck[a].c, d, l.first, r.second);
        DeCheck.push_back(t2);

        DeCheck[a].set_child(c);
        DeCheck[a].set_child(d);
        DeCheck[b].set_child(c);
        DeCheck[b].set_child(d);

        fix_corners(l.first, p, l.second, r.second);
        fix_corners(l.second, p, l.first, r.second);
        fix_corners(l.first, r.second, l.second, p);
        fix_corners(l.second, r.second, l.first, p);

        set_corners(r.first, r.second, l.first, l.second);

        legalize_edge(p, make_pair(r.second, l.first));
        legalize_edge(p, make_pair(r.second, l.second));
    }
}

void delaunay(){
    triangles temp;
    temp.set_triangle(-2, -1, 0, 0);
    DeCheck.push_back(temp);

    set_corners(0, -1, -2, -3);
    set_corners(0, -2, -1, -3);
    set_corners(-1, -2, 0, -3);
}

void delaunay(int i){
    int r;
    r = inside_triangle(i);
    edge p = DeCheck[r].check_on(graph[i]);
    fout<<endl<<"-----------------"<<endl;
    graph[i].print();

    if(make_pair(-1, -1) != p){
        fout<<"point is on an edge of "<<endl;
        DeCheck[r].print();
        triangles l, m, n, o;
        int j = DeCheck.size(), k;
        edge q = corners[p];
        int b = q.first;
        if(!(b == DeCheck[r].a || b == DeCheck[r].b || b == DeCheck[r].c)) q = make_pair(q.second, q.first);

        fout<<"Setting triangles"<<endl;

        l.let_triangle(DeCheck[r].a, DeCheck[r].b, DeCheck[r].c, j, p.second, i);
        DeCheck.push_back(l);
        DeCheck[r].set_child(j);

        m.let_triangle(DeCheck[r].a, DeCheck[r].b, DeCheck[r].c, j+1, p.first, i);
        DeCheck.push_back(m);
        DeCheck[r].set_child(j+1);

        k = search_triangle[get_tuple(p.first, p.second, q.second)];

        n.let_triangle(DeCheck[k].a, DeCheck[k].b, DeCheck[k].c, j+2, p.second, i);
        DeCheck.push_back(n);
        DeCheck[k].set_child(j+2);

        o.let_triangle(DeCheck[k].a, DeCheck[k].b, DeCheck[k].c, j+3, p.first, i);
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

        legalize_edge(i, make_pair(p.first, q.first));
        legalize_edge(i, make_pair(p.second, q.first));
        legalize_edge(i, make_pair(p.first, q.second));
        legalize_edge(i, make_pair(p.second, q.second));
    }
    else{
        fout<<"point is strictly interior to"<<endl;
        DeCheck[r].print();
        triangles l, m, n;
        int j = DeCheck.size();
        int a = DeCheck[r].a, b = DeCheck[r].b, c = DeCheck[r].c;

        fout<<"Setting triangles"<<endl;

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

        legalize_edge(i, make_pair(a, b));
        legalize_edge(i, make_pair(b, c));
        legalize_edge(i, make_pair(c, a));
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
    delaunay();
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

void drawVEdge(int l, int m, int n, int o){
    point a, b, c;
    a = graph[l];
    b = graph[m];

    point p, q;
    double D;

    if(n>-1){
        c = graph[n];
        D = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
        p.x = (((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.y - c.y)
        -  ((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.y - c.y)) / D;

        p.y = (((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.x - c.x)
        -  ((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.x - c.x)) / D;
    }
    if(o>-1){
        c = graph[o];
        D = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
        q.x = (((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.y - c.y)
        -  ((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.y - c.y)) / D;

        q.y = (((b.x - c.x) * (b.x + c.x) + (b.y - c.y) * (b.y + c.y)) / 2 * (a.x - c.x)
        -  ((a.x - c.x) * (a.x + c.x) + (a.y - c.y) * (a.y + c.y)) / 2 * (b.x - c.x)) / D;
    }
    if(n<0){
        c = graph[o];
        bool flag = cross(a, b, q);
        p.x = (a.x + b.x)/2;
        p.y = (a.y + b.y)/2;
        if(!((flag == cross(b, c, q))&&(flag == cross(c, a, q)))){
            p.x = 2*q.x-p.x;
            p.y = 2*q.y-p.y;
        }
    }
    if(o<0){
        c = graph[n];
        bool flag = cross(a, b, p);
        q.x = (a.x + b.x)/2;
        q.y = (a.y + b.y)/2;
        if(!((flag == cross(b, c, p))&&(flag == cross(c, a, p)))){
            q.x = 2*p.x-q.x;
            q.y = 2*p.y-q.y;
        }
    }

    glBegin(GL_LINES);
    {
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(p.x,p.y,0);
        glVertex3f(q.x,q.y,0);
    }
    glEnd();
}

void drawTriangle(int tri)
{
    glColor3f(1.0, 0.0, 1.0);
    point a, b, c;

    a = graph[DeCheck[tri].a];
    b = graph[DeCheck[tri].b];
    c = graph[DeCheck[tri].c];
    glBegin(GL_LINES);
    {
        glVertex3f( a.x,a.y,0);
        glVertex3f( b.x,b.y,0);

        glVertex3f(b.x,b.y,0);
        glVertex3f(c.x,c.y,0);

        glVertex3f(c.x,c.y,0);
        glVertex3f(a.x,a.y,0);
    }/*
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a.x, a.y, 0);
        glVertex3f(b.x, b.y, 0);
        glVertex3f(c.x, c.y, 0);
    }*/
    glEnd();
}

int step=1, show_voronoi=0, show_delaunay = 1;

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
        if(step<V) delaunay(step++);
        break;
    case GLUT_KEY_LEFT:
        for(;step<V;step++) delaunay(step);
        break;
    case GLUT_KEY_PAGE_UP:
        if(step==V) show_voronoi = 1 - show_voronoi;
        break;
    case GLUT_KEY_PAGE_DOWN:
        if(step==V) show_delaunay = 1 - show_delaunay;
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

    int ln = DeCheck.size();
    for(int i=0; i<ln; i++){
        //DeCheck[i].print();
        if(DeCheck[i].check_valid()){
            if(show_delaunay) drawTriangle(i);
            if(step == V && show_voronoi){
                int arr[3];
                arr[0] = DeCheck[i].a, arr[1] = DeCheck[i].b, arr[2] = DeCheck[i].c;
                for(int j=0; j<3; j++){
                    int k = j+1;
                    if(k == 3) k = 0;
                    edge e = corners[get_pair(make_pair(arr[j], arr[k]))];
                    drawVEdge(arr[j], arr[k], e.first, e.second);
                }
            }
        }
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
    cameraHeight=30.0;
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

    //delaunay();

    glutMainLoop();



}

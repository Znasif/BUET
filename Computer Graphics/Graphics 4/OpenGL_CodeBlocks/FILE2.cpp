#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <windows.h>
#include <glut.h>
#include "bitmap_image.hpp"

int Recursion_level;

using namespace std;

struct Point3
{
    Point3() {}
    Point3(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point3 operator+(Point3 a){
        return Point3(x + a.x, y + a.y, z + a.z);
    }
    Point3 operator-(Point3 a){
        return Point3(x - a.x, y - a.y, z - a.z);
    }
    Point3 operator*(double a){
        return Point3(x*a, y*a, z*a);
    }
    void Print(){
        printf("x: %f, y: %f, z: %f\n", x, y, z);
    }
	double x, y, z;
}pos, l, r, u;

double dot(Point3 a, Point3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

class Ray{
    public:
        Point3 start;
        Point3 dir;
        Ray(Point3 a, Point3 b){
            start = a;
            dir = b;
            normalize();
        }
        void normalize(){
            dir = dir*(1/sqrt(dot(dir, dir)));
        }
        void step(double s=1.0){
            start = start + dir*s;
        }
};

Point3 Reflect(Ray* incidentVec, Point3 norm)
{
  return incidentVec->dir - norm*(2 * dot(incidentVec->dir, norm));
}
Point3 Refract(Ray* incidentVec, Point3 norm, double eta=1.5)
{
    double N_dot_I = dot(norm, incidentVec->dir);
    double k = 1 - eta * eta * (1.0 - N_dot_I * N_dot_I);
    if (k < 0.0) return Point3(0.0, 0.0, 0.0);
    else return incidentVec->dir*eta - norm*(eta * N_dot_I + sqrt(k));
}

Point3 Cross(Point3 a, Point3 b)
{
  return Point3(a.y*b.z - a.z*b.y, b.x*a.z - b.z*a.x, a.x*b.y - a.y*b.x);
}

class Object{
    public:
        Point3 reference_point;
        double height, width, length;
        int Shine;
        bool isCircle = false;
        double color[3] = {1, 1, 1};
        double co_efficients[4];
        Object(){};
        virtual void draw(){};
        virtual Point3 getNormal(Point3 a){};
        virtual double intersect(Ray* r, double* color, int level){
            return -1;
        }
        void setColor(double a, double b, double c){
            color[0] = a;
            color[1] = b;
            color[2] = c;
        }
        void setShine(int s){
            Shine = s;
        }
        void setCoEfficients(double a, double b, double c, double d){
            co_efficients[0] = a;
            co_efficients[1] = b;
            co_efficients[2] = c;
            co_efficients[3] = d;
        }
};

vector<Object*> objects;
vector<Point3> lights;

int getColor(Ray ray, double* addn, int level){
    int nearest = -1;
    double t_min = INFINITY;
    for(int k=0; k<objects.size(); k++){
        double *dummyColorAt;
        double t = objects[k]->intersect(&ray, dummyColorAt, 0);
        if(t <= 0) continue;
        if(t < t_min){
            t_min = t;
            nearest = k;
        }
    }
    if(nearest != -1){
        double *ColorAt = new double[3];
        double t = objects[nearest]->intersect(&ray, ColorAt, 1);
        Point3 st = ray.start + (ray.dir)*t;
        for(int l=0; l<3; l++){
            addn[l] = ColorAt[l]*objects[nearest]->co_efficients[0];
        }
        for(int k=0; k<lights.size(); k++){
            Ray L(st, lights[k] - st);
            L.step();
            bool flag = true;
            for(int l=0; l<objects.size(); l++){
                double* dummy;
                double m = objects[l]->intersect(&L, dummy, 0);
                if(m<0 || m>sqrt(dot(lights[k] - L.start, lights[k] - L.start))) continue;
                flag = false;
                break;
            }
            if(flag){
                Point3 norm = objects[nearest]->getNormal(st);
                Point3 refl = Reflect(&ray, norm);
                double lambert = max(0.0, dot(norm, L.dir*(-1)));
                double phong = max(0.0, dot(refl, ray.dir));
                for(int l=0; l<3; l++){
                    addn[l] += ColorAt[l]*lambert*objects[nearest]->co_efficients[1];
                    addn[l] += ColorAt[l]*pow(phong, objects[nearest]->Shine)*objects[nearest]->co_efficients[2];
                }
            }
        }
        if(Recursion_level>level){
            Point3 norm = objects[nearest]->getNormal(st);
            if(dot(norm, ray.dir) > 0) norm = norm*(-1);
            Point3 refl = Reflect(&ray, norm);
            Ray newref(st, refl);
            newref.step();
            double* addref = new double[3];
            int near1 = getColor(newref, addref, level+1);
            if(near1 !=-1){
                for(int i=0; i<3; i++){
                    addn[i] += addref[i]*objects[nearest]->co_efficients[3];
                    addn[i] = max(0.0, min(1.0, addn[i]));
                }
            }
            if(objects[nearest]->isCircle){
                Point3 refr = Refract(&ray, norm);
                Ray newraf(st, refr);
                newraf.step();
                near1 = getColor(newraf, addref, level+1);
                if(near1 !=-1){
                    for(int i=0; i<3; i++){
                        addn[i] += addref[i];//1.5;
                        addn[i] = max(0.0, min(1.0, addn[i]));
                    }
                }
            }
        }
    }
    return nearest;
}

class Sphere : public Object{
    public:
        Sphere(Point3 Center, double Radius){
            reference_point=Center;
            length=Radius;
            isCircle = true;
        }
        void draw(){
            GLUquadric *quadric = gluNewQuadric();
            glPushMatrix();
            {
                glTranslated(reference_point.x, reference_point.y, reference_point.z);
                glColor3f(color[0], color[1], color[2]);
                gluSphere(quadric, length, 24, 24);
            }
            glPopMatrix();
        }

        Point3 getNormal(Point3 in){
            Point3 temp(in-reference_point);
            return temp*(1/sqrt(dot(temp, temp)));
        }

        double intersect(Ray* r, double* colorAt, int level){
            Point3 st = r->start - reference_point;
            double a = dot(r->dir, r->dir);
            double b = 2*dot(st, r->dir);
            double c = dot(st, st) - length*length;
            double d = b*b - 4*a*c;
            if(d<0) return -1;
            d = sqrt(d);
            if(level){
                for(int i=0; i<3; i++){
                    colorAt[i] = color[i];
                }
            }
            return min((-b-d)/(2*a), (-b+d)/(2*a));
        }
};

class Floor : public Object{
    public:
        bitmap_image tile;
        Floor(double FloorWidth, double TileWidth){
            reference_point=Point3(-FloorWidth/2, -FloorWidth/2, 0);
            length=TileWidth;
            tile = bitmap_image("grass.bmp");
        }
        Point3 getNormal(Point3 in){
            return Point3(0, 0, 1);
        }
        double intersect(Ray* r, double* colorAt, int level){
            double xmin = reference_point.x*2, xmax = -xmin;
            double ymin = reference_point.y*2, ymax = -ymin;
            double t=-1;
            if(r->dir.z) t = -r->start.z/(r->dir.z);
            Point3 sec = r->start + r->dir*t;
            if(sec.x<xmin || sec.x>xmax || sec.y<ymin || sec.y>ymax) return -1;
            else{
                int i = (sec.x - xmin)/length;
                int j = (sec.y - ymin)/length;
                int p = ((sec.x - xmin) - length*i)*25;
                int q = ((sec.y - ymin) - length*j)*25;
                if(level){
                    unsigned char b, c, d;
                    tile.get_pixel(p, q, b, c, d);
                    unsigned char a[] = {b, c, d};
                    for(int l=0; l<3; l++){
                        colorAt[l] = (1 - (i+j)%2) + ((i+j)%2)*a[l]/255.0;
                    }
                }
            }
            return t;
        }
        void draw(){
            glPushMatrix();
            {
                glTranslated(reference_point.x, reference_point.y, reference_point.z);
                int ln = -2*reference_point.x/length;
                for(int i = 0; i < ln ; i++) {
                    for(int j = 0; j < ln; j++) {
                        if((i + j)%2 == 0)
                            glColor3f(1, 1, 1);
                        else
                            glColor3f(0, 0, 0);
                        glBegin(GL_QUADS);
                        {
                            glVertex3d(length*i, length*j, 0);
                            glVertex3d((i+1)*length, length*j, 0);
                            glVertex3d(length*(i+1), length*(j+1), 0);
                            glVertex3d(length*i, length*(j+1), 0);
                        }
                        glEnd();
                    }
                }
            }
            glPopMatrix();
        }
};

class Triangle : public Object{
    public:
        Point3 corners[3];
        Triangle(Point3 a, Point3 b, Point3 c){
            corners[0] = a;
            corners[1] = b;
            corners[2] = c;
        }
        Point3 getNormal(Point3 in){
            Point3 a = corners[1] - corners[0];
            Point3 b = corners[2] - corners[0];
            Point3 norm = Cross(a, b);
            return norm*(1/sqrt(dot(norm, norm)));
        }
        double intersect(Ray* r, double* colorAt, int level){
            const double EPSILON = 0.0000001;
            Point3 edge1, edge2, h, s, q;
            double a,f,u,v;
            edge1 = corners[1] - corners[0];
            edge2 = corners[2] - corners[0];
            h = Cross(r->dir, edge2);
            a = dot(edge1, h);
            if (a > -EPSILON && a < EPSILON)
                return -1;
            f = 1/a;
            s = r->start - corners[0];
            u = f * (dot(s, h));
            if (u < 0.0 || u > 1.0)
                return -1;
            q = Cross(s, edge1);
            v = f * dot(r->dir, q);
            if (v < 0.0 || u + v > 1.0)
                return -1;
            double t = f * dot(q, edge2);
            if (t > EPSILON){
                if(level){
                    for(int l=0; l<3; l++){
                        colorAt[l] = color[l];
                    }
                }
                return t;
            }
            return -1;
        }
        void draw(){
            glPushMatrix();
            {
                glColor3d(color[0], color[1], color[2]);
                glBegin(GL_TRIANGLES);
                {
                    for(int i=0; i<3; i++) glVertex3d(corners[i].x, corners[i].y, corners[i].z);
                }
                glEnd();
            }
            glPopMatrix();
        }
};

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include <windows.h>
#include <glut.h>

Object{
    point reference_point;
    double height, width, length;
    int Shine;
    double color[3];
    double co_efficients[4];
    Object(){};
    virtual void draw(){};
    void setColor();
    void setShine();
    void setCoEfficients();
}

vector <Object>  objects;
vector <Vector3> lights;

Ray{
    Vector3 start;
    Vector3 dir;
}

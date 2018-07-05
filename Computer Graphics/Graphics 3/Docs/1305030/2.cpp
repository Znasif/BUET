#include <bits/stdc++.h>

#include "bitmap_image.hpp"

#define DEBUG true

#define dbg(s) if(DEBUG) cout<<endl<<"DEBUG : "<<s<<endl

using namespace std;

struct Edge;
struct Triangle;

int Screen_Width, Screen_Height, ***frame_buffer;
double x_l_limit, y_d_limit, z_f_limit, z_b_limit;
double **z_buffer;
double Bottom_y, Left_x, dy, dx;
map<int, vector<Edge>> ET;
vector<Triangle> triangles;

bitmap_image image;
int scanline = 10000;
struct Point{
    double x, y, z;
};

struct Edge{
    Point a, b;
    int idx;
    double sect, m;
    int ans;
    void update(){
        double t = (Bottom_y + dy*scanline - a.y)/(b.y - a.y);
        sect = a.x + t*(b.x - a.x);
    }
    void inc(){
        sect += dy/m;
    }
    bool set_edge(Point p, Point q, int z){
        a = p; b = q;
        idx = z;
        if(a.y == b.y){
            return false;
        }
        if(a.y > b.y){
            Point temp;
            temp = a;
            a = b;
            b = temp;
        }
        m=(a.y-b.y)/(a.x-b.x);
        if(Bottom_y + Screen_Height*dy<a.y) return false;
        ans = (a.y-Bottom_y)/dy + .5;
        if(Bottom_y + ans*dy<a.y) ans++;
        if(ans<scanline) scanline = ans;
        return true;
    }
};

bool cmp_x(Edge e1, Edge e2){
    return e1.sect<e2.sect;
}

struct Triangle{
    Point point[3];
    int color[3];
    double a, b, c;
    int flag = 0;
    void update_table(){
        int idx = triangles.size();
        for(int i = 0; i<3; i++){
            int j = i + 1;
            if(j == 3) j = 0;
            Edge e;
            if(e.set_edge(point[i], point[j], idx)){
                ET[e.ans].push_back(e);
            }
        }
        a=(point[1].y-point[0].y)*(point[2].z-point[0].z)-(point[1].z-point[0].z)*(point[2].y-point[0].y);
        b=(point[1].z-point[0].z)*(point[2].x-point[0].x)-(point[1].x-point[0].x)*(point[2].z-point[0].z);
        c=(point[1].x-point[0].x)*(point[2].y-point[0].y)-(point[1].y-point[0].y)*(point[2].x-point[0].x);
    }
    void change(){
        flag = 1- flag;
    }
    double calcZ(double r){
        double x = max(x_l_limit, r);
        double y = Bottom_y+scanline*dy;
        double o=-a*(x-point[0].x)-b*(y-point[0].y);
        if(c) return o/c + point[0].z;
        return 5000;
    }
};

void read_data(){
    ifstream cin;
    cin.open("config.txt");
    cin>>Screen_Width>>Screen_Height;
    cin>>x_l_limit>>y_d_limit;
    cin>>z_f_limit>>z_b_limit;
    cin.close();
    cin.open("stage3.txt");

    dx = - 2 * x_l_limit / Screen_Width;
    dy = - 2 * y_d_limit / Screen_Height;

    Bottom_y = y_d_limit + dy/2.0;
    Left_x = x_l_limit + dx/2.0;

    while(!cin.eof()){
        Triangle t;
        for(int i=0; i<3; i++){
            Point p;
            cin>>p.x>>p.y>>p.z;
            t.point[i] = p;
            t.color[i] = rand()%256;
        }
        t.update_table();
        triangles.push_back(t);
    }
    cin.close();
}

void initialize_buffer(){
    frame_buffer = new int**[Screen_Height];
    z_buffer = new double*[Screen_Height];

    for(int i=0; i<Screen_Height; i++){
        frame_buffer[i] = new int*[Screen_Width];
        z_buffer[i] = new double[Screen_Width];

        for(int j=0; j<Screen_Width; j++){
            frame_buffer[i][j] = new int[3];
            frame_buffer[i][j][0] = frame_buffer[i][j][1] = frame_buffer[i][j][2] = 0;
            z_buffer[i][j] = z_b_limit;
        }
    }
}

void apply_procedure(){
    vector<Edge>AET;
    for( ;scanline<Screen_Height; scanline++){
        vector<int>APT;
        for(int i=0; i<ET[scanline].size(); i++){
            ET[scanline][i].update();
            AET.push_back(ET[scanline][i]);
        }
        sort(AET.begin(),AET.end(),cmp_x);
        if(AET.size()){
            for(int i=0;i<AET.size()-1;i++){
                Edge now = AET[i], next = AET[i+1];
                triangles[now.idx].change();
                if(triangles[now.idx].flag){
                    APT.push_back(now.idx);
                }
                else{
                    for(int j = 0; j<APT.size(); j++){
                        if(APT[j]==now.idx){
                            APT.erase(APT.begin()+j);
                            break;
                        }
                    }
                }
                if(APT.size()){
                    int tri;
                    double z_max = z_b_limit;
                    for(int j = 0; j<APT.size(); j++){
                        double z = triangles[APT[j]].calcZ(now.sect);
                        if(z>z_f_limit && z<z_max){
                            z_max=z;
                            tri=APT[j];
                        }
                    }

                    int col=(max(x_l_limit,now.sect)-Left_x)/dx;
                    double left = max(x_l_limit,now.sect);
                    double right = min(-x_l_limit,next.sect);

                    for(double j=left; j<=right; j+=dx){
                        frame_buffer[Screen_Height-1-scanline][col][0]=triangles[tri].color[0];
                        frame_buffer[Screen_Height-1-scanline][col][1]=triangles[tri].color[1];
                        frame_buffer[Screen_Height-1-scanline][col][2]=triangles[tri].color[2];
                        col++;
                    }
                }
            }
        }
        for(int j = 0; j<AET.size(); j++){
            if(AET[j].b.y < Bottom_y+(scanline+1)*dy){
                AET.erase(AET.begin()+j);
                j--;
            }
            else{
                AET[j].inc();
            }
        }
        for(int j = 0; j<APT.size(); j++){
            triangles[APT[j]].flag = 0;
        }
        APT.clear();
    }
}

void save(){
    bitmap_image image(Screen_Width,Screen_Height);
    for(int i=0;i<Screen_Height;i++){
        for(int j=0;j<Screen_Width;j++){
            image.set_pixel(j,i,frame_buffer[i][j][0],frame_buffer[i][j][1],frame_buffer[i][j][2]);
        }
    }
    image.save_image("2.bmp");
}

void free_memory(){

}

int main(){
    read_data();
    initialize_buffer();
    apply_procedure();
    save();
    free_memory();
}

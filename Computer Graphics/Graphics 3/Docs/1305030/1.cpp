#include <bits/stdc++.h>

#include "bitmap_image.hpp"

#define DEBUG false

#define dbg(s) if(DEBUG) cout<<"DEBUG : "<<s<<endl

using namespace std;

int Screen_Width, Screen_Height, ***frame_buffer;
double x_l_limit, y_d_limit, z_f_limit, z_b_limit, **z_buffer, dx, dy, Top_y, Left_x;

struct Point{
    double x, y, z;
};

struct Triangle{
    Point point[3];
    double mny[2] = {INFINITY, -INFINITY}; // 0 holds smallest y
    double mnx[2] = {INFINITY, -INFINITY};
    int color[3];
    double sect[3];

    void set_tri(int i, Point p){
        point[i] = p;
        if(mny[0] > point[i].y){
            mny[0] = point[i].y;
        }
        if(mny[1] < point[i].y){
            mny[1] = point[i].y;
        }
        if(mnx[0] > point[i].x){
            mnx[0] = point[i].x;
        }
        if(mnx[1] < point[i].x){
            mnx[1] = point[i].x;
        }
    }

    bool intersection(int i, int j, double y){
        if(point[i].y == point[j].y) return false;
        double t = (y - point[i].y)/(point[j].y - point[i].y);
        sect[i] = point[i].x + t * (point[j].x - point[i].x);
        return true;
    }

    pair<double, double> xclip(double y){
        for(int i=0; i<3; i++){
            int j = (i+1)%3;
            if(intersection(i, j, y) && intersection(j, (j+1)%3, y)){
                if((sect[i]>=mnx[0] && sect[i]<=mnx[1]) && (sect[(j)%3]>=mnx[0] && sect[j]<=mnx[1])){
                    if(sect[i]>sect[j]) swap(sect[i], sect[j]);
                    return make_pair(max(sect[i], x_l_limit), min(sect[j], -x_l_limit));
                }
            }
        }
        return make_pair(2*x_l_limit, x_l_limit);
    }

    void print(){
        cout<<"********"<<endl;
        for(int i=0; i<3; i++){
            cout<<i<<" : "<<point[i].x<<", "<<point[i].y<<", "<<point[i].z<<endl;
        }
        cout<<mny[0]<<"********"<<mny[1]<<endl;
    }

    void set_middle(){
        if(mny[1] == min(mny[1], Top_y)){
            int g = (mny[1] - Top_y) / dy - 0.5;
            mny[1] = Top_y + g * dy;
        }
    }

    double calcZ(int x1, int y1){
        double l, m, n, o;
        double x = Left_x + dx*x1;
        double y = Top_y - dy*y1;
        l=(point[1].y-point[0].y)*(point[2].z-point[0].z)-(point[1].z-point[0].z)*(point[2].y-point[0].y);
        m=(point[1].z-point[0].z)*(point[2].x-point[0].x)-(point[1].x-point[0].x)*(point[2].z-point[0].z);
        n=(point[1].x-point[0].x)*(point[2].y-point[0].y)-(point[1].y-point[0].y)*(point[2].x-point[0].x);
        o=-l*(x-point[0].x)-m*(y-point[0].y);
        if(n) return o/n + point[0].z;
        return 10000;
    }
};

vector<Triangle> triangles;

void read_data(){
    ifstream cin;
    cin.open("config.txt");
    cin>>Screen_Width>>Screen_Height;
    cin>>x_l_limit>>y_d_limit;
    cin>>z_f_limit>>z_b_limit;
    cin.close();
    cin.open("stage3.txt");
    Point p;
    while(!cin.eof()){
        Triangle t;
        for(int i=0; i<3; i++){
            cin>>p.x>>p.y>>p.z;
            t.set_tri(i, p);
            t.color[i] = rand()%256;
        }
        triangles.push_back(t);
    }
    cin.close();
}

void initialize_buffer(){
    frame_buffer = new int**[Screen_Height];
    z_buffer = new double*[Screen_Height];

    dx = - 2 * x_l_limit / Screen_Width;
    dy = - 2 * y_d_limit / Screen_Height;

    for(int i=0; i<Screen_Height; i++){
        frame_buffer[i] = new int*[Screen_Width];
        z_buffer[i] = new double[Screen_Width];

        for(int j=0; j<Screen_Width; j++){
            frame_buffer[i][j] = new int[3];
            frame_buffer[i][j][0] = frame_buffer[i][j][1] = frame_buffer[i][j][2] = 0;
            z_buffer[i][j] = z_b_limit;
        }
    }
    Top_y = - y_d_limit - dy/2.0;
    Left_x = x_l_limit + dx/2.0;
}

void apply_procedure(){
    double top_scanline, bottom_scanline;
    Point left_scan_point, right_scan_point;
    for(int i=0; i<triangles.size(); i++){
        triangles[i].set_middle();
        top_scanline = min(Top_y, triangles[i].mny[1]);
        bottom_scanline = max(y_d_limit, triangles[i].mny[0]);
        int im_r = (Top_y - top_scanline) / dy + .5;
        double current_row = top_scanline;
        while(current_row >= bottom_scanline){
            pair<double, double> lr = triangles[i].xclip(current_row);
            if(lr.second != x_l_limit){
                int im_c = (lr.first - Left_x) / dx + .5;
                while(lr.first <= lr.second){
                    double z = triangles[i].calcZ(im_c, im_r);
                    if(z>=z_f_limit && z<=z_buffer[im_r][im_c]){
                        z_buffer[im_r][im_c] = z;
                        frame_buffer[im_r][im_c][0] = triangles[i].color[0];
                        frame_buffer[im_r][im_c][1] = triangles[i].color[1];
                        frame_buffer[im_r][im_c][2] = triangles[i].color[2];
                    }
                    lr.first += dx;
                    im_c ++;
                }
            }
            current_row -= dy;
            im_r++;
        }
    }
}

void save(){
    ofstream fout;
    bitmap_image image(Screen_Width, Screen_Height);
    fout.open("z_buffer.txt", ofstream::out);
    fout.precision(6);
    dbg("save");
    for(int i=0;i<Screen_Height;i++){
        for(int j=0;j<Screen_Width;j++){
            image.set_pixel(j,i,frame_buffer[i][j][0],frame_buffer[i][j][1],frame_buffer[i][j][2]);
            if(z_buffer[i][j]<z_b_limit) fout<<fixed<<z_buffer[i][j]<<"      ";
        }
        fout<<endl;
    }
    image.save_image("1.bmp");
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

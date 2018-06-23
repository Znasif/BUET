#include <bits/stdc++.h>

#include "bitmap_image.hpp"

#define dbg(s) if(DEBUG) cout<<endl<<"DEBUG : "<<s<<endl

using namespace std;

int Screen_Width, Screen_Height, ***frame_buffer;
double x_l_limit, y_d_limit, z_f_limit, z_b_limit;
bitmap_image image;

struct Point{
    double x, y, z;
};

struct Edge{
    Point a, b;
    int idx;
    double dx, y_max;
    void set_edge(Point p, Point q, int z){
        a = p; b = q;
        idx = z;
        if(a.y == b.y){
            return;
        }
        if(a.y > b.y){
            swap(a, b);
        }
        edge[a.y]
    }
};

map<double, vector<Edge>> edge;

struct Triangle{
    Point point[3];
    int color[3];
    void update_table(){
        int z = triangles.size();
        triangles.push_back(this);
        for(int i = 0; i<3; i++){
            int j = i + 1;
            if(j == 3) j = 0;
        }
    }
};

vector<Triangle> triangles;
vector<>

void read_data(){
    ifstream cin;
    cin.open("config.txt");
    cin>>Screen_Width>>Screen_Height;
    cin>>x_l_limit>>y_d_limit;
    cin>>z_f_limit>>z_b_limit;
    cin.close();
    cin.open("stage3.txt");
    Point p;
    Triangle t;
    while(!cin.eof()){
        for(int i=0; i<3; i++){
            cin>>p.x>>p.y>>p.z;
            t.point[i] = p;
            t.color[i] = rand()%256;
        }
    }
    cin.close();
}

void initialize_buffer(){
    frame_buffer = new int**[Screen_Width];
    z_buffer = new double*[Screen_Width];

    dx = - 2 * x_l_limit / Screen_Width;
    dy = - 2 * y_d_limit / Screen_Height;

    image = new bitmap_image(Screen_Width, Screen_Height);

    for(int i=0; i<Screen_Width; i++){
        frame_buffer[i] = new int*[Screen_Height];
        z_buffer[i] = new double[Screen_Height];

        for(int j; j<Screen_Height; j++){
            frame_buffer[i][j] = new int[3];
            frame_buffer[i][j][0] = frame_buffer[i][j][1] = frame_buffer[i][j][2] = 0;
            image.set_pixel(i,j,frame_buffer[i][j][0],frame_buffer[i][j][1],frame_buffer[i][j][2]);
            z_buffer[i][j] = INFINITY;
        }
    }

    Top_y = - y_d_limit - dy/2;
    Left_x = x_l_limit + dx/2;
}

void apply_procedure(){
    double top_scanline, bottom_scanline
    Point left_scan_point, right_scan_point;
    for(int i=0; i<triangles.size(); i++){
        triangles[i].set_middle();
        top_scanline = min(Top_y, triangles[i].mnx[1]);
        bottom_scanline = max(y_d_limit, mnx[0]);
        int im_r = (Top_y - top_scanline) / dy; // image buffer row
        double current_row = top_scanline;
        while(current_row > bottom_scanline){
            int k = triangles[i].get_intersection(current_row);
            if(k == -1) dbg("No Intersection found\n");
            else{
                left_scan_point = triangles[i].sect[k];
                k++;
                if(k == 3) k = 0;
                right_scan_point = triangles[i].sect[k];
                // some work still needs to be done
            }
        }
    }
}

void save(){
    ofstream fout;
    fout.open("z_buffer.txt", ofstream::out);
    fout.precision(6);
    for(int i=0;i<Screen_Width;i++){
        for(int j=0;j<Screen_Height;j++){
            image.set_pixel(i,j,frame_buffer[i][j][0],frame_buffer[i][j][1],frame_buffer[i][j][2]);
            fout<<fixed<<z_buffer[i][j]<<"      ";
        }
    }
    image.save_image("output.bmp");
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

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "bitmap_image.hpp"

using namespace std;


class Point {
public:
    double x, y, z;
    ~Point() {}
};

class Triangle {

public:

    Point points[3];
    int color[3];

    void printTriangle() {
        for (int i=0; i<3; i++) {
            cout<<points[i].x<<", "<<points[i].y<<", "<<points[i].z<<endl;
        }
    }

    ~Triangle() {}

};

int screenWidth, screenHeight;
double xLeftLimit, xRightLimit, yBottomLimit, yTopLimit, frontLimitOfZ, rearLimitOfZ;

vector<Triangle> triangles;


void readData() {

    ifstream inputFile;
    inputFile.open("config.txt");

    inputFile>>screenWidth>>screenHeight;
    inputFile>>xLeftLimit;
    inputFile>>yBottomLimit;
    inputFile>>frontLimitOfZ>>rearLimitOfZ;

    inputFile.close();

    xRightLimit = xLeftLimit * (-1);
    yTopLimit = yBottomLimit * (-1);

    Triangle triangle;

    inputFile.open("stage3.txt");

    while (!inputFile.eof()) {
        for (int i=0; i<3; i++) {
            inputFile>>triangle.points[i].x>>triangle.points[i].y>>triangle.points[i].z;
            triangle.color[i] = rand() % 256;
        }
        triangles.push_back(triangle);
    }

    inputFile.close();

//    for (int i=0; i<triangles.size(); i++){
//        triangles[i].printTriangle();
//    }

}

double dx, dy, topY, leftX, zMax;
double** zBuffer;
Point** frameBuffer;


void initializeZBufferAndFrameBuffer() {

    dx = 2 * abs(xLeftLimit) / screenWidth;
    dy = 2 * abs(yBottomLimit) / screenHeight;

    topY = yTopLimit - dy / 2;
    leftX = xLeftLimit + dx / 2;

    zMax = abs(rearLimitOfZ - frontLimitOfZ);

    zBuffer = new double* [screenWidth];
    frameBuffer = new Point* [screenWidth];

    Point color;
    color.x = color.y = color.z = 0;


    for (int i=0; i<screenWidth; i++) {
        zBuffer[i] = new double[screenHeight];
        frameBuffer[i] = new Point[screenHeight];

        for (int j=0; j<screenHeight; j++) {
            zBuffer[i][j] = zMax*100000;
            frameBuffer[i][j] = color;
        }
    }


}

double maxY(Triangle triangle) {

    double y = triangle.points[0].y;

    for (int i=1; i<3; i++) {
        if (y < triangle.points[i].y) {
            y = triangle.points[i].y;
        }
    }

    return y;
}

double minY(Triangle triangle) {

    double y = triangle.points[0].y;

    for (int i=1; i<3; i++) {
        if (y > triangle.points[i].y) {
            y = triangle.points[i].y;
        }
    }

    return y;
}

double maxX(Triangle triangle) {

    double x = triangle.points[0].x;

    for (int i=1; i<3; i++) {
        if (x < triangle.points[i].x) {
            x = triangle.points[i].x;
        }
    }

    return x;
}

double minX(Triangle triangle) {

    double x = triangle.points[0].x;

    for (int i=1; i<3; i++) {
        if (x > triangle.points[i].x) {
            x = triangle.points[i].x;
        }
    }

    return x;
}


Point findIntersection(Point start, Point end, double row) {

    Point point;
    point.y = row;

    double t = (point.y - start.y)/(end.y - start.y);

    point.x = start.x + t * (end.x - start.x);
    point.z = start.z + t * (end.z - start.z);

    //cout<<t<<", "<<point.x<<", "<<point.y<<", "<<point.z<<endl;

    return point;

}


void applyProcedure() {



    //foreach object:Triangles
    for (int i=0; i<triangles.size(); i++) {

        cout<<i<<endl;

        Triangle triangle = triangles[i];

        //Find top_scanline and bottom_scanline after necessary clipping

        double yMax = maxY(triangle);
        double yMin = minY(triangle);

        double xMax = maxX(triangle);
        double xMin = minX(triangle);

        double topScanline = yMax<yTopLimit? yMax : yTopLimit;
        double bottomScanline = yMin>yBottomLimit? yMin : yBottomLimit;

        double colLeft = xMin>xLeftLimit? xMin : xLeftLimit;
        double colRight = xMax<xRightLimit? xMax : xRightLimit;

        //cout<<topScanline<<", "<<bottomScanline<<"; "<<colLeft<<", "<<colRight<<endl;
        int r = (topY - topScanline)/dy;
        //cout<<r<<", "<<topY - topScanline<<", "<<dy<<endl;
        //for row from top_scanline to bottom_scanline
        for (double row=topScanline; row>=bottomScanline; row-=dy) {

            //Find left_intersecting_column and right_intersecting_column after necessary clipping

            Point col1 = findIntersection(triangle.points[0], triangle.points[1], row);

            Point col2 = findIntersection(triangle.points[1], triangle.points[2], row);

            Point col3 = findIntersection(triangle.points[2], triangle.points[0], row);

            Point leftPoint, rightPoint;

            //cout<<col1.x<<", "<<col2.x<<", "<<col3.x<<", "<<row<<endl;

            if ((col1.x >= xMin && col1.x <= xMax) && (col2.x >= xMin && col2.x <= xMax)) {
                leftPoint = col1;
                rightPoint = col2;
            } else if ((col2.x >= xMin && col2.x <= xMax) && (col3.x >= xMin && col3.x <= xMax)) {
                leftPoint = col2;
                rightPoint = col3;
            } else if ((col3.x >= xMin && col3.x <= xMax) && (col1.x >= xMin && col1.x <= xMax)) {
                leftPoint = col3;
                rightPoint = col1;
            }

            if (leftPoint.x > rightPoint.x) {
                Point temp = leftPoint;
                leftPoint = rightPoint;
                rightPoint = temp;
            }

            double leftIntersectingColumn = leftPoint.x>colLeft? leftPoint.x : colLeft;

            double rightIntersectingColumn = rightPoint.x<colRight? rightPoint.x : colRight;

            //cout<<leftIntersectingColumn<<", "<<rightIntersectingColumn<<endl;

            int c = (leftIntersectingColumn - leftX)/dx;
            //for col from left_column to right_column
            for (double col=leftIntersectingColumn; col<=rightIntersectingColumn; col+=dx) {


                //Calculate z values

                double t = (col - leftPoint.x) / (rightPoint.x - leftPoint.x);


                double z = leftPoint.z + t * (rightPoint.z - leftPoint.z);

                //Compare with z_buffer ​and z_front_limit​ and update if required
                //Update pixel information if required

                int comp = z*100000;


                //cout<<row<<", "<<col<<endl;
                //cout<<i<<", "<<r<<", "<<c<<endl;


                //cout<<z<<", "<<zBuffer[r][c]<<endl;

                if (comp >= frontLimitOfZ && comp <= zBuffer[r][c]) {
                    zBuffer[r][c] = comp;

                    //cout<<z<<endl;
                    //cout<<r<<", "<<c<<endl;


                    frameBuffer[r][c].x = triangle.color[0];
                    frameBuffer[r][c].y = triangle.color[1];
                    frameBuffer[r][c].z = triangle.color[2];


                    //cout<<r<<", "<<c<<", "<<triangle.color[0]<<", "<<triangle.color[1]<<", "<<triangle.color[2]<<endl;
                }


                c++;
                //end
            }
            r++;
            //end
        }

        //end
    }

}


void save() {


    bitmap_image image(screenWidth, screenHeight);

    for (int i=0; i<screenWidth; i++) {
        for (int j=0; j<screenHeight; j++) {
            image.set_pixel(j, i, frameBuffer[i][j].x, frameBuffer[i][j].y, frameBuffer[i][j].z);
        }
    }

    image.save_image("output.bmp");

    ofstream outputFile;
    outputFile.open("z_buffer.txt");
    outputFile.precision(6);

    for (int i=0; i<screenWidth; i++) {
        //bool flag = false;
        for (int j=0; j<screenHeight; j++) {
            //if (zBuffer[i][j] < zMax) {
                outputFile<<fixed<<i<<","<<j<<", "<<zBuffer[i][j]<<", "<<frameBuffer[i][j].x<<", "<<frameBuffer[i][j].y<<", "<<frameBuffer[i][j].z<<"\n";
                //flag = true;
           // }
        }
        //if (flag) {
            //outputFile<<endl;
        //}
    }

    outputFile.close();


}

void freeMemory() {


    vector<Triangle>().swap(triangles);

    delete[] frameBuffer;
    delete[] zBuffer;
}


int main() {

    cout<<fixed;
    //freopen("out.txt", "w", stdout);


    readData();

    initializeZBufferAndFrameBuffer();

    applyProcedure();

    save();

    freeMemory();

    return 0;
}



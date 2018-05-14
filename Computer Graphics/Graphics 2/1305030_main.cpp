#include <bits/stdc++.h>
#define deg (180/(2*acos(0.0)))

using namespace std;

class draw{
    private:
        double view[4][4];
        double projection[4][4];
        double temp[3][4][4];
        double point[4];
        double product[4][3];
        double result[3];

        int mystack_size=0;
        vector<vector<vector<double>>> mystack;

        ofstream stage1;
        ofstream stage2;
        ofstream stage3;

    public:
        draw();
        void print(int idx);
        void cross_product(int idx);
        void copy_to_stack(int idx);
        void multiply();
        void camera();
        void perspective();
        void get_point(int flag);
};

draw::draw(){
    FILE* scene = freopen("scene.txt","r",stdin);
    stage1.open("stage1.txt", ofstream::out);
    stage2.open("stage2.txt", ofstream::out);
    stage3.open("stage3.txt", ofstream::out);
    stage1<<fixed;
    stage2<<fixed;
    stage3<<fixed;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++){
            if(i==j) temp[0][i][j]=1;
            else temp[0][i][j]=0;
            if(i<3 && j<3){
                if(i==j) product[i][j]=1;
                else product[i][j]=0;
            }
        }
    }
    copy_to_stack(0);

    camera();
}

void draw::cross_product(int idx){
    if(idx!=-1){
        result[0] = product[idx][1]*product[3][2] - product[idx][2]*product[3][1];
        result[1] = product[idx][2]*product[3][0] - product[idx][0]*product[3][2];
        result[2] = product[idx][0]*product[3][1] - product[idx][1]*product[3][0];
    }
    else{
        result[0] = point[1]*product[3][2] - point[2]*product[3][1];
        result[1] = point[2]*product[3][0] - point[0]*product[3][2];
        result[2] = point[0]*product[3][1] - point[1]*product[3][0];
    }
}

void draw::camera(){
    double eye[3],look[3];
    for(int i=0; i<3; i++) cin>>eye[i];
    for(int i=0; i<3; i++) cin>>look[i];
    double norm=0;
    for(int i=0; i<3; i++){
        point[i] = look[i] - eye[i];
        look[i] = -point[i];
        norm += point[i]*point[i];
        temp[2][i][3] = -eye[i];
        cin>>product[3][i];
    }
    for(int i=0; i<3; i++) point[i] /= sqrt(norm);
    cross_product(-1);
    norm = 0;
    for(int i=0; i<3; i++) norm += result[i]*result[i];
    for(int i=0; i<3; i++) product[3][i] = result[i]/norm;
    for(int i=0; i<3; i++){
        temp[1][0][i] = product[3][i];
        temp[1][2][i] = look[i];
    }
    cross_product(-1);
    for(int i=0; i<3; i++) temp[1][1][i] = -result[i];

}

void draw::copy_to_stack(int idx){
    if(idx>=mystack_size){
        vector<vector<double>> A;
        vector<double> B[4];
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++){
                B[i].push_back(temp[0][i][j]);
            }
            A.push_back(B[i]);
        }
        mystack.push_back(A);
        mystack_size += 1;
    }
    else{
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++){
                mystack[idx][i][j] = temp[0][i][j];
            }
        }
    }
}

void draw::print(int idx){
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++) cout<<mystack[idx][i][j]<<" ";
        cout<<endl;
    }
}

void draw::multiply(){

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            temp[0][i][j]=0;
            for(int k=0; k<4; k++)
            {
                temp[0][i][j] += temp[1][i][k]*temp[2][k][j];
            }
        }
    }
}

void draw::get_point(int flag){
    double A[4];
    for(int i=0; i<4; i++)
    {
        A[i]=0;
        for(int k=0; k<4; k++)
        {
            if(flag==0){
                A[i] += mystack[mystack_size-1][i][k]*point[k];
            }
            else if(flag==1){
                A[i] += view[i][k]*point[k];
            }
            else{
                A[i] += projection[i][k]*point[k];
            }
        }
    }
    double norm = A[3];
    if(norm==0) norm=1;
    for(int i=0; i<4; i++) point[i] = A[i]/norm;
}

int main(){

}

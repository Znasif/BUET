#include <bits/stdc++.h>
#define rad ((2*acos(0.0))/180)

using namespace std;

class draw
{
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
    void copy_to_stack(bool flag);
    void copy_from_stack(int idx);
    void multiply();
    void camera();
    void perspective();
    void get_point(int flag);
    void R(int idx, double angle);
};

draw::draw()
{
    FILE* scene = freopen("scene.txt","r",stdin);
    stage1.open("stage1.txt", ofstream::out);
    stage2.open("stage2.txt", ofstream::out);
    stage3.open("stage3.txt", ofstream::out);
    stage1<<fixed;
    stage2<<fixed;
    stage3<<fixed;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
                temp[0][i][j]=1;
            else
                temp[0][i][j]=0;
            if(i<3 && j<3)
            {
                if(i==j)
                    product[i][j]=1;
                else
                    product[i][j]=0;
            }
        }
    }
    copy_to_stack(true);

    camera();
    perspective();

    bool flag = true;
    string action;

    while(flag)
    {
        cin>>action;
        //cout<<action<<endl;
        if(action=="triangle")
        {
            for(int k=0; k<3; k++)
            {
                for(int i=0; i<3; i++)
                    cin>>point[i];
                point[3]=1;
                get_point(0);
                for(int i=0; i<2; i++)
                    stage1<<setprecision(7)<<point[i]<<" ";
                stage1<<setprecision(7)<<point[2]<<endl;
                get_point(1);
                for(int i=0; i<2; i++)
                    stage2<<setprecision(7)<<point[i]<<" ";
                stage2<<setprecision(7)<<point[2]<<endl;
                get_point(2);
                for(int i=0; i<2; i++)
                    stage3<<setprecision(7)<<point[i]<<" ";
                stage3<<setprecision(7)<<point[2]<<endl;
            }
            stage1<<endl;
            stage2<<endl;
            stage3<<endl;
        }
        else if(action=="translate")
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(i==j)
                        temp[2][i][j]=1;
                    else if(j==3 && i!=3)
                        cin>>temp[2][i][j];
                    else
                        temp[2][i][j]=0;
                }
            }
            copy_from_stack(1);
            multiply();
            copy_to_stack(false);
        }
        else if(action=="scale")
        {
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(i==j && i!=3)
                        cin>>temp[2][i][j];
                    else if(i==j && i==3)
                        temp[2][i][j]=1;
                    else
                        temp[2][i][j]=0;
                }
            }
            copy_from_stack(1);
            multiply();
            copy_to_stack(false);
        }
        else if(action=="rotate")
        {
            double angle, norm=0;
            cin>>angle;
            for(int i=0; i<3; i++)
            {
                cin>>product[3][i];
                norm += product[3][i]*product[3][i];
                temp[2][3][i]=0;
                temp[2][i][3]=0;
            }
            for(int i=0; i<3; i++)
                product[3][i] /= sqrt(norm);
            temp[2][3][3]=1;
            for(int i=0; i<3; i++)
            {
                R(i, angle);
                for(int j=0; j<3; j++)
                {
                    temp[2][i][j]=result[j];
                }
            }
            copy_from_stack(1);
            multiply();
            copy_to_stack(false);
        }
        else if(action=="push")
        {
            copy_from_stack(0);
            copy_to_stack(true);
        }
        else if(action=="pop")
        {
            mystack.pop_back();
            mystack_size -= 1;
        }
        else
        {
            flag=false;
        }
    }
}

void draw::R(int idx, double angle)
{
    double scalar=0;
    for(int i=0; i<3; i++)
        scalar += product[idx][i]*product[3][i];
    cross_product(idx);
    for(int i=0; i<3; i++)
    {
        result[i] *= sin(rad*angle);
        result[i] += cos(rad*angle)*product[idx][i] + (1-cos(rad*angle))*scalar*product[3][i];
    }
}

void draw::cross_product(int idx)
{
    if(idx!=-1)
    {
        result[0] = product[idx][1]*product[3][2] - product[idx][2]*product[3][1];
        result[1] = product[idx][2]*product[3][0] - product[idx][0]*product[3][2];
        result[2] = product[idx][0]*product[3][1] - product[idx][1]*product[3][0];
    }
    else
    {
        result[0] = point[1]*product[3][2] - point[2]*product[3][1];
        result[1] = point[2]*product[3][0] - point[0]*product[3][2];
        result[2] = point[0]*product[3][1] - point[1]*product[3][0];
    }
}

void draw::perspective()
{
    double fovY, aspectRatio, near, far;
    cin>>fovY>>aspectRatio>>near>>far;
    double fovX = fovY*aspectRatio;
    double t = near*tan(rad*fovY/2);
    double r = near*tan(rad*fovX/2);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            projection[i][j] = 0;
        }
    }
    projection[0][0]=near/r;
    projection[1][1]=near/t;
    projection[2][2]=-(far+near)/(far-near);
    projection[2][3]=-(2*far*near)/(far-near);
    projection[3][2]=-1;
}

void draw::camera()
{
    double eye[3],look[3];
    for(int i=0; i<3; i++)
        cin>>eye[i];
    for(int i=0; i<3; i++)
    {
        cin>>look[i];
        temp[1][i][3] = 0;
        temp[1][3][i] = 0;
    }
    temp[1][3][3] = 1;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(i==j)
                temp[2][i][j] = 1;
            else
                temp[2][i][j] = 0;
        }
    }
    double norm=0;
    for(int i=0; i<3; i++)
    {
        point[i] = look[i] - eye[i];
        norm += point[i]*point[i];
        temp[2][i][3] = -eye[i];
        cin>>product[3][i];
    }
    for(int i=0; i<3; i++)
    {
        point[i] /= sqrt(norm);
        look[i] = -point[i];
    }
    cross_product(-1);
    norm = 0;
    for(int i=0; i<3; i++)
        norm += result[i]*result[i];
    for(int i=0; i<3; i++)
        product[3][i] = result[i]/sqrt(norm);
    cross_product(-1);
    for(int i=0; i<3; i++)
    {
        temp[1][0][i] = product[3][i];
        temp[1][1][i] = -result[i];
        temp[1][2][i] = look[i];
    }
    multiply();
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
            view[i][j]=temp[0][i][j];
    }
}

void draw::copy_from_stack(int idx)
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            temp[idx][i][j]=mystack[mystack_size-1][i][j];
        }
    }
}
void draw::copy_to_stack(bool flag)
{
    if(flag)
    {
        vector<vector<double>> A;
        vector<double> B[4];
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                B[i].push_back(temp[0][i][j]);
            }
            A.push_back(B[i]);
        }
        mystack.push_back(A);
        mystack_size += 1;
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                mystack[mystack_size-1][i][j] = temp[0][i][j];
            }
        }
    }
}

void draw::print(int idx)
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
            cout<<mystack[idx][i][j]<<" ";
        cout<<endl;
    }
}

void draw::multiply()
{

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

void draw::get_point(int flag)
{
    double A[4];
    for(int i=0; i<4; i++)
    {
        A[i]=0;
        for(int k=0; k<4; k++)
        {
            if(flag==0)
            {
                A[i] += mystack[mystack_size-1][i][k]*point[k];
            }
            else if(flag==1)
            {
                A[i] += view[i][k]*point[k];
            }
            else
            {
                A[i] += projection[i][k]*point[k];
            }
        }
    }
    double norm = A[3];
    if(norm==0)
        norm=1;
    for(int i=0; i<4; i++)
        point[i] = A[i]/norm;
}

int main()
{
    draw mydraw;
}

#include <bits/stdc++.h>
#define deg (180/(2*acos(0.0)))

using namespace std;

double **I = new double*[4];
double **view = new double*[4];
double **pro = new double*[4];
double **look = new double*[3];
double near, far, fovY, aspectRatio;


map<string, int> op;


vector<double**> mystack;

void copymat(double **A, double **B)
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
            A[i][j]=B[i][j];
    }
}

void print(double **A, int m=4, int n=4)
{
    for(int i=0; i<m; i++)
    {
        for(int j=0; j<n; j++)
            cout<<A[i][j]<<" ";
        cout<<endl;
    }
}

double* cross(double *A, double *B)
{
    double *C = new double[3];
    C[0] = A[1]*B[2] - A[2]*B[1];
    C[1] = A[2]*B[0] - A[0]*B[2];
    C[2] = A[0]*B[1] - A[1]*B[0];
    return C;
}

void multi(double **A, double **B, int flag=0)
{
    double **C = new double*[4];
    for(int i=0; i<4; i++)
    {
        C[i] = new double[4];
        for(int j=0; j<4; j++)
        {
            C[i][j] = 0;
            for(int k=0; k<4; k++)
            {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(C[i][3]) C[i][j] /= C[i][3];
        }
    }
    if(flag)
        copymat(B, C);
    else
        copymat(A, C);
}

void fix_camera()
{
    double sum=0;
    for(int i=0; i<3; i++)
    {
        look[1][i]-=look[0][i];
        sum += look[1][i];
    }
    if(sum!=0){
        for(int i=0; i<3; i++)
            look[1][i] /= sum;
    }
    for(int i=0; i<4; i++)
    {
        view[i] = new double[4];
        pro[i] = new double[4];
        for(int j=0; j<4; j++)
        {
            if(i==j)
                pro[i][j]=1;
            else if(j==3)
                pro[i][j] = -look[0][i];
            else
                pro[i][j]=0;
        }
    }
    look[0] = cross(look[1], look[2]);
    sum=0;
    for(int i=0; i<3; i++)
        sum+=look[0][i];
    if(sum!=0){
        for(int i=0; i<3; i++)
            look[0][i] /= sum;
    }
    look[1] = cross(look[0], look[1]);
    for(int i=0; i<3; i++)
    {
        view[0][i] = look[0][i];
        view[1][i] = look[2][i];
        view[2][i] = -look[1][i];
        view[3][i] = 0;
    }
    view[3][3] = 1;
    multi(view, pro);
}

void fix_perspective()
{
    double fovX = fovY*aspectRatio;
    double t = near*tan(fovY/2);
    double r = near*tan(fovX/2);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            pro[i][j] = 0;
        }
    }
    pro[0][0]=near/r;
    pro[1][1]=near/t;
    pro[2][2]=-(far+near)/(far-near);
    pro[2][3]=-(2*far*near)/(far-near);
    pro[3][2]=-1;
}

void R(double *A, double *B, double angle){
    double *temp = cross(A,B);
    double sum=0;
    for(int i=0; i<3; i++) sum+=A[i]*B[i];
    for(int i=0; i<3; i++){
        temp[i] *= sin(angle*deg);
        temp[i] += A[i]*cos(angle*deg) + sum*(1-cos(angle*deg))*B[i];
    }
    A = temp;
}

void process()
{
    FILE* fi = freopen("scene.txt","r",stdin);
    ofstream fm("stage1.txt", ofstream::out);
    fm<<fixed;
    ofstream fv("stage2.txt", ofstream::out);
    fv<<fixed;
    ofstream fp("stage3.txt", ofstream::out);
    fp<<fixed;
    for(int i=0; i<3; i++)
    {
        look[i] = new double[3];
        for(int j=0; j<3; j++)
            cin>>look[i][j];
    }

    fix_camera();
    cin>>fovY>>aspectRatio>>near>>far;
    fix_perspective();

    bool flag=true;

    while(flag)
    {
        double **A = new double*[4];
        double *temp1 = new double[3];
        double *temp2 = new double[3];

        double angle;
        string now;

        cin>>now;
        cout<<now;
        switch(op[now])
        {
        case 0:
            for(int i=0; i<4; i++)
            {
                A[i] = new double[4];
                for(int j=0; j<4; j++)
                    if(i==3 && j==3)
                        A[i][j]=1;
                    else if(i==3 || j==3)
                        A[i][j]=0;
                    else
                        cin>>A[i][j];
            }
            multi(mystack[mystack.size()-1], A, 1);
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<2; j++)
                    fm<<setprecision(7)<<A[i][j]<<" ";
                fm<<setprecision(7)<<A[i][2]<<endl;
            }
            fm<<endl;
            multi(view, A, 1);
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<2; j++)
                    fv<<setprecision(7)<<A[i][j]<<" ";
                fv<<setprecision(7)<<A[i][2]<<endl;
            }
            fv<<endl;
            multi(pro, A, 1);
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<2; j++)
                    fp<<setprecision(7)<<A[i][j]<<" ";
                fp<<setprecision(7)<<A[i][2]<<endl;
            }
            fp<<endl;
            flag=false;
            break;
        case 1:
            for(int i=0; i<4; i++)
            {
                A[i] = new double[4];
                for(int j=0; j<3; j++)
                {
                    if(i==j)
                        A[i][j] = 1;
                    else A[i][j] = 0;
                }
            }
            for(int i=0; i<3; i++) cin>>A[i][3];
            A[3][3]=1;
            multi(mystack[mystack.size()-1], A);
            break;
        case 2:
            for(int i=0; i<4; i++)
            {
                A[i] = new double[4];
                for(int j=0; j<3; j++)
                {
                    if(i==3 && j==3)
                        A[i][j] = 1;
                    else if(i==j) cin>>A[i][j];
                    else A[i][j] = 0;
                }
            }
            multi(mystack[mystack.size()-1], A);
            break;
        case 3:
            cin>>temp1[0]>>temp1[1]>>temp1[2];
            for(int i=0;i<3;i++){
                A[i] = new double[4];
                for(int j=0;j<3;j++){
                    if(i==j) temp2[i] = 1;
                    else temp2[i] = 0;
                }
                R(temp2, temp1, angle);
                for(int j=0;j<3;j++) A[j][i]=temp2[j];
                A[i][3] = 0;
            }
            A[3] = new double[4];
            for(int j=0;j<3;j++) A[3][j] = 0;
            A[3][3] = 1;
            multi(mystack[mystack.size()-1], A);
            break;
        case 4:
            for(int i=0; i<4; i++)
            {
                A[i] = new double[4];
                int n=mystack.size()-1;
                for(int j=0; j<4; j++)
                {
                    A[i][j] = mystack[n][i][j];
                }
            }
            mystack.push_back(A);
            break;
        case 5:
            mystack.pop_back();
            break;
        default:
            flag=false;
            break;
        }
    }
    fm.close();
    fv.close();
    fp.close();
}

int main()
{
    for(int i=0; i<4; i++)
    {
        I[i] = new double[4];
        for(int j=0; j<4; j++)
        {
            if(i==j)
                I[i][j]=1;
            else
                I[i][j]=0;
        }
    }
    mystack.push_back(I);
    op["triangle"] = 0;
    op["translate"] = 1;
    op["scale"] = 2;
    op["rotate"] = 3;
    op["push"] = 4;
    op["pop"] =5;
    op["end"] = 6;
    process();
}


#include<bits/stdc++.h>
#define X 0
#define Y 1
#define CH cout<<"?"<<endl;

using namespace std;

struct Point{
    int x, y;
};

int rd;
Point r1,r2;

void merge(Point arr[], int l, int m, int r,int axis)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    Point L[n1], R[n2];
    for (i = 0; i < n1; i++){
        L[i].x = arr[l + i].x;
        L[i].y = arr[l + i].y;
    }
    for (j = 0; j < n2; j++){
        R[j].x = arr[m+1+j].x;
        R[j].y = arr[m+1+j].y;
    }
    i = 0;
    j = 0;
    k = l;
    if(axis==X){
    while (i < n1 && j < n2)
    {
        if (L[i].x <= R[j].x)
        {
            arr[k].x = L[i].x;
            arr[k].y = L[i].y;
            i++;
        }
        else
        {
            arr[k].x = R[j].x;
            arr[k].y = R[j].y;
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k].x = L[i].x;
        arr[k].y = L[i].y;
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k].x = R[j].x;
        arr[k].y = R[j].y;
        j++;
        k++;
    }
    }
    if(axis==Y){
    while (i < n1 && j < n2)
    {
        if (L[i].y <= R[j].y)
        {
            arr[k].x = L[i].x;
            arr[k].y = L[i].y;
            i++;
        }
        else
        {
            arr[k].x = R[j].x;
            arr[k].y = R[j].y;
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k].x = L[i].x;
        arr[k].y = L[i].y;
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k].x = R[j].x;
        arr[k].y = R[j].y;
        j++;
        k++;
    }
    }
}
void mergeSort(Point arr[], int l, int r, int axis)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        if(axis==X){
            mergeSort(arr, l, m,X);
            mergeSort(arr, m+1, r,X);
            merge(arr, l, m, r,X);
        }
        else{
            mergeSort(arr, l, m,Y);
            mergeSort(arr, m+1, r,Y);
            merge(arr, l, m, r,Y);
        }

    }
}

int SqDist(Point a, Point b){
    return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

int nearest(Point A[],int n, int d,Point cp[2]){
    int m=d;
    mergeSort(A,0,n,Y);
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n && abs(A[j].y - A[i].y) < sqrt(m); j++){
            if (SqDist(A[i],A[j]) < m){
                m = SqDist(A[i], A[j]);
                cp[0]=A[i];
                cp[1]=A[j];
            }
        }
    }
    return m;
}

int simply(Point A[],int n,Point cp[2]){
    int d;
    rd=1000000000;
    for(int i=0;i<n;i++){
        for(int j=i+1; j<n;j++){
            d=SqDist(A[i],A[j]);
            if(d<rd){
                rd=d;
                cp[0]=A[i];
                cp[1]=A[j];
            }
        }
    }
    return rd;
}

Point *tmp;

int closestPair(Point A[],int n,Point cp[2]){
    if(n<=3){
        return simply(A,n,cp);
    }
    int mid=n/2,x1=A[mid].x;
    Point cpl[2],cpr[2];
    int dl=closestPair(A,mid,cpl);
    int dr=closestPair(A+mid,n-mid,cpr);
    int d;
    if(dl<dr){
        d=dl;
        cp[0]=cpl[0];
        cp[1]=cpl[1];
    }
    else{
        d=dr;
        cp[0]=cpr[0];
        cp[1]=cpr[1];
    }
    Point C[n];
    int j = 0;
    for (int i = 0; i < n; i++){
        if (abs(A[i].x - x1)< sqrt(d)){
            C[j].x = A[i].x;
            C[j].y = A[i].y;
            j++;
        }
    }

    int temp=nearest(C,j,d,cpl);
    if(temp<d){
        d=temp;
        cp[0]=cpl[0];
        cp[1]=cpl[1];
    }
    return d;
}

int main(){
    freopen("input.txt","r",stdin);
    int n;
    cin>>n;
    Point A[n],C[n];
    int p,q;
    for(int i=0;i<n;i++){
        cin>>p>>q;
        A[i].x=p;
        A[i].y=q;
    }
    mergeSort(A,0,n,X);
    tmp=A;
    Point ans[2];
    rd=closestPair(A,n,ans);
    cout<<"Distance : "<<sqrt(rd)<<endl;
    cout<<ans[0].x<<" "<<ans[0].y<<endl;
    cout<<ans[1].x<<" "<<ans[1].y<<endl;
    return 0;
}

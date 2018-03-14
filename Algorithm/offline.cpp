#include<iostream>
#include<math.h>
#define X 0
#define Y 1

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

void closestPair(Point A[],int l, int r){
    rd=999999;
    int d;
    if(r<l+1){
        d=SqDist(A[l],A[r]);
        if(d<rd && d!=0){
            rd=d;
            r1.x=A[l].x;
            r2.x=A[r].x;
            r1.y=A[l].y;
            r2.y=A[r].y;
            return;
        }
    }
    else{
        int mid=(l+r)/2;
        closestPair(A,l,mid);
        closestPair(A,mid+1,r);
        return;
    }
}


void bruteForce(Point A[],int n){
    int d;
    rd=999999;
    for(int i=0;i<n;i++){
        for(int j=i+1; j<n;j++){
            d=SqDist(A[i],A[j]);
        if(d<rd){
            rd=d;
            r1.x=A[i].x;
            r2.x=A[j].x;
            r1.y=A[i].y;
            r2.y=A[j].y;
        }
    }
    }
    return;
}

int main(){
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
    closestPair(A,0,n);
    int mid=n/2;
    int i,j=0;
    for(i=0;i<n;i++){
        if((A[mid].x-A[i].x)*(A[mid].x-A[i].x)<=rd){
            C[j].x=A[i].x;
            C[j].y=A[i].y;
            j++;
        }
    }
    mergeSort(C,0,n,Y);
    bruteForce(C,j);
    cout<<"Smallest Distance : "<<sqrt(rd)<<endl;
    cout<<"          Point 1 : "<<r1.x<<" "<<r1.y<<endl;
    cout<<"          Point 2 : "<<r2.x<<" "<<r2.y<<endl;
    return main();
}

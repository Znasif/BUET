#include <bits/stdc++.h>

using namespace std;

vector<pair<int, pair<double, double>>> graph;
vector<pair<double, int>> sorted;
vector<int> final;

int V=0;
int p0,p1;

bool sortbysec(const pair<int,pair<double, double>> &a,
               const pair<int,pair<double, double>> &b)
{
    return (a.second > b.second);
}


bool orientation(int f, int s, int i){
    double px,py,qx,qy,rx,ry;
    px = graph[f].second.first;
    py = graph[f].second.second;

    qx = graph[s].second.first;
    qy = graph[s].second.second;

    rx = graph[i].second.first;
    ry = graph[i].second.second;

    return (qy - py) * (rx - qx) >= (qx - px) * (ry - qy);
}

void convexhull(int show=0)
{
    FILE* fi = freopen("1305030_points.txt","r",stdin);
    int cnt=0;
    //cout.precision(12);
    if(show) cout <<  "Points :"<<endl;
    cin>>V;
    while (cnt<V)
    {
        double a,b;
        cin>>a>>b;
        graph.push_back(make_pair(cnt, make_pair(a,b)));
        if(show)
        {
            cout << graph[cnt].first<<  " X : "<< graph[cnt].second.first << " Y : "<< graph[cnt].second.second << '\n';
        }
        cnt++;
    }
    fclose(fi);
}

void writePoints()
{
    ofstream myfile("1305030_edges.txt", ofstream::out);
    int H=final.size();
    double cost=0.0;
    cout<<"Extreme Points : \n"<<endl;
    cout<<1<<" "<<graph[final[0]].second.first<<" "<<graph[final[0]].second.second<<endl;
    for(int i=0; i<H-1; i++)
    {
        myfile<<final[i]<<" "<<final[i+1]<<endl;
        cout<<i+2<<" "<<graph[final[i+1]].second.first<<" "<<graph[final[i+1]].second.second<<endl;
        double a = graph[final[i]].second.first-graph[final[i+1]].second.first;
        double b = graph[final[i]].second.second-graph[final[i+1]].second.second;
        cost += sqrt(a*a+b*b);
    }
    cout<<"\nTotal Length of Convex Hull : "<<cost<<endl;
    myfile.close();
}


int main()
{
    int start_s=clock();
    convexhull();
    writePoints();
    double a,b;

    while(1){
        cout<<"Check Points: "<<endl;
        cin>>a>>b;
        cout<<a<<b<<endl;
        checkPoint(a,b);
    }
    int stop_s=clock();
    cout << "\ntime: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
}

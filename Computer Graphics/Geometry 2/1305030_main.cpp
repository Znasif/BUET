#include <bits/stdc++.h>

#define stv 0
#define env 1
#define spv 2
#define mrv 3
#define rgv 4
#define pi acos(0.0)

using namespace std;

vector<pair<pair<int,int>, pair<double, double>>> graph, sorted;

int V=0;
int p0,p1;

bool sortbysec(const pair<pair<int,int>, pair<double, double>> &a,
               const pair<pair<int,int>, pair<double, double>> &b)
{
    return (a.second.second > b.second.second);
}

struct bst
{
    bool operator() (const double& lhs, const double& rhs) const
    {
        return lhs<rhs;
    }
};

int decide_point(int f, int s, int i)
{
    double px,py,qx,qy,rx,ry;
    px = graph[f].second.first;
    py = graph[f].second.second;

    qx = graph[s].second.first;
    qy = graph[s].second.second;

    rx = graph[i].second.first;
    ry = graph[i].second.second;

    double ax, ay, bx, by;
    bx = px - qx;
    by = py - qy;
    ax = rx - qx;
    ay = ry - qy;

    double ang = atan2(ax*by-bx*ay, ax*bx+ay*by);
    cout<<ang/pi*180<<" "<<qx<<" "<<qy<<" || "<<py<<" "<<ry<<endl;

    if(qy>py && qy>ry)
    {
        if(ang < pi)
            return stv;
        else if(ang > pi)
            return spv;
        return rgv;
    }
    if(qy<py && qy<ry)
    {
        if(ang < pi)
            return env;
        else if(ang > pi)
            return mrv;
        return rgv;
    }
    return rgv;
}

void make_monotone()
{
    //FILE* fi = freopen("1305030_out.txt","r",stdin);
    //FILE* fi = freopen("1305030_input1.txt","r",stdin);
    FILE* fi = freopen("1305030_input2.txt","r",stdin);
    int cnt=0;
    //cout.precision(12);
    cin>>V;
    while (cnt<V)
    {
        double a,b;
        cin>>a>>b;
        graph.push_back(make_pair(make_pair(cnt,-1), make_pair(a,b)));
        sorted.push_back(make_pair(make_pair(cnt,-1), make_pair(a,b)));
        // cout << graph[cnt].first<<  " X : "<< graph[cnt].second.first << " Y : "<< graph[cnt].second.second << '\n';
        cnt++;
    }
    sort(sorted.begin(), sorted.end(), sortbysec);
    for (int i = 0; i < V-2; i++)
    {
        //cout << sorted[i].first.first<<" "<<sorted[i].first.second<<  " X : "<< sorted[i].second.first;
        //cout << " Y : "<< sorted[i].second.second << '\n';
        int ret = decide_point(i, i+1, i+2);
        cout<<ret<<endl;
        graph[i+1].first.second = ret;
        //cout << graph[i].first.first<<" "<<graph[i].first.second<<  " X : "<< graph[i].second.first;
        //cout << " Y : "<< graph[i].second.second << '\n';
    }
    fclose(fi);
}


int main()
{
    int start_s=clock();



    make_monotone();



    int stop_s=clock();
    cout << "\ntime: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
}

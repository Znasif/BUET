#include <bits/stdc++.h>

#define stv 0
#define env 1
#define spv 2
#define mrv 3
#define rgl 4
#define rgr 5

#define pdi pair<int, int>
#define pdd pair<double, double>
#define pdp pair<pdi, pdd>

using namespace std;

vector<pdp> graph, sorted;
int V=0;

struct bst
{
    bool operator () (const int& left,const int& right) const
    {
        double px,py,qx,qy,rx,ry;
        int next=left+1;
        if(next==V) next=0;
        px = graph[left].second.first;
        py = graph[left].second.second;

        qx = graph[next].second.first;
        qy = graph[next].second.second;

        rx = graph[right].second.first;
        ry = graph[right].second.second;

        double ax, ay, bx, by;
        bx = qx - px;
        by = qy - py;
        ax = rx - px;
        ay = ry - py;

        double ang = bx*ay - by*ax;
        return ang>0;
    }
};

vector<pdi> diagonals;
set<int, bst> edges;
map<int, int> helper;

int p0,p1;

bool sortbysec(const pdp &a, const pdp &b)
{
    return (a.second.second > b.second.second);
}

void delete_edge(int f){
    int prev = f-1;
    if(prev==-1) prev = V-1;
    if(graph[helper[prev]].first.second==mrv) diagonals.push_back(make_pair(f, helper[prev]));
    edges.erase(prev);
}

void update_edge(int f){
    set<int>::iterator it;
    it = edges.lower_bound(f);
    it--;
    diagonals.push_back(make_pair(f, helper[*(it)]));
}

void process(int f)
{
    int point_type = graph[f].first.second;
    switch(point_type)
    {
    case stv:
        edges.insert(f);
        helper[f]= f;
        break;
    case env:
        delete_edge(f);
        break;
    case spv:
        update_edge(f);
        edges.insert(f);
        helper[f]= f;
        break;
    case mrv:
        delete_edge(f);
        update_edge(f);
        break;
    default:
        break;
    }
}


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
    bx = qx - px;
    by = qy - py;
    ax = rx - px;
    ay = ry - py;

    double ang = bx*ay - by*ax;

    if(qy>=py && qy>=ry)
    {
        if(ang < 0)
            return spv;
        else
            return stv;
    }
    if(qy<=py && qy<=ry)
    {
        if(ang < 0)
            return mrv;
        else
            return env;
    }
    if(py>qy && qy>ry) return rgl;
    return rgr;

}

void make_monotone()
{
    //FILE* fi = freopen("1305030_out.txt","r",stdin);
    //FILE* fi = freopen("1305030_input1.txt","r",stdin);
    FILE* fi = freopen("1305030_mono.txt","r",stdin);
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

    for (int i = 0; i < V; i++)
    {
        //cout << sorted[i].first.first<<" "<<sorted[i].first.second<<  " X : "<< sorted[i].second.first;
        //cout << " Y : "<< sorted[i].second.second << '\n';
        if(i==0)
        {
            graph[V-1].first.second = decide_point(V-2, V-1, 0);
            graph[0].first.second = decide_point(V-1, 0, 1);
        }
        if(i<V-2)
        {
            graph[i+1].first.second = decide_point(i, i+1, i+2);
        }
        cout << graph[i].first.first<<" "<<graph[i].first.second<<  " X : "<< graph[i].second.first;
        cout << " Y : "<< graph[i].second.second << '\n';
    }

    for (int i = 0; i < V; i++)
    {
        //process(sorted[i].first.first);
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

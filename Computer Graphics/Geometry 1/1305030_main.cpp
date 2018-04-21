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
    if(show) cout<<'\n';

    for(int i=0; i<V; i++)
    {
        if(graph[p0].second.first <=graph[i].second.first)
        {
            if(graph[p0].second.first == graph[i].second.first && graph[p0].second.second < graph[i].second.second)
            {
                continue;
            }
            p0 = graph[i].first;
        }
    }

    for(int i=0; i<V; i++)
    {
        double slope, a1, b1, a0, b0;
        a1 = graph[i].second.first;
        b1 = graph[i].second.second;
        a0 = graph[p0].second.first;
        b0 = graph[p0].second.second;
        if(a1==a0 and b1==b0) continue;
        else if(a1==a0 and b1!=b0)
        {
            sorted.push_back(make_pair(-INFINITY,i));
        }
        else
        {
            slope = (b1-b0)/(a1-a0);
            sorted.push_back(make_pair(slope,i));
        }
    }
    sort(sorted.begin(),sorted.end());
    sorted.push_back(make_pair(0.0, p0));

    for(int i=0; i<sorted.size(); i++)
    {
        if(show) cout <<sorted[i].second<<  " Slope : "<< sorted[i].first<< '\n';
    }

    p1 = sorted[0].second;

    final.push_back(p0);
    final.push_back(p1);

    int f, s;

    for(int i=1;i<sorted.size();i++){
        //find orientation
        int H = final.size();
        f = final[H-2];
        s = final[H-1];
        bool flag = orientation(f,s,sorted[i].second);
        while(flag){
            final.pop_back();
            int H = final.size();
            if(H<3){
                break;
            }
            f = final[H-2];
            s = final[H-1];
            flag = orientation(f,s,sorted[i].second);
        }
        final.push_back(sorted[i].second);
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

void checkPoint(double a, double b){
    int H=final.size();
    int flag=1;
    bool check;
    graph.push_back(make_pair(V, make_pair(a,b)));
    for(int i=0; i<H-1; i++)
    {
        if(flag){
            check = orientation(final[i], final[i+1], V);
            flag=0;
            continue;
        }
        if(check!=orientation(final[i], final[i+1], V)){
            cout<<"Not inside the Convex Hull"<<endl;
            graph.pop_back();
            return;
        }
    }
    cout<<"Inside the Convex Hull"<<endl;
    graph.pop_back();
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

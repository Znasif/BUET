#include <bits/stdc++.h>
#include "1305030_MST.cpp"
#include "1305030_bound.cpp"
#include "1305030_algo.cpp"

using namespace std;

vector<pair<int, pair<double, double>>> graph;
double* edges = NULL;
int* mstParent;
int* parent;
int V=0;


void printEdge()
{
    cout <<  "Edges :"<<endl;
    for(int i=0; i<V; i++)
    {
        for(int j=0; j<V; j++)
        {
            cout<<edges[i*V+j]<< " ";
        }
        cout<<'\n';
    }
}

bool allEdges(int show=0)
{
    double a, b;
    edges = new double[V*V];
    double m[V][V][2];
    if(show) cout <<  "Slopes and Intersection :"<<endl;
    for(int i=0; i<V-1; i++)
    {
        for(int j=i+1; j<V; j++)
        {
            a = graph[i].second.first-graph[j].second.first;
            b = graph[i].second.second-graph[j].second.second;
            edges[i*V+j] = sqrt(a*a+b*b);
            edges[j*V+i] = edges[i*V+j];
            m[i][j][0] = b/a;
            if( m[i][j][0] == -0 || m[i][j][0] == -INFINITY)  m[i][j][0] = -m[i][j][0];
            if(a!=0) m[i][j][1] = graph[i].second.second - graph[i].second.first*m[i][j][0];
            else m[i][j][1] = graph[i].second.first;
            if(show) cout <<  m[i][j][0]  << " "<< m[i][j][1] << '\n';
        }
    }
    if(show)
    {
        cout<<endl;
        printEdge();
        cout<<endl;
    }
    for(int i=1; i<V-1; i++)
    {
        for(int j=i+1; j<V; j++)
        {
            if(m[i][j][0] != m[0][1][0] || m[i][j][1] != m[0][1][1]) return false;
        }
    }
    return true;
}

float calcAllPerm(int show=0)
{
    double min_cost=INFINITY,cost;
    int x, y;
    do
    {
        if(show) cout <<  "Current Permutation : ";
        //****************  Exponential Solution **********************
        cost = edges[graph[0].first*V+graph[V-1].first];
        if(show) cout << graph[0].first;
        for(int i=1; i<V; i++)
        {
            if(show) cout << graph[i].first;
            x = graph[i].first;
            y = graph[i-1].first;
            cost += edges[x*V+y];
            if(cost >= min_cost) break;
        }
        if(show) cout<<'\n';
        if(cost<min_cost)
        {
            min_cost = cost;
            parent[graph[0].first]=graph[V-1].first;
            for(int i=1; i<V; i++)
            {
                parent[graph[i].first]=graph[i-1].first;
            }
        }
        //*************************************************************
    }
    while ( next_permutation(graph.begin()+1,graph.end()));
    return min_cost;
}

void init(int show=0)
{
    mstParent = new int[V];
    parent = new int[V];
    if(allEdges(show))
    {
        cout<<"No solution exists "<<'\n';
        exit(0);
    }
    return;
}

void readPoints(int show=0)
{
    ifstream myfile;
    string line;
    string::size_type sz;
    myfile.open("1305030_points.txt");
    //cout.precision(12);
    int cnt = 0;
    if(show) cout <<  "Points :"<<endl;
    while ( getline (myfile,line) )
    {
        double a = stod(line, &sz);
        double b = stod(line.substr(sz));
        graph.push_back(make_pair(cnt, make_pair(a,b)));
        if(show)
        {
            cout << graph[cnt].first<<  " X : "<< graph[cnt].second.first << " Y : "<< graph[cnt].second.second << '\n';
        }
        cnt++;
    }
    cout<<'\n';
    V=cnt;
    init(show);
    myfile.close();
}

void writePoints(int MST=0)
{
    ofstream myfile("1305030_edges.txt", ofstream::out);
    if(MST==1)
    {
        primMST(V, 1);
        for(int i=1; i<V; i++)
        {
            myfile<<i<<" "<< mstParent[i]<<'\n';
        }
    }
    else
    {
        if(MST==0) cout<<"\nCost for TSP : "<<calcAllPerm()<<endl;
        else if(MST==2) cout<<"\nApproximate Cost for TSP : "<<approx(1)<<endl;
        else if(MST==3) cout<<"\nCost for TSP using Branch&Bound: "<<branch_bound()<<endl;
        for(int i=0; i<V; i++)
        {
            myfile<<i<<" "<< parent[i]<<'\n';
        }
    }
    myfile.close();
}

void cleanup()
{
    delete[] edges;
    delete[] parent;
    delete[] mstParent;
}

int main()
{
    int start_s=clock();


	readPoints();
    writePoints(3);
    cleanup();


    int stop_s=clock();
    cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
}

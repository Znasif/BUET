#include <bits/stdc++.h>

using namespace std;

extern vector<pair<int, pair<double, double>>> graph;
extern double* edges;
extern int V;
extern int* mstParent;
extern int* parent;

int minKey(int key[], bool mstSet[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
    {
        if (mstSet[v] == false && key[v] < min) min = key[v], min_index = v;
    }

    return min_index;
}
int printMST()
{
    printf("Edge   Weight\n");
    for (int i = 1; i < V; i++) cout<< mstParent[i]<<" - "<<i<<"    "<<edges[i*V+mstParent[i]]<<endl;
}
double primMST(int E, int show=0)
{
    int key[E];
    bool mstSet[E];


    for (int i = 0; i < E; i++) key[i] = INT_MAX, mstSet[i] = false;
    key[0] = 0;
    mstParent[0] = -1;

    for (int count = 0; count < E-1; count++)
    {
        int u = minKey(key, mstSet);

        mstSet[u] = true;
        for (int v = 0; v < E; v++)
        {
            if (edges[u*E+v] && mstSet[v] == false && edges[u*E+v] <  key[v]) mstParent[v]  = u, key[v] = edges[u*E+v];
        }
    }
    double cost=0.0;
    for (int i = 1; i < E; i++) cost+=edges[i*E+mstParent[i]];

    if(show) printMST();
    return cost;
}

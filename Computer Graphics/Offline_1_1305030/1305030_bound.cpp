#include<stdio.h>

using namespace std;

void swap(char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(char *a, int l, int r)
{
    int i;
    if (l == r)
        printf("%s\n", a);
    else
    {
        for (i = l; i <= r; i++)
        {
            swap((a+l), (a+i));
            permute(a, l+1, r);
            swap((a+l), (a+i));
        }
    }
}

double branch_bound()
{
    double min_cost=INFINITY,cost;
    int x, y;
    do
    {
        //****************  Branch and Bound *************************
        for(int i=0; i<V-1; i++)
        {
            double* temp_edges=edges;
            double min_a=INFINITY,min_b=INFINITY;
            const int sz = V-i-1;
            edges = new double[sz*sz];

            for(int j=0; i<sz-1; j++)
            {
                for(int k=j+1; k<sz; k++)
                {
                    x = graph[j+i+1].first;
                    y = graph[k+i+1].first;

                    double p = graph[0].second.first-graph[j+i+1].second.first;
                    double q = graph[0].second.second-graph[j+i+1].second.second;
                    if(min_a>sqrt(p*p+q*q)) min_a=sqrt(p*p+q*q);
                    p = graph[0].second.first-graph[k+i+1].second.first;
                    q = graph[0].second.second-graph[k+i+1].second.second;
                    if(min_b>sqrt(p*p+q*q)) min_b=sqrt(p*p+q*q);

                    edges[j*sz+k]=temp_edges[x*V+y];
                }
            }
            cost = primMST(sz)+min_a+min_b;

            for(int j; j<V; j++)
            {

            }

            delete[] edges;
            edges = temp_edges;
        }
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

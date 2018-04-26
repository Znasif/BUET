#include <bits/stdc++.h>


using namespace std;

int *visited, *adj;
vector<int> taken;


void DFS(int i)
{
    visited[i]=1;
    taken.push_back(i);
    for(int j=0; j<V; j++)
    {
        if(!visited[j] && adj[i*V+j]==1)
        {
            DFS(j);
            taken.push_back(i);
        }
    }
}


double approx(int show=0)
{
    visited = new int[V];
    adj = new int[V*V];
    for(int i=0; i<V; i++)
    {
        for(int j=i; j<V; j++)
        {
            adj[i*V+j] = 0;
            adj[j*V+i] = 0;
        }
        visited[i] = 0;
    }

    primMST(V);
    for(int i=1; i<V; i++)
    {
        adj[i*V+mstParent[i]] = 1;
        adj[mstParent[i]*V+i] = 1;
    }
    if(show)
    {
        cout<<"\nAdjacency Matrix from MST : "<<endl;
        for(int i=0; i<V; i++)
        {
            for(int j=0; j<V; j++)
            {
                cout<<adj[i*V+j]<<" ";
            }
            cout<<endl;
        }
    }

    DFS(0);

    delete[] adj;
    delete[] visited;

    visited = new int[2*V+1];
    for(int i=0; i<2*V+1; i++)
    {
        visited[i]=0;
    }


    if(show) cout<<"\nRoute taken : "<<endl;
    for(int i=0; i<taken.size()-1; i++)
    {
        if(visited[taken[i]]) taken[i]=-1;
        else visited[taken[i]]=1;
        if(show) cout<<taken[i]<<" ";
    }
    if(show) cout<<endl;

    delete[] visited;

    int cnt=0;
    double cost=0.0;
    for(int i=0; i<taken.size()-1; i++)
    {
        if(taken[i]!=-1)
        {
            cnt=i+1;
            while(cnt<taken.size() && taken[cnt]==-1)
            {
                cnt++;
            }
            cout<<"\nParent of : "<<taken[i]<<" is : "<<taken[cnt]<<endl;
            parent[taken[i]]=taken[cnt];
            cost+=edges[taken[i]*V+taken[cnt]];
        }
    }
    return cost;
}

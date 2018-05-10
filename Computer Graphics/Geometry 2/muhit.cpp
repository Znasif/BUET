#include <bits/stdc++.h>
#define pdd pair<double ,double>
#define vpdd vector<pdd>
using namespace std;

void sort_y(vpdd v)
{
    vpdd kome,bare;
    int i,j,sz=v.size();
    int idx=-1;
    double mn=INFINITY;
    for(i=0;i<=sz-1;i++){
        if(v[i+1].second>v[i].second){
            if(v[i].second<mn){
                mn=v[i].second;
                idx=i;
            }
        }
    }
    cout<<v[idx].first<<" "<<v[idx].second;
}


int main()
{
    freopen("1305030_mono.txt","r",stdin);
    vpdd v;
    int point_number,i;
    double x,y;
    cin>>point_number;
    for(i=1;i<=point_number;i++){
        cin>>x>>y;
        v.push_back(pdd(x,y));
    }

    sort_y(v);
}

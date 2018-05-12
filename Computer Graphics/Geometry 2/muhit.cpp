#include <bits/stdc++.h>
#define pdd pair<double ,double>
#define vpdd vector<pdd>
using namespace std;

struct bst
{
    bool operator () (const pdd& left,const pdd& right) const
    {
        return left < right;
    }
};

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
    set<pdd, bst> s;
    //map<pdd, pdd> s;
    int point_number,i;
    double x,y;
    cin>>point_number;
    for(i=0;i<point_number;i++){
        cin>>x>>y;
        v.push_back(pdd(x,y));
        //s[v[i]]=v[i];
        s.insert(pdd(x,y));
    }
    set<pdd>::iterator it = s.begin();
    while(it != s.end()){
        cout<<(*it).first<<" "<<(*it).second<<endl;
        it++;
    }
    it = s.end();
    //for(i=0;i<point_number;i++) cout<<s[v[i]].first<<" "<<s[v[i]].second<<endl;
    sort_y(v);
}

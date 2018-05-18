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

struct compare{
    bool operator ()(const int &lft,const int &rgt) const
    {
        int lft1=lft,lft2=(lft+1),rgt1=rgt,rgt2=(rgt+1);
        if(lft2==n)lft2=0;
        if(rgt2==n)rgt2=0;
        if(isUpore(lft2,lft1))swap(lft1,lft2);
        if(isUpore(rgt2,rgt1))swap(rgt1,rgt2);
        if(isUpore(lft1,rgt1)){
            if(rgt1==lft1 || rgt1==lft2)
            return !isRight((points[rgt1][0]+points[rgt2][0])/2.0,(points[rgt1][1]+points[rgt2][1])/2.0,lft1,lft2);
            return !isRight(points[rgt1][0],points[rgt1][1],lft1,lft2);
        }
        if(lft1==rgt1 || lft1==rgt2)
        return isRight((points[lft1][0]+points[lft2][0])/2.0,(points[lft1][1]+points[lft2][1])/2.0,rgt1,rgt2);
        return isRight(points[lft1][0],points[lft1][1],rgt1,rgt2);
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

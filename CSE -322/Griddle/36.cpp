#include <bits/stdc++.h>
#define vv vector< vector<node> >
#define file freopen("in.txt","r",stdin)
using namespace std;
struct node
{
    int teacher;
    int cls;
    int room;
};
vector<node>tmp;
vv period;
int mn_cost;
void init_push(int i,int j,int times,int Room_No)
{
    node a;
    int t,c,r,k;
    t=j;
    if(i%Room_No)
    {
        r=(i/Room_No)+1;
        c=i%Room_No;
    }
    else
    {
        r=(i/Room_No);
        c=Room_No;
    }
    a.teacher=t,a.cls=c,a.room=r;
    for(k=1; k<=times; k++)
    {
        tmp.push_back(a);
    }
}

int teacher_cost(vv tmp,int Room_No)
{
    int i,j,k,sz=tmp.size(),cnt=0;
    for(i=0; i<sz; i++)
    {
        for(j=0; j<=Room_No-2; j++)
        {
            for(k=j+1; k<=Room_No-1; k++)
            {
                if(tmp[i][k].teacher==tmp[i][j].teacher)cnt++;
            }
        }
    }
    return cnt;
}
int class_cost(vv tmp,int Room_No)
{
    int i,j,k,sz=tmp.size(),cnt=0;
    for(i=0; i<sz; i++)
    {
        for(j=0; j<=Room_No-2; j++)
        {
            for(k=j+1; k<=Room_No-1; k++)
            {
                if(tmp[i][k].cls==tmp[i][j].cls)cnt++;
            }
        }
    }
    return cnt;
}
int room_cost(vv tmp,int Room_No)
{
    int i,j,k,sz=tmp.size(),cnt=0;
    for(i=0; i<sz; i++)
    {
        for(j=0; j<=Room_No-2; j++)
        {
            for(k=j+1; k<=Room_No-1; k++)
            {
                if(tmp[i][k].room==tmp[i][j].room)cnt++;
            }
        }
    }
    return cnt;
}

void hill_climb(vv p,int Room_No,int total_period)
{

    int i,j,k,l;
    int ptc=teacher_cost(p,Room_No);
    int pcc=class_cost(p,Room_No);
    int prc=room_cost(p,Room_No);
    int total_pc=ptc+pcc+prc;
    mn_cost=total_pc;
    if(mn_cost==0)return;

    vv t1=p;
    vv t2=t1;
    int flag;
    while(1)
    {
        flag=0;
        for(i=0; i<=total_period-2; i++)
        {
            for(j=i+1; j<=total_period-1; j++)
            {
                for(k=0; k<=Room_No-1; k++)
                {
                    for(l=0; l<=Room_No-1; l++)
                    {
                        swap(t1[i][k],t1[j][l]);
                        int t1_tc=teacher_cost(t1,Room_No);
                        int t1_cc=class_cost(t1,Room_No);
                        int t1_rc=room_cost(t1,Room_No);
                        int t1_totalcost=t1_tc+t1_cc+t1_rc;
                        if(t1_totalcost<mn_cost){
                            mn_cost=t1_totalcost;
                            t2=t1;
                            flag=1;
                        }
                        swap(t1[i][k],t1[j][l]);
                    }
                }
            }
        }

        if(!flag)break;
        else{
            t1=t2;
            ptc=teacher_cost(t1,Room_No);
            pcc=class_cost(t1,Room_No);
            prc=room_cost(t1,Room_No);
            total_pc=ptc+pcc+prc;
            mn_cost=total_pc;
            if(mn_cost==0)return;
        }

    }

}



int main()
{
    file;
    int i,j,k,l,tch,cls,rm,total_room;
    scanf("%d%d%d",&tch,&cls,&rm);
    total_room=rm*rm;
    int req[total_room+3][tch+2];
    for(i=1; i<=total_room; i++)
    {
        for(j=1; j<=tch; j++)
        {
            scanf("%d",&req[i][j]);
        }
    }

    for(i=1; i<=total_room; i++)
    {
        for(j=1; j<=tch; j++)
        {
            init_push(i,j,req[i][j],rm);
        }
    }
    /*for(i=0;i<tmp.size();i++){
        cout<<tmp[i].teacher<<" "<<tmp[i].cls<<" "<<tmp[i].room<<endl;
    }*/
    int sz=tmp.size();
    int total_period=sz/rm;
    vector<node>u;
    for(i=0; i<total_period; i++)
    {
        j=i*rm;
        for(k=1; k<=rm; k++)
        {
            u.push_back(tmp[j]);
            j++;
        }
        period.push_back(u);
        u.clear();
    }

    /*for(i=0;i<total_period;i++){
        for(j=0;j<rm;j++){
            cout<<period[i][j].teacher<<" "<<period[i][j].cls<<" "<<period[i][j].room<<": ";
        }
        cout<<endl;
    }*/
    /*int tc=teacher_cost(period,rm);
    int cc=class_cost(period,rm);
    int rc=room_cost(period,rm);
    cout<<tc<<" "<<cc<<" "<<rc<<endl;
    cout<<tc+cc+rc;*/

    /*int cnt=0;
    for(i=0; i<=total_period-2; i++)
    {
        for(j=i+1; j<=total_period-1; j++)
        {
            for(k=0; k<=rm-1; k++)
            {
                for(l=0; l<=rm-1; l++)
                {
                    cnt++;
                }
            }
        }
    }*/
    //cout<<cnt;
    hill_climb(period,rm,total_period);
    cout<<mn_cost;
}
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
vector<vector<pdd>> monotone;
ofstream fw;
int V=0;

bool below(int f, int s){
    double px,py,qx,qy,rx,ry;
    px = graph[f].second.first;
    py = graph[f].second.second;

    qx = graph[s].second.first;
    qy = graph[s].second.second;
    if(py < qy) return true;
    if(py == qy && px > qx) return true;
    return false;
}

struct bst
{
    bool operator () (const int& left,const int& right) const
    {
        double px,py,qx,qy,rx,ry,sx,sy;
        int next=left+1;
        if(next==V) next=0;
        px = graph[left].second.first;
        py = graph[left].second.second;

        qx = graph[next].second.first;
        qy = graph[next].second.second;

        next=right+1;
        if(next==V) next=0;
        rx = graph[right].second.first;
        ry = graph[right].second.second;

        sx = graph[next].second.first;
        sy = graph[next].second.second;

        double ax, ay, bx, by, ang;
        /*if(ry > py){
            bx = qx - px;
            by = qy - py;
            ax = rx - px;
            ay = ry - py;

            return bx*ay > by*ax;
        }
        else{
            bx = sx - rx;
            by = sy - ry;
            ax = px - rx;
            ay = py - ry;

            return bx*ay > by*ax;
        }*/
        bx = qx - px;
        by = qy - py;
        ax = rx - px;
        ay = ry - py;
        //cout<<py<<" "<<ry<<endl;
        return bx*ay > by*ax;
    }
};

vector<pdi> diagonals;
vector<pair<pdd, pdd>> monotone_diagonals;
set<int, bst> edges;
map<int, int> helper;

int p0,p1;

bool sortbysec(const pdp &a, const pdp &b)
{
    if(a.second.second > b.second.second) return true;
    if(a.second.second < b.second.second) return false;
    if(a.second.second == b.second.second) return a.second.first<b.second.first;
}

void delete_edge(int f){
    int prev = f-1;
    if(prev==-1) prev = V-1;
    if(graph[helper[prev]].first.second==mrv) diagonals.push_back(make_pair(f, helper[prev]));
    edges.erase(prev);
}

void update_edge(int f, bool flag=false){
    set<int>::iterator it;
    it = edges.lower_bound(f);
    it--;
    if(flag) diagonals.push_back(make_pair(f, helper[*(it)]));
    else if(graph[helper[*(it)]].first.second==mrv) diagonals.push_back(make_pair(f, helper[*(it)]));
    helper[*(it)] = f;
}

void sort_monotone(int p_num){
    int l, r, b, sz = monotone[p_num].size();
    vector<int> l_chain, r_chain, temp;
    double bottom = INFINITY, next;

    for(int i=0;i<sz;i++){
        next = monotone[p_num][i+1].second;
        if(i==sz-1) next = monotone[p_num][0].second;
        if(next > monotone[p_num][i].second){
            if(monotone[p_num][i].second < bottom){
                bottom=monotone[p_num][i].second;
                b=i;
            }
        }
    }

    //cout<<monotone[p_num][b].first<<" "<<monotone[p_num][b].second;

    next = b+1;
    if(next==sz) next = 0;
    int now = b;
    while(monotone[p_num][now].second <= monotone[p_num][next].second){
        r_chain.push_back(now);
        now = next;
        next = next+1;
        if(next==sz) next = 0;
    }
    r = r_chain.size();
    while(now!=b){
        l_chain.push_back(now);
        now = now+1;
        if(now==sz) now = 0;
    }
    l = l_chain.size();

    int rx = r-1, lx = 0;
    int chain[sz];
    b=0;

    while(rx>-1 && lx<l){
        if(monotone[p_num][l_chain[lx]].second >= monotone[p_num][r_chain[rx]].second){
            temp.push_back(l_chain[lx]);
            //cout<<l_chain[lx]<<" ";
            chain[temp[temp.size()-1]] = 0;
            lx++;
        }
        else{
            temp.push_back(r_chain[rx]);
            //cout<<r_chain[rx]<<" ";
            chain[temp[temp.size()-1]] = 1;
            rx--;
        }
        while(lx==l && rx>-1){
            temp.push_back(r_chain[rx]);
            //cout<<r_chain[rx]<<" ";
            chain[temp[temp.size()-1]] = 1;
            rx--;
        }
        while(rx==-1 && lx<l){
            temp.push_back(l_chain[lx]);
            //cout<<l_chain[lx]<<" ";
            chain[temp[temp.size()-1]] = 0;
            lx++;
        }
    }
    //cout<<endl;
    //for(int i=0;i<l;i++) cout<<l_chain[i]<<" ";
    //cout<<endl;
    //for(int i=0;i<r;i++) cout<<r_chain[i]<<" ";
    //cout<<endl;
    //for(int i=0;i<sz;i++) cout<<temp[i]<<" ";
    for(int i=0;i<sz-1;i++){
        b = i+1;
        if(b==sz) b = 0;
        if(monotone[p_num][temp[i]].second == monotone[p_num][temp[b]].second){
            if(monotone[p_num][temp[i]].first > monotone[p_num][temp[b]].first){
                l = chain[temp[b]], r = temp[b];
                chain[temp[b]] = chain[temp[i]], temp[b] = temp[i];
                chain[temp[i]] = l, temp[i] = r;
            }
        }
    }
    //cout<<endl;
    //for(int i=0;i<sz;i++) cout<<temp[i]<<" ";

    stack<int> mystack;
    b = 0;
    mystack.push(temp[b++]);
    mystack.push(temp[b++]);

    for(;b<sz-2;b++){
        //cout<<monotone[p_num][temp[b]].first<<" "<<monotone[p_num][temp[b]].second<<endl;
        if(chain[temp[b]] != chain[mystack.top()]){
            cout<<"Other Chain"<<endl;
            r = mystack.top();
            while(mystack.size()>1){
                l = mystack.top();
                mystack.pop();
                monotone_diagonals.push_back(make_pair(monotone[p_num][l], monotone[p_num][temp[b]]));
            }
            mystack.pop();
            mystack.push(r);
            mystack.push(temp[b]);
        }
        else{
            cout<<"Same Chain"<<endl;
            r = mystack.top();
            mystack.pop();
            l = mystack.top();

            //cout<<r<<" "<<temp[b]<<" "<<l<<endl;
            double px,py,qx,qy,rx,ry;
            px = monotone[p_num][temp[b]].first;
            py = monotone[p_num][temp[b]].second;

            qx = monotone[p_num][r].first;
            qy = monotone[p_num][r].second;

            rx = monotone[p_num][l].first;
            ry = monotone[p_num][l].second;

            double ax, ay, bx, by;
            bx = qx - px;
            by = qy - py;
            ax = rx - px;
            ay = ry - py;

            double ang = bx*ay - by*ax;
            while(ang>0){
                mystack.pop();
                monotone_diagonals.push_back(make_pair(monotone[p_num][l], monotone[p_num][temp[b]]));

                r = l;
                if(mystack.empty()) break;
                l = mystack.top();

                //cout<<r<<" "<<temp[b]<<" "<<l<<endl;

                px = monotone[p_num][temp[b]].first;
                py = monotone[p_num][temp[b]].second;

                qx = monotone[p_num][r].first;
                qy = monotone[p_num][r].second;

                rx = monotone[p_num][l].first;
                ry = monotone[p_num][l].second;

                bx = qx - px;
                by = qy - py;
                ax = rx - px;
                ay = ry - py;

                ang = bx*ay - by*ax;
            }
            mystack.push(l);
            mystack.push(temp[b]);
        }
    }
    mystack.pop();
    while(mystack.size()>1){
        l = mystack.top();
        mystack.pop();
        monotone_diagonals.push_back(make_pair(monotone[p_num][l], monotone[p_num][temp[sz-1]]));
    }
    cout<<monotone_diagonals.size()<<endl;

    for(int i=0;i<monotone_diagonals.size();i++){
        fw<<monotone_diagonals[i].first.first<<" "<<monotone_diagonals[i].first.second<<" ";
        fw<<monotone_diagonals[i].second.first<<" "<<monotone_diagonals[i].second.second<<endl;
    }
}

void process(int f)
{
    int point_type = graph[f].first.second;
    switch(point_type)
    {
    case stv:
        //cout<<stv<<endl;
        edges.insert(f);
        helper[f]= f;
        break;
    case env:
        //cout<<env<<endl;
        //break;
        delete_edge(f);
        break;
    case spv:
        //cout<<spv<<endl;
        //break;
        update_edge(f, true);
        edges.insert(f);
        helper[f]= f;
        break;
    case mrv:
        //cout<<mrv<<endl;
        //break;
        delete_edge(f);
        update_edge(f);
        break;
    case rgl:
        //cout<<rgl<<endl;
        ///break;
        delete_edge(f);
        edges.insert(f);
        helper[f]= f;
        break;
    default:
        //cout<<rgr<<endl;
        //break;
        update_edge(f);
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

    if(below(f, s) && below(i, s))
    {
        if(ang < 0)
            return spv;
        else
            return stv;
    }
    if(below(s, f) && below(s, i))
    {
        if(ang < 0)
            return mrv;
        else
            return env;
    }
    if(below(s, f) && below(i, s)) return rgl;
    return rgr;

}

void make_monotone()
{
    //FILE* fi = freopen("1305030_out.txt","r",stdin);
    FILE* fi = freopen("1305030_outmany1.txt","r",stdin);
    //FILE* fi = freopen("1305030_mono.txt","r",stdin);
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
        //cout << graph[i].first.first<<" "<<graph[i].first.second<<  " X : "<< graph[i].second.first;
        //cout << " Y : "<< graph[i].second.second << '\n';
    }

    for (int i = 0; i < V; i++)
    {
        process(sorted[i].first.first);
        set<int>::iterator it = edges.begin();
        while(it!=edges.end()){
            cout<<graph[*(it)].second.first<<" ";
            it++;
        }
        cout<<endl;
    }
    for (int i = 0; i < diagonals.size(); i++)
    {
        fw<<graph[diagonals[i].first].second.first<<" "<<graph[diagonals[i].first].second.second<<" ";
        fw<<graph[diagonals[i].second].second.first<<" "<<graph[diagonals[i].second].second.second<<endl;
    }
    fclose(fi);
}

void triangulate(){
    FILE* fi = freopen("1305030_input6.txt","r",stdin);
    int cnt;
    vector<pdd> temp;
    cin>>cnt;
    while (cnt>0)
    {
        double a,b;
        cin>>a>>b;
        temp.push_back(make_pair(a,b));
        cnt--;
    }
    monotone.push_back(temp);
    sort_monotone(0);
}

int main()
{
    int start_s=clock();
    fw.open("monotone.txt", ofstream::out);



    make_monotone();
    //triangulate();



    int stop_s=clock();
    cout << "\ntime: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;
}

#include <bits/stdc++.h>
#include "1305030_Simplex.cpp"

typedef unsigned long long ll;

using namespace std;

int n, m;

ll *S;
double **memo, *cost;
bool **lookUp;
double **A;
vector<int> *sets;

void print(ll u){
    bitset<10> w(u);
    cout<<w<<endl;
}

void print(double **A){
    for(int i=0; i<n+1; i++){
        for(int j=0; j<m+1; j++) cout<<A[i][j]<<" ";
        cout<<endl;
    }
}

double set_covered(ll coveredMask,int nowConsiderIndex){
    if(nowConsiderIndex == m & coveredMask != (1<<n) - 1) return INFINITY;
    else if(nowConsiderIndex == m & coveredMask == (1<<n) - 1) return 0;
    else if(memo[nowConsiderIndex][coveredMask] != -1) return memo[nowConsiderIndex][coveredMask];

    double valChoose= set_covered( coveredMask | S[nowConsiderIndex], nowConsiderIndex+1 ) + cost[nowConsiderIndex];
    double valNotChoose= set_covered( coveredMask, nowConsiderIndex+1 );
    memo[nowConsiderIndex][coveredMask] = min( valChoose, valNotChoose );

    if(valChoose==memo[nowConsiderIndex][coveredMask]) lookUp[nowConsiderIndex][coveredMask]=true;
    return memo[nowConsiderIndex][coveredMask];
}

double runDP(int show=0){
    double act = set_covered(0, 0);
    if(show) cout<<" Actual Minimum Cost : "<<act<<endl<<endl;
    int cnt=0;
    ll flag=0;
    if(act == INFINITY) return INFINITY;
    while(cnt<m && flag!=(1<<n)-1){
        if(lookUp[cnt][flag]==true){
            flag |= S[cnt];
            if(show) cout<<cnt<<" ";
        }
        cnt++;
    }
    return act;
}

double runLP(int show=0){
    double f=-INFINITY;
    for(int i=0; i<n; i++){
        int sz = sets[i].size();
        if(f<sz) f=sz;
        for(int j=0; j<m; j++) A[i][j]=0;
        A[i][m]=-1;
        for(int j=0; j<sz; j++){
            A[i][sets[i][j]]=-1;
        }
    }
    double *b, Ret = INFINITY;
    b = new double[m];
    long ret = Simplex(n, m, A, b, Ret);
    if(ret==1){
        Ret = 0;
        for(int i=0; i<m; i++){
            // Deterministic Rounding
            if(b[i]>=1/f){
                if(show) cout<<i<<" ";
                Ret+=cost[i];
            }
        }
        if(show) cout<<"\n\n Minimum Cost with Deterministic Rounding: "<<Ret<<endl<<endl;
        ll check;
        do{
        check = 0;
        Ret = 0;
        if(show) cout<<"\nTry : ";
        for(int i=0; i<m; i++){
            // Randomized Rounding
            bool take = (rand() % 100) < 100*b[i];
            if(take){
                if(show) cout<<i<<" ";
                Ret+=cost[i];
                check |= S[i];
            }
        }
        }while(check!=(1<<n)-1);
        if(show) cout<<"\n\n Minimum Cost with Randomized Rounding: "<<Ret<<endl;
    }
    else if(ret==0 && show) cout<<"Solution not Feasible";
    else if(ret==-1 && show) cout<<"Unbounded";
    if(show) cout<<endl<<endl;
    delete[] b;
    return Ret;
}

void clean(){
    for(int i=0; i<m; i++){
        delete[] memo[i];
        delete[] A[i];
        delete[] lookUp[i];
    }
    for(int i=0; i<n; i++){
        sets[i].clear();
    }
    delete[] sets;
    delete[] S;
    delete[] cost;
    delete[] memo;
    delete[] A[m];
    delete[] A;
    delete[] lookUp;
}

void setup(int show=0)
{
    FILE* fi = freopen("1305030_test.txt","r",stdin);
    //FILE* fi = freopen("5. Sample test case.txt","r",stdin);
    int T, p;
    cin>>T;
    while(T>0){
        if(show) cout<<"*******       "<<T<<"        *******\n";
        cin>>n>>m;
        S = new ll[m];
        memo = new double*[m];
        A = new double*[n+1];
        sets = new vector<int>[n];
        lookUp = new bool*[m];
        for(int i=0; i<m; i++){
            S[i] = 0;
            memo[i] = new double[(1<<n) - 1];
            lookUp[i] = new bool[(1<<n) - 1];
            for(int j=0; j<(1<<n)-1; j++){
                memo[i][j]=-1;
                lookUp[i][j]=false;
            }
        }
        for(int i=0; i<n+1; i++){
            A[i] = new double[m+1];
        }
        cost = new double[m];
        for(int i=0;i<m;i++){
            int j;
            cin>>cost[i]>>j;
            A[n][i]=-cost[i];
            for(int k=0; k<j; k++){
                cin>>p;
                sets[p].push_back(i);
                S[i] |= 1<<p;
            }
        }

        A[n][m] = 0;

        double dp, lp;
        lp = runLP(show);
        dp = runDP(show);

        if(!show) cout<<lp/dp<<endl;

        if(show) cout<<endl;
        //clean();
        T--;
    }
}


int main()
{
    setup(1);
}

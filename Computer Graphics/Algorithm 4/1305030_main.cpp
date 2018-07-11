#include <bits/stdc++.h>

#define DEBUG 1
#define dbg(s) if(DEBUG) fout<<s

typedef long long int lli;

using namespace std;

ifstream fin("1305030_string.txt");
lli n, m, cnt, d = 26;
string T, P;
set<string> Match;

lli q[] = {1000000007, 1000000103};

void Naive_1(){
    for(lli i=0; i<n-m+1; i++){
        if(P==T.substr(i, m)) cnt++;
    }
    cout<<cnt<<endl;
    cnt = 0;
}

void Naive_2(){
    for(lli i=0; i<n-1; i++){
        for(lli j=i+1; j<n; j++){
            Match.insert(T.substr(i, j));
        }
    }
    cout<<Match.size()<<endl;
    Match.clear();
}

void hashing(){
    lli h[] = {1, 1}, p[] = {0, 0}, t[] = {0,0};
    for(lli i=0; i<m; i++){
        for(int j = 0; j<1; j++){
            p[j] = (d*p[j] + P[i])%q[j];
            t[j] = (d*t[j] + T[i])%q[j];
            if(i<m-1) h[j] = (h[j]*d)%q[j];
        }
    }
    for(lli i=0; i<n-m+1; i++){
        if(p[0]==t[0] && p[1]==t[1]) cnt++;
        if(i<n-m){
            for(int j = 0; j<1; j++){
                t[j] = ((t[j]-T[i]*h[j])*d + T[i+m])%q[j];
                if(t[j]<0) t[j] += q[j];
            }
        }
    }
    cout<<cnt<<endl;
    cnt = 0;
}

void KMP(){
    lli pi[m];
    pi[0] = 0;
    lli k=0;
    for(lli i=1; i<m; i++){
        while(k>0 && P[k] != P[i]) k = pi[k];
        if(P[k] == P[i]) k++;
        pi[i] = k;
    }
    lli q = 0;
    for(lli i=0; i<n; i++){
        while(q>0 && P[q] != T[i]) q = pi[q];
        if(P[q] == T[i]) q++;
        if(q == m - 1){
            cnt++;
            q = pi[q];
        }
    }
    cout<<cnt<<endl;
    cnt = 0;
}

int main()
{
    fin>>n;
    fin>>T;
    fin>>m;
    fin>>P;
    Naive_1();
    hashing();
    /*Naive_2();*/
    KMP();
}

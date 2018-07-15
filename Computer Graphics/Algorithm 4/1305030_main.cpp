#include <bits/stdc++.h>

#define DEBUG 1
#define dbg(s) if(DEBUG) fout<<s

typedef long long int lli;

using namespace std;

ofstream fout;

lli q[] = {1000000007, 1000000103};

struct Common{
    lli n, m, cnt, d = 26;
    string T, P;

    void Naive(){
        cnt = 0;
        fout<<"Naive :\n";
        for(lli i=0; i<n-m+1; i++){
            if(P==T.substr(i, m)){
                cnt++;
                fout<<i<<" ";
            }
        }
        cout<<"Naive : "<<cnt<<endl;
        cnt = 0;
    }

    void hashing(){
        lli h[] = {1, 1}, p[] = {0, 0}, t[] = {0,0};
        fout<<"\n\nHashing :\n";
        for(lli i=0; i<m; i++){
            for(int j = 0; j<1; j++){
                p[j] = (d*p[j] + P[i])%q[j];
                t[j] = (d*t[j] + T[i])%q[j];
                if(i<m-1) h[j] = (h[j]*d)%q[j];
            }
        }
        for(lli i=0; i<n-m+1; i++){
            if(p[0]==t[0] && p[1]==t[1]){
                cnt++;
                fout<<i<<" ";
            }
            if(i<n-m){
                for(int j = 0; j<1; j++){
                    t[j] = ((t[j]-T[i]*h[j])*d + T[i+m])%q[j];
                    if(t[j]<0) t[j] += q[j];
                }
            }
        }
        cout<<"Hashing : "<<cnt<<endl;
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
        fout<<"\n\nKMP :\n";
        for(lli i=0; i<n; i++){
            while(q>0 && P[q] != T[i]) q = pi[q - 1];
            if(P[q] == T[i]) q++;
            if(q == m){
                cnt++;
                fout<<i-m+1<<" ";
                q = pi[q - 1];
            }
        }
        cout<<"KMP : "<<cnt<<endl;
        cnt = 0;
    }
};

struct entry
{
    int p;
    int nr[2];
};

int cmp(entry a, entry b)
{
    return (a.nr[0] == b.nr[0]) ? (a.nr[1] < b.nr[1] ? 1 : 0):
               (a.nr[0] < b.nr[0] ?1: 0);
}

struct Distinct{
    set<string> Match;
    lli n, cnt;
    int stp;
    string T;

    void Naive(){
        for(int i=0; i<n; i++){
            for(int j=0; j<n-i; j++){
                Match.insert(T.substr(i, j+1));
            }
        }
        cout<<"Naive : "<<Match.size()<<endl;
        Match.clear();
    }

    void SuffixArray()
    {
        lli i;
        int S[21][n];
        int sa[n];
        entry L[n];
        for (i = 0; i < n; i ++) S[0][i] = T[i] - 'a';

        for (stp = 1, cnt = 1; cnt >> 1 < n; stp ++, cnt <<= 1)
        {
            for (i = 0; i < n; i ++)
            {
                L[i].nr[0] = S[stp - 1][i];
                L[i].nr[1] = i + cnt < n ? S[stp - 1][i + cnt] : -1;
                L[i].p = i;
            }
            sort(L, L + n, cmp);
            for (i = 0; i < n; i++){
                S[stp][L[i].p] = i > 0 && L[i].nr[0] == L[i - 1].nr[0] && L[i].nr[1] == L[i - 1].nr[1] ?
                S[stp][L[i - 1].p] : i;
            }
        }

        for(int i=0;i<n;i++) sa[S[stp-1][i]]=i;

        lli ret=n;
        ret*=(n+1);
        ret/=2;

        for(int i=0; i<n-1; i++){
            int x=sa[i];
            int y = sa[i+1], k;
            for (k = stp - 1; k >= 0 && x < n && x < n; k --){
                if (S[k][x] == S[k][y]){
                    x += 1 << k;
                    y += 1 << k;
                    ret -= 1 << k;
                }
            }
        }

        cout<<"LCP : "<<ret<<endl;
    }
};


int main()
{
    fout.open("1305030_out.txt", ofstream::out);
    cout<<"1: All occurrences of P on T \n2: Number of Distinct Substrings\n";
    int q;
    cin>>q;
    ifstream fin("1305030_string.txt");
    if(q == 1){
        Common C;
        fin>>C.n;
        fin>>C.T;
        fin>>C.m;
        fin>>C.P;
        C.Naive();
        C.hashing();
        C.KMP();
    }
    else if(q == 2){
        Distinct D;
        fin>>D.n;
        fin>>D.T;
        D.Naive();
        D.SuffixArray();
    }
}

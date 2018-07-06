#include <bits/stdc++.h>

#define DEBUG 1
#define dbg(s) if(DEBUG) fout<<s

using namespace std;

ifstream fin("1305030_string.txt");
long long int n, m, cnt;
string T, P;

void Naive_1(){
    for(long long int i=0; i<n-m+1; i++){
        if(P==T.substr(i, m)) cnt++;
    }
    cout<<cnt<<endl;
}

void hashing(){

}

int main()
{
    fin>>n;
    fin>>T;
    fin>>m;
    fin>>P;
    Naive_1();
    //cout<<T<<" "<<P<<endl;
}

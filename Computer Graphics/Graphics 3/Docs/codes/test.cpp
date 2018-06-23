#include <bits/stdc++.h>

#define dbg(s) if(DEBUG) cout<<endl<<"DEBUG : "<<s<<endl

using namespace std;

int main(){
    double x, y, z;
    double a;
    while(true){
        cin>>x>>y>>z;
        a = (x - y)/z;
        a = y + int(a)*z;
        cout<<a<<endl;
    }
}

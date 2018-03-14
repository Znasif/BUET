#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define read(s) freopen(s, "r", stdin)
#define INF 1000000
#define DLY 3

using namespace std;

int sockfd,bind_flag,sent_bytes,bytes_received;
struct sockaddr_in send_to,get_from;
socklen_t addrlen;


char from[100], buffer[1024];
int msg_cost,msg_len;
int send_now;

vector<string> routers;
string outputbuffer,myIP="",ip1="",ip2="",msg_frwd="";

struct Path{
    int cost;
    string nextHop;
    void updatePath(string n,int c){
        cost=c;
        nextHop=n;
    }
    void showPath(){
        cout<<"          "<<nextHop<<"      "<<cost<<endl;
    }
    string msgPath(){
        string c = std::to_string(cost);
        string send = "_"+nextHop+"_"+c+"|";
        return send;
    }
};

map <string, Path> myTable,topTable;
map <string, int> n_routers;

int check(string p)
{

    for(int i=0; i<routers.size(); i++)
    {
        if(p==routers[i])return 0;
    }
    return 1;
}

string msgTable()
{
    string temp="";
    //cout<<"sending"<<endl;
    for (std::map<string, struct Path>::iterator it=myTable.begin(); it!=myTable.end(); it++){
        if(it->first!="")temp+=it->first+(it->second).msgPath();
    }
    //cout<<"  Sending  "<<temp<<endl;
    return temp;
}

void initTable(string f){
    read(f.c_str());
    string a,b;
    int c;
    Path temp;
    temp.updatePath(myIP,0);
    myTable[myIP]=temp;
    topTable[myIP]=temp;
    while(cin>>a){
        cin>>b;
        cin>>c;
        if(a==myIP){
            temp.updatePath(b,c);
            myTable[b]=temp;
            topTable[b]=temp;
            if(check(b)) routers.push_back(b);
            n_routers[b]=0;
        }
        else if(b==myIP){
            temp.updatePath(a,c);
            myTable[a]=temp;
            topTable[a]=temp;
            if(check(a)) routers.push_back(a);
            n_routers[a]=0;
        }
        else{
            if(check(a)){
                routers.push_back(a);
                temp.updatePath("000000000000",INF);
                myTable[a]=temp;
                topTable[a]=temp;
            }
            if(check(b)){
                routers.push_back(b);
                temp.updatePath("000000000000",INF);
                myTable[b]=temp;
                topTable[b]=temp;
            }
        }
    }
}

void showTable(){
    cout<<endl<<endl;
    cout<<"Destination           Next Hop              Cost"<<endl<<endl;
    for (std::map<string, struct Path>::iterator it=myTable.begin(); it!=myTable.end(); ++it){
        if(it->first!=""){
            cout<<it->first;
            (it->second).showPath();
        }
    }
    cout<<endl<<endl;
}

void updateTable(string dest,string nextHop,int cost, string sender){
    int d=(cost+topTable[sender].cost);
    if(((d < myTable[dest].cost) && nextHop!=myIP) || ( myTable[dest].nextHop==sender) ){
        myTable[dest].updatePath(sender,min(d,INF));
    }
}

void getTable(string sender){
    string msg(buffer);
    //cout<<"  Recieving  "<<buffer<<endl;
    int len=msg.size();
    int cost,flag=0;
    n_routers[sender]=send_now;
    string dest,nextHop;
    string temp="";
    for(int i=0;i<len;i++){
        if(msg[i]=='|'){
            cost=stoi(temp);
            flag=0;
            temp="";
            //cout<<i<<"  "<<dest<<" "<<nextHop<<" "<<cost<<" "<<sender<<endl;
            updateTable(dest,nextHop,cost,sender);
            continue;
        }
        else if(msg[i]=='_'){
            if(flag==0){
                dest=temp;
                flag=1;
            }
            else if(flag==1){
                nextHop=temp;
                flag=2;
            }
            temp="";
            continue;
        }
        temp+=msg[i];
    }
}

void updateCost(){
    if(ip1!=myIP){
        topTable[ip1].cost=msg_cost;
        cout<<"New Cost "<<topTable[ip1].cost<<endl;
    }
    else{
        topTable[ip2].cost=msg_cost;
        cout<<"New Cost "<<topTable[ip2].cost<<endl;
    }
    //showTable();
}

string ord(unsigned char x){
    string ret="";
    if(x==0) return "0";
    while(x){
        ret+=(x%10+'0');
        x/=10;
    }
    reverse(ret.begin(),ret.end());
    return ret;
}

void forward_msg(char* b){
    if(myTable[ip2].cost<INF){
        string a=myTable[ip2].nextHop;
        inet_pton(AF_INET,a.c_str(),&send_to.sin_addr);
        b[0]='f';b[1]='r';b[2]='w';b[3]='d';
        cout<<"Forwarding message to neighbor : "<<a<<endl;
        sent_bytes=sendto(sockfd, b, 1024, 0, (struct sockaddr*) &send_to, sizeof(sockaddr_in));
    }
    else{
        cout<<"Cannot Forward !"<<endl;
    }
}

void checkDown(){
    vector<string> down;
    for (std::map<string, int>::iterator it=n_routers.begin(); it!=n_routers.end(); ++it){
        if(send_now-it->second>=DLY){
            down.push_back(it->first);
            //cout<<"route to "<<it->first<<" down"<<endl;
        }
    }
    for (std::map<string, struct Path>::iterator it=myTable.begin(); it!=myTable.end(); ++it){
        string s=it->first;
        if(s==myIP) continue;
        s=(it->second).nextHop;
        if(std::find(down.begin(),down.end(),s)!=down.end()) (it->second).updatePath("000000000000",INF);
    }
}

void sendTable(){
    outputbuffer = msgTable();
    send_to.sin_family = AF_INET;
    send_to.sin_port = htons(4747);
    for (std::map<string, int>::iterator it=n_routers.begin(); it!=n_routers.end(); ++it){
        string a=it->first;
        inet_pton(AF_INET,a.c_str(),&send_to.sin_addr);
        //cout<<"Sending Table to neighbor : "<<a<<endl;
        sent_bytes=sendto(sockfd, outputbuffer.c_str(), 1024, 0, (struct sockaddr*) &send_to, sizeof(sockaddr_in));
    }
}

int getclk()
{
    return atoi(buffer+4);
}

void driverMsg(){
    string s(buffer,0,4);
    if(s=="clk "){
        send_now=getclk();
        //cout<<buffer<<endl;
        sendTable();
        checkDown();
    }
    else if(s=="cost"){
        ip1="";
        for(int i=4;i<8;i++){
            ip1+=ord(buffer[i]);
            if(i!=7) ip1+=".";
        }
        for(int i=8;i<12;i++){
            ip2+=ord(buffer[i]);
            if(i!=11) ip2+=".";
        }
        int low=buffer[12];
        int high=buffer[13];
        msg_cost=low+high*10;
        updateCost();
    }
    else if(s=="send"){
        ip2="";
        for(int i=8;i<12;i++){
            ip2+=ord(buffer[i]);
            if(i!=11) ip2+=".";
        }
        int low=buffer[12];
        int high=buffer[13];
        msg_len=low+high*10;
        if(ip2==myIP){
            msg_frwd="";
            for(int i=0;i<msg_len;i++){
                msg_frwd+=buffer[14+i];
            }
            cout<<"( "<<msg_frwd<<" )"<<" packet reached destination"<<endl;
        }
        else{
            forward_msg(buffer+4);
        }
    }
    else if(s=="show"){
        showTable();
    }
}

void routerMsg(){
    int ret_flag=4,j=0;
    string s(buffer,0,4);
    if(s=="frwd"){
        ip2="";
        for(int i=4;i<8;i++){
            ip2+=ord(buffer[i]);
            if(i!=7) ip2+=".";
        }
        if(myIP==ip2){
            int low=buffer[8];
            int high=buffer[9];
            msg_len=low+high*10;
            msg_frwd="";
            for(int i=0;i<msg_len;i++){
                msg_frwd+=buffer[10+i];
            }
            cout<<"( "<<msg_frwd<<" )"<<" packet reached destination"<<endl;
        }
        else{
            forward_msg(buffer);
        }
    }
    else{
        string from1(from);
        if(from1!=myIP){
            //cout<<"Recieved Table from "<<from1<<endl;
            getTable(from1);
        }
    }
}

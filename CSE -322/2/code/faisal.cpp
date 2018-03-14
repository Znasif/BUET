#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define file freopen("topo.txt","r",stdin);
#define balsal cout<<"dhukse"<<endl;



int sockfd;
int bind_flag;
int sent_bytes;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
char buffer[1024], outputbuffer[1024];
int bytes_received;
socklen_t addrlen;
string my_ip;
vector<string> all_routers;
char nijer[100], onno[100];
string send_message;

map<string, pair<string, int> > routing_table;


void create_initial_routing_table();
int check_copy(string p);
void show_routing_table();


string create_message_from_table()
{
    string temp="";

    int loop = all_routers.size();
    sort(all_routers.begin(),all_routers.end());
    for(int i=0; i<loop; i++)
    {
        string now = all_routers[i];
        pair<string, int> p = routing_table[now];

        std::string cost = std::to_string(p.second);
        temp=temp+now+"_"+p.first+"_"+cost+"|";
    }
    return temp;
}

void extract_messsage_and_update(string message ,string source)
{

    cout<<"i am in extract message now ....\n";
    cout<<message<<" "<<"came from    "<<source<<endl;
    int loop = message.size();
    int i = 0;
    int cur=0;  ///cur 0hole dest ... 1 hole nh...2 hole cost

    string dest="" , nh ="" ,cost="";
    string temp="";

    while(i<loop)
    {
        if(message[i]=='|')
        {
            cost=temp;
            temp="";
            cur=0;


            cout<<dest<<" "<<nh<<" "<<cost<<endl;

            int real_cost = stoi(cost);
        }
        else if(message[i]=='_')
        {
            if(cur==0) ///dest paisi
            {
                dest=temp;
                temp="";
                cur=1;
            }
            else if(cur==1)///nh paisi
            {
                nh=temp;
                temp="";
                cur=2;
            }

        }
        else
        {
            temp+=message[i];
        }
        i++;
    }
return;
}



void call()
{
    bzero(buffer,1024);
    bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &client_address, &addrlen);

    //if (bytes_received != -1)puts(buffer);

    if (bytes_received != -1)
    {
        cout<<"balsal"<<endl;
        if(buffer[0]=='c')
        {
            balsal;
            if (strcmp(nijer, "192.168.10.1") == 0) strcpy(onno, "192.168.10.2"), strcpy(outputbuffer, "sent from 192.168.10.1");
            else if (strcmp(nijer, "192.168.10.2") == 0) strcpy(onno, "192.168.10.1"), strcpy(outputbuffer, "sent from 192.168.10.2");

            //onner kache pathabe
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(4747);
            inet_pton(AF_INET,onno,&server_address.sin_addr);

            printf("attempt to send to %s\n",onno);

            send_message = create_message_from_table();
            sent_bytes=sendto(sockfd, send_message.c_str(), 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
        }
        else
        {
            //puts(buffer);
            for(int i=0;i<10;i++)
            {
                unsigned char p = (unsigned char) buffer[i];
                cout<<"buffer["<<i<<"]  =  "<<p<<endl;
            }
            char found[100];
            strcpy(found,inet_ntoa(client_address.sin_addr));

            string message_came_from = string(found);
            string incoming_message = string(buffer);

            cout<<"amar jeitaase   "<<message_came_from<<endl;

            //memset(buffer,0,sizeof(buffer));

            extract_messsage_and_update(incoming_message, message_came_from);

        }


    }
    else
    {
        cout<<"kichu ase nai taiman -1"<<endl;

    }

}




int main(int argc, char *argv[])
{


    my_ip = string(argv[1]);
    cout<<"my ip address is "<<my_ip<<endl<<endl;


    create_initial_routing_table();
    show_routing_table();

    strcpy(nijer, argv[1]);

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(4747);
    //client_address.sin_addr.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET,argv[1],&client_address.sin_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind_flag = bind(sockfd, (struct sockaddr*) &client_address, sizeof(sockaddr_in));
    if(bind_flag==0)printf("successful bind\n");


    while (true)
    {
        call();
    }

    close(sockfd);

    return 0;

}

///fnction for showing routing table
void show_routing_table()
{
    int loop = all_routers.size();
    sort(all_routers.begin(),all_routers.end());
    cout<<"destination           next hop              cost"<<endl<<endl;
    for(int i=0; i<loop; i++)
    {
        string now = all_routers[i];
        pair<string, int> p = routing_table[now];
        cout<<now<<"          "<<p.first<<"      "<<p.second<<endl;
    }
    cout<<endl<<endl;
}

///create initial routing table
void create_initial_routing_table()
{

    file;
    string a,b;
    int c;
    all_routers.push_back(my_ip);
    routing_table[my_ip]=make_pair(my_ip,0);
    for(int i=0; i<5; i++)
    {
        cin>>a>>b;
        cin>>c;

        if(a[11]==my_ip[11] )
        {
            pair<string, int> temp = make_pair(b,c);
            routing_table[b] = temp;
            if(check_copy(b)==1)all_routers.push_back(b);
        }
        else if(b[11]==my_ip[11])
        {
            pair<string, int> temp = make_pair(a,c);
            routing_table[a] = temp;
            if(check_copy(a)==1)all_routers.push_back(a);
        }
        else
        {
            pair<string, int> temp = make_pair("not found yet", 1000000);
            if(routing_table[a].second==0)routing_table[a] = temp;
            if(routing_table[b].second==0)routing_table[b] = temp;

            if(check_copy(a)==1)all_routers.push_back(a);
            if(check_copy(b)==1)all_routers.push_back(b);
        }
    }
}
///check copy in all routers
int check_copy(string p)
{

    for(int i=0; i<all_routers.size(); i++)
    {
        if(p==all_routers[i])return 0;
    }
    return 1;
}


#include "table.cpp"

void route(){
    bzero(buffer,1024);
    addrlen=sizeof(sockaddr_in);
    bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &get_from, &addrlen);
    if (bytes_received != -1){

        strcpy(from,inet_ntoa(get_from.sin_addr));
        if (strcmp(from, "192.168.10.100") == 0){
            driverMsg();
        }
        else{
            routerMsg();
        }
    }
    else{
        cout<<"Recieve interrupt"<<endl;
    }
}


int main(int argc, char *argv[]){
    myIP=string(argv[1]);
    initTable(argv[2]);
    showTable();

    get_from.sin_family = AF_INET;
    get_from.sin_port = htons(4747);
    inet_pton(AF_INET,argv[1],&get_from.sin_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind_flag = bind(sockfd, (struct sockaddr*) &get_from, sizeof(sockaddr_in));
    if(bind_flag==0)printf("successful bind\n");

    while (true){
        route();
    }

    close(sockfd);
    return 0;

}


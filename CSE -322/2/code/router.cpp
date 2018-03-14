#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int sockfd;
int bind_flag;
int sent_bytes;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
char buffer[1024], outputbuffer[1024];
int bytes_received;
socklen_t addrlen;
char nijer[100], onno[100];

void neerjhor_kande()
{
    bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &client_address, &addrlen);

    if (bytes_received != -1) printf("%s\n", buffer);

////    printf("[%s:%hu]: %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
//
//    if (strcmp(inet_ntoa(client_address.sin_addr), "192.168.10.100") == 0)
//    {
//        printf("Received from driver\n");
//    }
//    else
//    {
//
//    }

    if (strcmp(nijer, "192.168.10.1") == 0) strcpy(onno, "192.168.10.2"), strcpy(outputbuffer, "sent from 192.168.10.1");
    else if (strcmp(nijer, "192.168.10.2") == 0) strcpy(onno, "192.168.10.1"), strcpy(outputbuffer, "sent from 192.168.10.2");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(4747);
    inet_pton(AF_INET,onno,&server_address.sin_addr);

    printf("nijer %s\n", nijer);
    printf("onno %s\n", onno);
    printf("attempt to send to %s\n", inet_ntoa(client_address.sin_addr));
    sent_bytes=sendto(sockfd, outputbuffer, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
}


int main(int argc, char *argv[])
{


    if(argc != 2)
    {
        printf("%s <ip address>\n", argv[1]);
        exit(1);
    }
    strcpy(nijer, argv[1]);

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(4747);
    //client_address.sin_addr.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET,argv[1],&client_address.sin_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind_flag = bind(sockfd, (struct sockaddr*) &client_address, sizeof(sockaddr_in));
    if(bind_flag==0)printf("successful bind");

    while (true)
    {
        neerjhor_kande();
    }

    close(sockfd);

    return 0;

}


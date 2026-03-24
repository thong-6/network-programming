#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
int main(){
    int reciver = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(reciver, (struct sockaddr *)&server_addr, sizeof(server_addr));
    char buf[256];
    while(1){
        int res = recvfrom(reciver, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, (socklen_t[]){sizeof(client_addr)});
        if(res > 0){
            buf[res] = '\0';
            printf("Received from client: %s\n", buf);
            sendto(reciver, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        }
    }
}
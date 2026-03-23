
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
int main(){
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server==-1){
        perror("socket error");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(server, (struct sockaddr *)&addr, sizeof(addr));
    if(res==-1){
        perror("bind error");
        return -1;
    }
    
    res = listen(server, 5);
    if(res==-1){
        perror("listen error");
        return -1;
    }
    
    while(1){
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client = accept(server, (struct sockaddr *)&client_addr, &client_addr_len);
        if(client==-1){
            perror("accept error");
            continue;
        }
        printf("client connected\n");
        char buffer[1024];
        int len = recv(client, buffer, sizeof(buffer)-1, 0);
        if(len==-1){
            perror("recv error");
        } else {
            buffer[len] = '\0';
            printf("Received from client: %s\n", buffer);
            char* msg = "Hello, client!";
            send(client, msg, strlen(msg), 0);}
        close(client);
    }
    
}
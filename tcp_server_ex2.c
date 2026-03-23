#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){

    if(argc != 4){
        fprintf(stderr,"Usage: %s <port> <greeting_file> <output_file>\n", argv[0]);
        return -1;
    }

    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server==-1){
        perror("socket error");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;

    int port = atoi(argv[1]);
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(server,(struct sockaddr *)&addr,sizeof(addr));
    if(res==-1){
        perror("bind error");
        return -1;
    }

    res = listen(server,5);
    if(res==-1){
        perror("listen error");
        return -1;
    }

    FILE *fp = fopen(argv[2],"r");
    if(fp == NULL){
        perror("fopen greeting file");
        return -1;
    }

    char file_content[1024];
    fgets(file_content,sizeof(file_content),fp);
    fclose(fp);

    while(1){

        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client = accept(server,(struct sockaddr *)&client_addr,&client_addr_len);

        if(client==-1){
            perror("accept error");
            continue;
        }

        printf("client connected\n");

        send(client,file_content,strlen(file_content),0);

        char buffer[1024];

        int len = recv(client,buffer,sizeof(buffer)-1,0);

        if(len > 0){
            buffer[len] = '\0';

            printf("Received from client: %s\n",buffer);

            FILE *fp = fopen(argv[3],"a");
            fprintf(fp,"%s",buffer);
            fclose(fp);
        }

        close(client);
    }
}
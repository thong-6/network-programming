#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[3]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return -1;
    }
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if(res==-1){
        perror("connect error");
        return -1;
    }
    char* msg = malloc(1024);
    if (!msg) {
        fprintf(stderr, "Memory allocation error\n");
        return -1;
    }
    fgets(msg, 1024, stdin);
    send(client, msg, strlen(msg), 0);
    char buffer[1024];
    int len = recv(client, buffer, sizeof(buffer)-1, 0);
    buffer[len] = '\0';
    printf("Received from server: %s\n", buffer);
    close(client);
    free(msg);

}
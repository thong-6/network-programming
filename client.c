#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8888

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    while(1) {
        char input[1024];
        printf("Enter a string (or 'exit' to quit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "exit") == 0) {
            break;
        }
        send(sock, input, strlen(input), 0);
    }

}
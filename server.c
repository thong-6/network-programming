#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024
int count_pattern(char *data, int len){
    int count = 0;
    for (int i = 0; i <= len - 10; i++) {
        if (strncmp(&data[i], "0123456789", 10) == 0) {
            count++;
        }
    }
    return count;
}

int main(){
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    bind(server_sock, (struct sockaddr *)&server, sizeof(server));
    listen(server_sock, 5);
    printf("Server is listening on port %d...\n", PORT);
    int client = accept(server_sock, NULL, NULL);
    char buffer[BUFFER_SIZE];
    char data[BUFFER_SIZE*2];
    int data_len = 0;
    int total_count = 0;
    int n;
    while ((n = recv(client, buffer, BUFFER_SIZE, 0)) > 0) {
        memcpy(data + data_len, buffer, n);
        data_len += n;
        int count = count_pattern(data, data_len);
        total_count += count;
        printf("Current count: %d\n", total_count);
        if(data_len > 9) {
            memmove(data, data + data_len - 9, 9);
            data_len = 9;
        }
    }
    close(client);
    close(server_sock);
    return 0;
}
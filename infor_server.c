#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


void recv_int(int sock, int* value) {
    recv(sock, value, sizeof(int), 0);
    *value = ntohl(*value);
}
void recv_long(int sock, long* value) {
    recv(sock, value, sizeof(long), 0);
    *value = ntohl(*value);
}
void recv_string(int sock, char *str){
    int len;
    recv_int(sock, &len);
    recv(sock, str, len, 0);
    str[len] = '\0';
}

int main() {

    int port = 8888;

    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr*)&server, sizeof(server));
    listen(server_sock, 5);

    printf("Server listening on port %d...\n", port);

    int client_sock = accept(server_sock, (struct sockaddr*)&client, &client_len);

    printf("Client connected!\n");

    // ===== nhận dữ liệu =====

    char cwd[256];
    recv_string(client_sock, cwd);
    printf("Current working directory: %s\n", cwd);
    int count;
    recv_int(client_sock, &count);
    printf("Number of files: %d\n", count);
    for (int i = 0; i < count; i++) {
        char name[256];
        long size;
        long mtime;
        recv_string(client_sock, name);
        recv_long(client_sock, &size);
        recv_long(client_sock, &mtime);
        printf("File: %s, Size: %ld bytes, Modified time: %ld\n", name, size, mtime);
    }
    close(client_sock);
    close(server_sock);
    return 0;
}
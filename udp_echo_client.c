#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>


int main(){
    int sender = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    char *message = "Hello, UDP Server!";
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    char buf[256];
    while(1){
        printf("Enter message to send (or 'exit' to quit): ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0'; // Remove newline character
        if(strcmp(buf, "exit") == 0){
            break;
        }
        sendto(sender, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));


        char recv_buf[256];
        int res = recvfrom(sender, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&server_addr, (socklen_t[]){sizeof(server_addr)});
        if(res > 0){
            recv_buf[res] = '\0';
            printf("Received from server: %s\n", recv_buf);
        }

    }
    close(sender);
    return 0;
}
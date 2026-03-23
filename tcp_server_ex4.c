#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

struct Student{
    char mssv[20];
    char name[100];
    char dob[20];
    float gpa;
};

int main(int argc, char* argv[]){

    if(argc != 3){
        fprintf(stderr,"Usage: %s <port> <log_file>\n", argv[0]);
        return -1;
    }

    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server,(struct sockaddr *)&addr,sizeof(addr));

    listen(server,5);

    while(1){

        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client = accept(server,(struct sockaddr *)&client_addr,&client_addr_len);

        struct Student sv;

        int len = recv(client,&sv,sizeof(sv),0);

        if(len > 0){

            char *client_ip = inet_ntoa(client_addr.sin_addr);

            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            char time_str[64];
            strftime(time_str,sizeof(time_str),"%Y-%m-%d %H:%M:%S",t);

            printf("%s %s %s %s %s %.2f\n",
                   client_ip,time_str,sv.mssv,sv.name,sv.dob,sv.gpa);

            FILE *fp = fopen(argv[2],"a");

            fprintf(fp,"%s %s %s %s %s %.2f\n",
                    client_ip,time_str,sv.mssv,sv.name,sv.dob,sv.gpa);

            fclose(fp);
        }

        close(client);
    }
}
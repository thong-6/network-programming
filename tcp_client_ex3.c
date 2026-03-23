#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct Student {
    char mssv[20];
    char name[100];
    char dob[20];
    float gpa;
};

int main(int argc, char *argv[]){

    if(argc != 3){
        fprintf(stderr,"Usage: %s <server_ip> <server_port>\n",argv[0]);
        return -1;
    }

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int res = connect(client,(struct sockaddr *)&addr,sizeof(addr));

    if(res == -1){
        perror("connect error");
        return -1;
    }

    struct Student sv;

    printf("MSSV: ");
    fgets(sv.mssv,sizeof(sv.mssv),stdin);

    printf("Ho ten: ");
    fgets(sv.name,sizeof(sv.name),stdin);

    printf("Ngay sinh: ");
    fgets(sv.dob,sizeof(sv.dob),stdin);

    printf("Diem TB: ");
    scanf("%f",&sv.gpa);

    send(client,&sv,sizeof(sv),0);

    printf("Data sent to server\n");

    close(client);
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
typedef struct {
    char name[256];
    long size;
    time_t mtime;
} FileInfo;
void send_int(int sock, int value){
    value = htonl(value);
    send(sock, &value, sizeof(value), 0);
}
void send_long(int sock, long value){
    value = htonl(value); 
    send(sock, &value, sizeof(value), 0);
}
void send_string(int sock, const char* str){
    int len = strlen(str);
    send_int(sock, len);
    send(sock, str, len, 0);
}
int main(){
    char cwd[256];
    getcwd(cwd,sizeof(cwd));
    DIR* dir = opendir(".");
    struct dirent* entry;
    struct stat st;
    FileInfo files[100];
    int count = 0;
    while((entry = readdir(dir))!=NULL){
        stat(entry->d_name,&st);
        if(S_ISREG(st.st_mode)){
            strcpy(files[count].name,entry->d_name);
            files[count].size = st.st_size;
            files[count].mtime = st.st_mtime;
            count++;
        }
    }
    closedir(dir);
    int client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    // Khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);
    // Ket noi den server
    if(connect(client,(struct sockaddr *)&addr,sizeof(addr))<0){
        perror("connect");
        return 1;       
    }
    send_string(client, cwd);
    send_int(client, count);
    for(int i=0;i<count;i++){
        send_string(client, files[i].name);
        send_long(client, files[i].size);
        send_long(client, files[i].mtime);
    }
    close(client);
    return 0;

}
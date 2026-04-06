#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Sử dụng: %s <port_s> <ip_d> <port_d>\n", argv[0]);
        return 1;
    }

    int port_s = atoi(argv[1]);
    const char *ip_d = argv[2];
    int port_d = atoi(argv[3]);

    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0)
    {
        printf("Lỗi tạo socket.\n");
        return 1;
    }

    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(port_s);

    if (bind(udp_socket, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        printf("Lỗi bind.\n");
        close(udp_socket);
        return 1;
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port_d);

    if (inet_pton(AF_INET, ip_d, &dest_addr.sin_addr) <= 0)
    {
        printf("Địa chỉ IP đích không hợp lệ!\n");
        close(udp_socket);
        return 1;
    }

    fd_set read_fds;
    int max_fd = udp_socket > STDIN_FILENO ? udp_socket : STDIN_FILENO;

    printf("Ứng dụng chat UDP đã khởi động!\n");
    printf("- Lắng nghe tại cổng: %d\n", port_s);
    printf("- Gửi dữ liệu đến: %s:%d\n", ip_d, port_d);
    printf("Nhập tin nhắn (gõ 'exit' để thoát):\n\n");

    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(udp_socket, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
            printf("Lỗi select.\n");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            char buffer[1024];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            {
                break;
            }

            if (strncmp(buffer, "exit", 4) == 0)
            {
                printf("Đóng ứng dụng chat.\n");
                break;
            }

            sendto(udp_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        }

        if (FD_ISSET(udp_socket, &read_fds))
        {
            char buffer[1024];
            struct sockaddr_in sender_addr;
            socklen_t sender_len = sizeof(sender_addr);

            int bytes_received = recvfrom(udp_socket, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&sender_addr, &sender_len);
            if (bytes_received > 0)
            {
                buffer[bytes_received] = '\0';
                printf("[Đối tác]: %s", buffer);
            }
        }
    }

    close(udp_socket);
    return 0;
}
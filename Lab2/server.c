// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 5005
#define FILENAME "test.bmp"
#define BUFSIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char buffer[BUFSIZE];
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);
    printf("Server is waiting for a connection on port %d...\n", PORT);

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("Client connected. Sending file...\n");

    // Send file size
    send(new_socket, &filesize, sizeof(filesize), 0);

    // Send file content
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFSIZE, fp)) > 0) {
        send(new_socket, buffer, bytes, 0);
    }

    printf("File sent successfully.\n");
    fclose(fp);
    close(new_socket);
    close(server_fd);
    return 0;
}


// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5005
#define BUFSIZE 1024
#define OUTPUT_FILE "received.bmp"

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFSIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Use localhost or change IP to server's address
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Receive file size
    long filesize;
    recv(sockfd, &filesize, sizeof(filesize), 0);
    printf("Receiving file of size: %ld bytes\n", filesize);

    FILE *fp = fopen(OUTPUT_FILE, "wb");
    if (!fp) {
        perror("Failed to create file");
        close(sockfd);
        return 1;
    }

    long received = 0;
    ssize_t bytes;
    while (received < filesize && (bytes = recv(sockfd, buffer, BUFSIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes, fp);
        received += bytes;
    }

    printf("File received successfully.\n");
    fclose(fp);
    close(sockfd);
    return 0;
}


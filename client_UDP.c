/* Nepovezaven UDP odjemalec */
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 4096
#define SERVPORTNO 50000
#define BUFFER_SIZE 1920 * 1080 * 3
#define BUFF_SIZE_SEND 1
void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    int fo;
    char *buffer;
    char *buffer_send;
    int packet;


    buffer = (char *)malloc(BUFFER_SIZE);
    buffer_send = (char *)malloc(BUFF_SIZE_SEND);

    if (argc != 2)
    {
        printf("usage: udpcli <IP SERVER address>\n");
        exit(1);
    }

    if ((fo = open("video", O_RDWR)) < 0)
    {
        printf("Napaka open video.\n");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        diep("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORTNO);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    while (1)
    {   
        printf("Sending packet...\n");
        // Send first packet to server to get response
        sendto(sockfd, buffer_send, BUFF_SIZE_SEND,0, (struct sockaddr *)&servaddr, sizeof(servaddr) );
        //printf("Waiting for packet.... \n");
        // Receive packet from server
        if ((packet = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL)) == -1)
        {
            diep("recvfrom()");
        }
        printf("Received packet: %d", packet);
        // Write received packet to FIFO
        if (write(fo, buffer, packet) ==-1)
        {
            printf("write err\n");
            exit(1);
        }
    }
    exit(0);
}
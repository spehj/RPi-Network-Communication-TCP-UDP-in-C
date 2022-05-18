/* Nepovezaven UDP streznik */
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
#define BUFF_REC_SIZE 1

void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len; 
    char *buffer;
    char *buffer_receive;
    int packet;
    int fi;

    
    buffer = (char *)malloc(BUFFER_SIZE);
    buffer_receive = (char *)malloc(BUFF_REC_SIZE);

    if (argc != 1)
    {
        printf("usage: udpcli <IP address>\n");
        exit(1);
    }

    if ((fi = open("video_test.h264", O_RDONLY)) < 0)
    {
        printf("Napaka open video.\n");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        diep("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORTNO);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        diep("bind");

    printf("WHILE\n");
    len = sizeof(cliaddr);
    while (1)
    {   
        // Waiting for first packet from client of size 1 to get the address
        printf("Waiting for packet...\n");
        recvfrom(sockfd, buffer_receive , BUFF_REC_SIZE, 0, (struct sockaddr *)&cliaddr,&len);
        // Read image bytes from FIFO to buffer (max number of bytes over UDP is 4096)
        packet = read(fi, buffer, MAXLINE);
        printf("Packet: %d\n", packet);
        // Send image bytes over ODP to client
        if (sendto(sockfd, buffer, packet, 0, (struct sockaddr*)&cliaddr, len) == -1)
            diep("sendto()");
    }
}
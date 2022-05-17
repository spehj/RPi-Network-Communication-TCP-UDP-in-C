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

void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int slen = sizeof(servaddr);
    char *buffer;
    int packet;
    int fi;

    // Alociramo spomin
    buffer = (char *)malloc(BUFFER_SIZE);

    if (argc != 2)
    {
        printf("usage: udpcli <IP address>\n");
        exit(1);
    }

    if ((fi = open("video_test.h264", O_RDONLY)) < 0)
    {
        printf("Napaka open video.\n");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        diep("socket");

    memset((char *)&servaddr, 0, sizeof(servaddr));
    // bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORTNO);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    // {
    //     fprintf(stderr, "inet_aton() failed\n");
    //     exit(1);
    // }
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        diep("bind");
    // }
    // sleep(1);
    while (1)
    {
        if (sendto(sockfd, buffer, BUFFER_SIZE, 0, &servaddr, slen) == -1)
            diep("sendto()");
        // while ((packet = read(fi, buffer, BUFFER_SIZE)) > 0)
        // {
        //     printf("Packet size: %d\n", packet);
        //     if (sendto(sockfd, buffer, packet, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) == -1)
        //         diep("sendto()");
        // }
        // len = sizeof(cliaddr);
        // n = recvfrom(sockfd,mesg,MAXLINE,0,(struct sockaddr *)&cliaddr,&len);
        // printf("client: %s:%d\n", inet_ntoa( cliaddr.sin_addr ), ntohs( cliaddr.sin_port));
        // memcpy(mesg,"s-> ",4);
    }
}
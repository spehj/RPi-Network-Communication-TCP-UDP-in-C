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
    struct sockaddr_in servaddr, cliaddr;
    // char sendline[ MAXLINE ], recvline[ MAXLINE ];
    int s, slen = sizeof(cliaddr);
    int fo;
    char *buffer;
    char *buffer_send;
    int packet;
    int fd;

    buffer = (char *)malloc(BUFFER_SIZE);
    buffer_send = (char *)malloc(BUFF_SIZE_SEND);

    if (argc != 2)
    {
        printf("usage: udpcli <IP address>\n");
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
    //memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORTNO);
    // First argument is server IP
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    // if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    // {
    //     fprintf(stderr, "inet_aton() failed\n");
    //     exit(1);
    // }

    // if (bind(s, &servaddr, sizeof(servaddr))==-1)
    //     diep("bind");

    printf("Before packet.\n");

    // packet = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);

    // printf("Packet: %d\n", packet);

    // while( (packet = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL)) > 0)
    // {
    //     printf("pack fd: %d\n", packet);
    //     if (write(fo, buffer, packet) < 0)
    //     {
    //         printf("napaka write\n");
    //         exit(1);
    //     }
    // }

    while (1)
    {   
        sendto(sockfd, buffer_send, BUFF_SIZE_SEND,0, (struct sockaddr *)&servaddr, slen );
        if ((packet = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, &cliaddr, &slen)) == -1)
        {
            diep("recvfrom()");
            printf("Received packet from \nData: \n\n");
        }

        if (write(fo, buffer, packet) ==-1)
        {
            printf("write err\n");
            exit(1);
        }
    }

    //   while (fgets(&sendline[4], MAXLINE,stdin) != NULL){
    //     memcpy(sendline,"c-> ", 4);
    //     fputs(sendline,stdout);
    //     n = sendto(sockfd,sendline,strlen(sendline),0,
    //             (struct sockaddr *)&servaddr,sizeof(servaddr));
    //     if( n == -1 ){
    //       perror("sento err");
    //       exit(1);
    //     }
    //     n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
    //     if( n == -1 ){
    //       perror("recvfrom err");
    //       exit(1);
    //     }
    //     recvline[n]=0;
    //     fputs(recvline,stdout);
    //   }

    // while ((packet = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL)) > 0)
    // {
    //     printf("pack: %d\n", packet);
    //     if (write(fo, buffer, packet) < 0)
    //     {
    //         printf("write err\n");
    //         exit(1);
    //     }
    // }
    exit(0);
}
/* Povezaven - TCP odjemalec */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
 

 
#define PORT_NUMBER 55000
 
 
int main( int argc, char **argv )//ODJEMALEC - PC
{
  int    n, sd, initClient( char * );
  char   buf[128];
  int fd;
  int packet;
  char *buffer;
  int buffer_size = 1280*1024*3;
 
  buffer = (char *)malloc(buffer_size);
 
  if( argc != 2 ){
    printf("Uporaba: %s <naslov streznika>\n", argv[0]);
    exit( 1 );
  }
 
  if ((fd = open("video", O_RDWR)) < 0){
    printf("Napaka open fifo file.\n");
    exit(1);
  }
 
  // vstavi ip server pc-ja
  if( (sd = initClient( argv[1] )) < 0 ){
    printf("Napaka init client.\n");  exit( 1 );
  }
  else{
    while( (packet = read(sd, buffer, buffer_size)) > 0 ){
      printf("Packet: %d\n", packet);
      if (write(fd, buffer, packet) < 0) {
        printf("Napaka write packet.\n");
        exit(1);
      }
   
    }
    close( sd );
  }
  exit( 0 );
}
 
 
int initClient( char *hostName )
{
  struct sockaddr_in  servAddr;
  struct hostent     *host;
  int    sd;
 
  if( ( host = gethostbyname( hostName )) == NULL) return( -1 );
  memset( &servAddr, 0, sizeof(servAddr));
  memcpy( &servAddr.sin_addr, host->h_addr, host->h_length );
  servAddr.sin_family = host->h_addrtype;
  servAddr.sin_port   = htons( PORT_NUMBER );
 
  printf("Streznik: %s, ", host -> h_name);
  printf("%s:%d\n", inet_ntoa( servAddr.sin_addr ), PORT_NUMBER);
  if( (sd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) return( -2 );
  if( connect(sd, (struct sockaddr *)&servAddr,sizeof(servAddr)) < 0) return( -3 );
  return( sd );
}
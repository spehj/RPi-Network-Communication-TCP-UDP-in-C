/* Povezaven - TCP streznik */
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
// 1. First run:
// raspivid -w 1280 -h 1024 -t 0 -o test.h264
// 2. 
// 


int main( int argc, char **argv )//STREŽNIK - RASPBERRY
{
  struct sockaddr_in cliAddr;
  socklen_t size;
 
  int       initServer( char * );
  int       sd, sn, n, fd;
  int buffer_size = 1024*1280*3;
  char *buff;

  buff = (char *)malloc(buffer_size);

  if( argc != 2 ){
    printf("Uporaba: %s <naslov streznika>\n", argv[0]);
    exit( 1 );
  }

  // vstavi ip server pc-ja
  if( (sd = initServer( argv[1] )) < 0){
    printf("Napaka: init server\n"); exit( 1 );
  }

  if ((fd = open("videoTest.h264", O_RDONLY))  < 0){
    printf("Napaka open fifo file.\n");
    exit(1);
  }

  printf("fd: %d\n", fd);
  
  listen( sd, 5 );
  alarm( 60 ); /* koncaj po eni minuti */

  while( 1 ){
    size = sizeof(cliAddr);
    memset( &cliAddr, 0, size );
    
    if( (sn = accept(sd, (struct sockaddr *)&cliAddr, &size)) < 0){
      printf("Accept error."); exit( 2 ); 
    }
    /* zveza je vzpostavljena, ustvari strezni proces */
    if( fork() == 0 ){
      printf("Odjemalec: %s:%d\n", inet_ntoa( cliAddr.sin_addr ), ntohs( cliAddr.sin_port ) );
      while( (n = read( fd, buff, buffer_size)) > 0 ){
        if( write(sn, buff, n) == -1)
          printf("Write to buffer error.\n");
      }
      printf("Odjemalec: %s:%d je prekinil povezavo.\n", inet_ntoa( cliAddr.sin_addr ), ntohs( cliAddr.sin_port ));
      close( sn );

      exit( 0 );
    }
  }
}

int initServer( char *hostName )
{
  struct sockaddr_in  servAddr; 
  struct hostent     *host;
  int    sd;

  if( (host = gethostbyname( hostName )) == NULL) return( -1 );
  memset( &servAddr, 0, sizeof(servAddr) );
  memcpy( &servAddr.sin_addr, host->h_addr, host->h_length );
  servAddr.sin_family  = host->h_addrtype;
  servAddr.sin_port    = htons( PORT_NUMBER );

  printf("Streznik: %s, ", host -> h_name);
  printf("%s:%d\n", inet_ntoa(servAddr.sin_addr), PORT_NUMBER );

  if( (sd = socket(AF_INET, SOCK_STREAM,0)) < 0 ) return( -2 );
  if( bind(sd, (struct sockaddr *)&servAddr, sizeof( servAddr )) < 0) return( -3 );
  printf("sd: %d \n", sd);
  return( sd );
}

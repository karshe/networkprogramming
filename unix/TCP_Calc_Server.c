/*****************************************
 * SERVER TO CALCULATE HEX
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * **************************************/

#include "networking.h"

int main(void){
 int port_no = 6000;
 int sockfd = TCP_SOCKET_BIND(port_no);
 int listenfd;
 int num;
 char hex[10];
 char integer[10];
 
 if(sockfd < 0) exit(1);
 
 printf("SERVER READY! \n");
 
 listenfd = TCP_SOCKET_LISTNER(sockfd, 5);
 TCP_SOCKET_CONNECTED_CLIENT(sockfd);
 
 strcpy(integer, TCP_READ_LISTENER(listenfd));
 
 printf("REQUESTED NUMBER TO CONVERT : %s\n", integer);

 num = atoi(integer);
 sprintf(hex, "%x", num);
 
 TCP_WRITE_LISTENER(listenfd, hex);	

 TCP_CLOSE_SOCKET(listenfd);
 TCP_CLOSE_SOCKET(sockfd);
 
 return 0;
}


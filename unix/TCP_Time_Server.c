/*****************************************
 * TCP BASED TIME SERVER
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * USING "networking.h"
 * **************************************/


#include "networking.h"
#include <time.h>

int main(void){
 int port_no = 6000;
 int sockfd = TCP_SOCKET_BIND(port_no);
 int listenfd;
 time_t tick;
 char time_str[255];
 
 if(sockfd < 0) exit(1);
 printf("TIME SERVER ACTIVATED ON PORT %d \n",port_no);
 
 listenfd = TCP_SOCKET_LISTNER(sockfd, 5);
 TCP_SOCKET_CONNECTED_CLIENT(sockfd);
 
 tick = time(NULL);
 snprintf(time_str, sizeof(time_str), "%s", ctime(&tick));
 TCP_WRITE_LISTENER(listenfd, time_str);	
 
 TCP_CLOSE_SOCKET(listenfd);
 TCP_CLOSE_SOCKET(sockfd);
 
 return 0;
}

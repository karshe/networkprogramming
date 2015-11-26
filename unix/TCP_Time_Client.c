/*****************************************
 * TCP BASED TIME CLIENT
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * USING "networking.h"
 * **************************************/

#include "networking.h"

int main(void){
 char host[] = "localhost";
 int port_no = 6000;
 
 int sockfd = TCP_SOCKET_CLIENT();
	if( TCP_CONNECT_SERVER(host, port_no, sockfd) ){
			 printf("CONNECTED WITH %s ON PORT %d \n", host, port_no);
				printf("TIME AT SERVER - %s\n", TCP_RECEIVE_SERVER(sockfd));
				TCP_CLOSE_SOCKET(sockfd);
		 }else{
			 printf("NO CONNECTION MADE WITH TIME SERVER ON PORT %d ", port_no);
		 }
	return 0;
 }

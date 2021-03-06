/*****************************************
 * LETS DO HEXA HEXA! 
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * USING "networking.h"
 * **************************************/

#include "networking.h"

int main(void){
 char host[] = "localhost";
 int port_no = 6000;
 char buffer[256];
 
 int sockfd = TCP_SOCKET_CLIENT();
	if( TCP_CONNECT_SERVER(host, port_no, sockfd) ){
			 printf("CONNECTED WITH CALCULATOR ON PORT %d \n", port_no);
				
				/* ASK FOR NUMBER */
				printf("GIVE NUMBER TO CONVERT : "); 
				bzero(buffer,256);
				fgets(buffer,255,stdin);
				TCP_SEND_SERVER(sockfd, buffer);
				
				printf("CONVERTED HEX IS - %s\n", TCP_RECEIVE_SERVER(sockfd));
				TCP_CLOSE_SOCKET(sockfd);
		 }else{
			 printf("NO CONNECTION MADE WITH SERVER ON PORT %d ", port_no);
		 }
	return 0;
 }

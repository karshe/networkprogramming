/*****************************************
 * LETS RUN COMMANDS!
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * **************************************/

#include "networking.h"

int main(void){
 int port_no = 6000;
 int sockfd = TCP_SOCKET_BIND(port_no);
 int listenfd;
 int firstRun = 1;
 char cmd[10];
 
 if(sockfd < 0) exit(1);
 
 printf("FILE READER SERVER READY! \n");
 
 listenfd = TCP_SOCKET_LISTNER(sockfd, 5);
 TCP_SOCKET_CONNECTED_CLIENT(sockfd);
 
 while(1){
		/* FIRST RUN SCRIPT */
		if(firstRun){
			 TCP_WRITE_LISTENER(listenfd, "WELCOME TO SERVER");
			 firstRun = 0;	
			
		}
		/* ACCEPT COMMANDS HERE*/
		strcpy(cmd, TCP_READ_LISTENER(listenfd));
		system(cmd);
		TCP_WRITE_LISTENER(listenfd, "EXECUTED!");
	 }

 TCP_CLOSE_SOCKET(listenfd);
 TCP_CLOSE_SOCKET(sockfd);
 
 return 0;
}


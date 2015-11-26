/*****************************************
 * TCP_Concurent_Server.c
 * A concurrent server using TCP.
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * **************************************/

/*****************************************
 * 
 * ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

void doprocessing (int sock) {
   int n;
   char buffer[256];
   bzero(buffer,256);
   n = read(sock,buffer,255);
   
   if (n < 1) {
      printf("ERROR IN READING CLIENT \n");
      exit(1);
   }
   
   printf("CLIENT SAYS: %s\n", buffer);
   
   n = write(sock,"THANKS!",18);
   
   if (n < 1) {
      printf("CAN'T WRITE ON SOCKET! \n");
      exit(1);
   }
	
}


int main(int argc, char *argv[]){
	
	int listen_sock; /* SOCKET TO LISTEN */
	int accept_sock; /* CONNECTION FROM CLIENT */
	struct sockaddr_in addr; /* SOCKET STRUCTURE */
	struct sockaddr_in listnerclient_addr;
	
	int server_port; /* SERVER PORT */
	int pid;
	socklen_t accept_socksize;
	
	/* SOCKET CREATION */
	listen_sock = socket(AF_INET, SOCK_STREAM, 0); 
	
	if(listen_sock < 0){
		perror("CAN'T CREATE SOCKET! \n");
		exit(1);
	}
	/* SOCKET CREATED SUCCESSFULLY */
	printf("SOCKET CREATED! WAITING FOR BIND \n");
	
	/* WE NEED TO BIND IT ON SOME PORT */
	/* YOU CAN USE BZERO ALSO! NULIFIED EVERYTHING*/
	memset(&addr, 0, sizeof(addr));
	server_port = 6000;
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(server_port);
	
	if( bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		/* UNSUCCESSFUL BINDING */
		perror("BINDING ERROR! \n");
		close(listen_sock); /* CLOSE SOCKET */
		exit(1);
	}
	
	/* BINDIND SUCCESSFULLY */
	printf("SOCKET BINDED TO PORT %d \n", server_port);
	
	if( listen(listen_sock, 5) < 0 ){
		/* UNSUCCESSFUL LISTENING ATTEMPT */
		perror("LISTEN ERROR! \n");
		close(listen_sock); /* CLOSE SOCKET */
		exit(1);
	}
	
	/* LISTENING SUCCESSFULLY */
	printf("SERVER READY \n");
	
	while(1){
		/* WAITING FOR NEW CONNECTIONS */
		printf("SERVER WAITING NOW CLIENT TO GET CONNECTED! \n");
		accept_socksize = sizeof(listnerclient_addr);
		accept_sock = accept(listen_sock, (struct sockaddr *)&listnerclient_addr, &accept_socksize);
		printf("CONNECTED TO - %s\n", inet_ntoa(listnerclient_addr.sin_addr));
		
		/* CREATE CHILD PROCESS TO SERVE CLIENT */
		pid = fork();
		
		if(pid < 0){
			perror("FORKING ERROR!");
			exit(0);
		}
		
		if(pid == 0){
			close(listen_sock);
			doprocessing(accept_sock);
			exit(0);
		}else{
			close(accept_sock);
		}
	}
	
	printf("TURNING OFF SERVER NO MORE REQUESTS!");
	close(listen_sock);
	return 0;
}

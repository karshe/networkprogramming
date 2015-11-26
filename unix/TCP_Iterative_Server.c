/*****************************************
 * An iterative server using TCP.
 * by https://github.com/karshe
 * PROJECT : NETWORK PROGRAMMING IN UNIX
 * **************************************/

/*****************************************
 * ONE CLIENT AT A TIME, MAX OF DEFINED IN
 * client_conn VARIABLE
 * RECEIVES DATA FROM CLIENT n AND SAYS 
 * BYE AND WAIT FOR n+1 CLIENT  
 * ***************************************/

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]){
	
	int listen_sock; /* SOCKET TO LISTEN */
	int accept_sock; /* CONNECTION FROM CLIENT */
	char buffer[256]; /* BUFFER */
	struct sockaddr_in addr; /* SOCKET STRUCTURE */
	struct sockaddr_in listnerclient_addr;
	
	int server_port; /* SERVER PORT */
	int client_conn = 2;
	int i;
	int chunk;
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
	
	for(i=0; i<client_conn; i++){
		accept_socksize = sizeof(listnerclient_addr);
		accept_sock = accept(listen_sock, (struct sockaddr *)&listnerclient_addr, &accept_socksize);
		
			if(accept_sock < 0){
				perror("ACCEPT SOCK ERROR!");
				exit(1);
			}	
			printf("WAITING FOR CLIENT %d \n", i+1);
		
			bzero(buffer, sizeof(buffer));
			chunk = read(accept_sock, buffer, sizeof(buffer));
			
			if(chunk < 1){
				perror("CHUNK CAN'T BE READ FROM CLIENT");
				exit(1);
			}
			
			printf("CLIENT %d SAID : %s", i+1, buffer);
			
			bzero(buffer, sizeof(buffer));
			strcpy(buffer, "THANKS FOR CONNECTING ME!");
			
			if(write(accept_sock, buffer, sizeof(buffer)) < 1){
				printf("CAN'T SEND TO CLIENT %d", i+1);
				exit(1);
			}
			
			printf("BYE CLIENT %d\n", i+1);
			close(accept_sock);
	}
	printf("TURNING OFF SERVER NO MORE REQUESTS!");
	close(listen_sock);
	return 0;
}

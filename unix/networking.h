/**********************************************************
 * NETWORKING.H
 * COLLECTION OF TCP/UDP BASED SERVER-CLIENT SOCKET CODES
 * *******************************************************/

#ifndef NETWORKING_H_  
#define NETWORKING_H_

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define author "https://github.com/karshe/unixnetworkprograms"

/***************************************
 * DO INCLUDE FOLLOWING HEADER FILES
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <netdb.h>
 * #include <netinet/in.h>
 * #include <sys/socket.h>
 * #include <unistd.h>
 * #include <string.h>
 * *************************************/
 
/* TCP FAMILY IPV4 TYPE */

/****************************************
 * CREATE SOCKET BINDED TO PORT - SERVER
 * RETURN SOCKET FILE DESC
 * *************************************/

int TCP_SOCKET_BIND(int portno){
		int sockfd;
		struct sockaddr_in serv_addr;
		
		sockfd = socket(AF_INET, SOCK_STREAM, 0); 
		 if(sockfd < 0){
			 perror("ERROR IN CREATING SOCKET!");
			 exit(1);
		 }
		 bzero((char *) &serv_addr, sizeof(serv_addr));
		 serv_addr.sin_family = AF_INET; /*TCP*/
		 serv_addr.sin_addr.s_addr = INADDR_ANY; 
		 serv_addr.sin_port = htons(portno); /* PORT */
		 
		 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
			  perror("CAN'T BIND THE SOCKET!");
			  exit(1);
		   }
		   
		  return sockfd;
	}


/****************************************
 * CREATE LISTENER TO SOCKET WITH BACKLOG - SERVER
 * RETURN LISTENER SOCKET FILE DESC
 * *************************************/
 
int TCP_SOCKET_LISTNER(int sockfd, int backlog){
	 int listnerfd;
	 struct sockaddr_in listnerclient_addr;
	 socklen_t listnersize;
	 
	 listen(sockfd, backlog); 
	 listnersize = sizeof(listnerclient_addr);
	 listnerfd = accept(sockfd, (struct sockaddr *)&listnerclient_addr, &listnersize);
	 
	 if(listnerfd < 0){
		 perror("CAN'T ACCEPT CONNECTION FROM CLIENT!");
		 exit(1);
	 }
	 return listnerfd;
}

/****************************************
 * READ FROM LISTENER SOCKET - SERVER
 * RETURN BUFFER READ FROM CLIENT
 * *************************************/
 
char *TCP_READ_LISTENER(int lisenfd){
	int t;
	static char buffer[256];
	
	bzero(buffer, 256);
	t = read(lisenfd, buffer, 255);
	
	if(t < 1){
		 printf("CAN'T READ FROM CLIENT!");
		 exit(1);
	}
	return(buffer);
}

/****************************************
 * WRITE ON SOCKET - SERVER
 * SENDS DATA TO CLIENT
 * *************************************/
 
void TCP_WRITE_LISTENER(int listenfd, char buff[]){
		int t;
		char buffer[256];
		bzero(buffer, 256);
		strcpy(buffer, buff);
		t = write(listenfd, buffer, strlen(buffer)); /* WRITE ON CLIENT SOCKET */
		 if(t < 1){
			 printf("I CAN'T WRITE ON SOCKET!");
			 exit(1);
		 }
}

/****************************************
 * GET INFO OF CLIENT CONNECTED - SERVER
 * PRINTS CLIENT CONNECTED TO WHICH PORT
 * *************************************/
 
void TCP_SOCKET_CONNECTED_CLIENT(int sockfd){
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1){
		perror("I WAS UNABLE TO CALL getsockname");
		exit(1);
	}else{
		printf("CLIENT CONNECTED AT PORT %d\n", ntohs(sin.sin_port));
	}
}

/****************************************
 * CUSTOM COMMAND FOR CHAT SERVER
 * *************************************/
 int TCP_SOCKET_CLIENT_CONNECTED(int sockfd){
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1){
		perror("I WAS UNABLE TO CALL getsockname");
		exit(1);
	}else{
		printf("CLIENT CONNECTED AT PORT %d\n", ntohs(sin.sin_port));
		return 0;
	}
}

/****************************************
 * CLOSES SOCKET
 * *************************************/
 
void TCP_CLOSE_SOCKET(int sockid){
	shutdown(sockid, 1);
	}

/****************************************
 * CREATE SOCKET FOR - CLIENT
 * RETURN SOCKET FILE DESC
 * *************************************/
int TCP_SOCKET_CLIENT(){
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	 if(sockfd < 0){
		 perror("ERROR IN CREATING SOCKET!");
		 exit(1);
	 }
	 return sockfd;
}

/****************************************
 * CONNECTS TO SERVER (LOCATION, PORT NO, CLIENT SOCKET)
 * RETURN 1 IF CONNECTION MADE SUCCESSFULL
 * *************************************/

int TCP_CONNECT_SERVER(char loc[], int portno, int socketfd){
	struct sockaddr_in client_addr;
	struct hostent *server;
	char hostloc[256];
	bzero(hostloc, 256);
	strcpy(hostloc, loc);
	
	server = gethostbyname(hostloc);
	
	if(server == NULL){
		printf("CAN'T CONNCET TO SPECIFIED SERVER");
		exit(0);
	}
	
	bzero((char *) &client_addr, sizeof(client_addr)); /* NULLIFIED! */
	client_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&client_addr.sin_addr.s_addr, server->h_length);
	client_addr.sin_port = htons(portno);
	
	if (connect(socketfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
      return 0;
   }
   
   return 1;
}

/****************************************
 * SENDS DATA TO SPECIFIED SOCKET - FROM CLIENT TO SERVER
 * RETURNS NOTHING
 * *************************************/

void TCP_SEND_SERVER(int socketfd, char buff[]){
		int t;
		char buffer[256];
		bzero(buffer, 256);
		strcpy(buffer, buff);
		t = write(socketfd, buff, strlen(buff)); 
		
		  
		if (t < 0) {
		  printf("\nCLIENT WAS UNABLE TO WRITE ON SERVER SOCKET!");
		  exit(1);
		}
	
}

/****************************************
 * RECEIVE DATA FROM SERVER - CLIENT
 * RETURNS DATA
 * *************************************/

char *TCP_RECEIVE_SERVER(int socketfd){
		int t;
		static char buffer[256];
		bzero(buffer,256);
		
		/* READ FROM SERVER */
		t = read(socketfd, buffer, 255);

		if (t < 1) {
		  printf("CONNECTION BETWEEN SERVER AND CLIENT LOST!");
		  exit(1);
		}
		
		return(buffer);
}

#endif //

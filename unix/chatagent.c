/********************************************
 * CHAT AGENT 
 * BASED ON TCP 
 * DEVELOPED IN UNIX BY SOCKET PROGRAMMING
 * ******************************************/

#include "networking.h"

int ChatAgent(){

 /*CREATE SOCKET AND BIND IT TO SOME PORT*/
 int sockfd;
 int port_no;
  
 int doingChat = 0;

 /* CHAT COMMANDS */
 char cmd[10];
 
 char buffer[256];
 char choice;
 
 do{
	 
	 /* HE IS NOT DOING CHAT RIGHT NOW */
	 if(!doingChat){
		 printf("YOU ARE NOT CONNECTED TO ANY USER.");
		 printf("USE (Chat) COMMAND TO START CHAT OR USE (Help) TO HELP \n");
		 printf("CMD (chat-agent)>> ");
		 fgets(cmd, sizeof(cmd), stdin);
		 
		 if(strcmp(cmd, "Chat\n") == 0 || strcmp(cmd, "chat\n") == 0){
			 printf("SPECIFY HIS PORT (chat-agent)>> ");
			 scanf("\n%d%*c", &port_no);
			 sockfd = TCP_SOCKET_CLIENT();
			 
			 do{
				 if( TCP_CONNECT_SERVER("localhost", port_no, sockfd) ){
					 printf("CONNECTED WITH %s ON PORT %d \n", "localhost", port_no);
					 while(1){
						printf("MESSAGE (chat-agent)>> ");
						bzero(buffer,256);
						fgets(buffer,255,stdin);
						
						TCP_SEND_SERVER(sockfd, buffer);
						printf("REPLY (chat-agent)>> %s\n", TCP_RECEIVE_SERVER(sockfd));
					}
					if(sockfd > 0) { TCP_CLOSE_SOCKET(sockfd); }
				 }else{
					 printf("NO CONNECTION MADE WITH SERVER. DO YOU WANT TO RETRY (Y/N) : ");
					 scanf("\n%c%*c", &choice);
				 }
			 }while(choice == 'y' || choice == 'Y');
			 
			 printf("Command (Bye, Quit) ? : ");
			 fgets(cmd, sizeof(cmd), stdin);
			 //printf("COMPARE VALUE : %d", strcmp(cmd, "quit\n"));
			 
			 if(strcmp(cmd, "Bye\n") == 0 || strcmp(cmd, "bye\n") == 0){
				 printf("WE DISCONNECTED YOU FROM %d PORT. \n", port_no);
			 }else if(strcmp(cmd, "Quit\n") == 0 || strcmp(cmd, "quit\n") == 0){
				 printf("SIGNING OFF \n");
				 break;
			 }else{
				 printf("Didn't got you! Try again! \n");
			 }
			 
		 }else if(strcmp(cmd, "Help\n") == 0 || strcmp(cmd, "help\n") == 0){
				 printf("HELP MENU. \n");
				 continue;
		 }else if(strcmp(cmd, "Quit\n") == 0 || strcmp(cmd, "quit\n") == 0){
				 printf("SIGNING OFF. \n");
				 break;
		 }else{
			 printf("YOU ARE ALONE! WAIT UNTIL SOMEONE CONNECTS YOU! \n");
			 printf("... \n");
		 }
	 }
	 
	 
 }while(1);
 
 printf("THANKS FOR USING COMMAND LINE CHAT APP!");
 return 0;
}

int ChatServer(int port_no){
	int sockfd = TCP_SOCKET_BIND(port_no);
	int listenfd;
	char buffer[256];

	if(sockfd < 0) exit(1);
	printf("SERVER RUNNING WITH CLIENT ON PORT %d \n",port_no);

	listenfd = TCP_SOCKET_LISTNER(sockfd, 5);
	TCP_SOCKET_CONNECTED_CLIENT(sockfd);

	while(1){
	 printf("CLIENT (chat-agent)>> ");
	 printf("%s", TCP_READ_LISTENER(listenfd));
	 
	 printf("YOU (chat-agent)>> ");
	 bzero(buffer,256);
	 fgets(buffer,255,stdin);
	 TCP_WRITE_LISTENER(listenfd, buffer);	
	}

	TCP_CLOSE_SOCKET(listenfd);
	TCP_CLOSE_SOCKET(sockfd);

	return 0;
}

int rand_range(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

int main(int argc, char *argv[]){
	char s;
	int rand_port;
	
	printf("CHAT AGENT v0.0.1 \n");
	
	do{
		printf("WHAT YOU WANT TO SERVE! \n");
		printf("1. CHAT CLIENT (TALK TO PORT) \t 2. CHAT SERVER (LISTEN TO PORT) \t 9. QUIT\n");
		printf("OPTION (chat-app)>> ");
		fflush(stdin);
		scanf(" %c%*c", &s);
		
		if(s == '1'){
			printf("... \n");
			printf("ACTIVATED CHAT CLIENT\n");
			printf("... \n");
			ChatAgent();
					
		}else if(s == '2'){
			printf("... \n");
			printf("ACTIVATED CHAT SERVER\n");
			printf("... \n");
			rand_port = rand_range(2000, 5000);
			printf("YOUR PORT IS (chat-app)>> %d \n", rand_port);
			ChatServer(rand_port);
		
		}else{
			continue;
		}
	}while(s != '9');
	
	return 0;
}

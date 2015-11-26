#include "networking.h"

int main(void){
 char host[] = "localhost";
 int port_no = 4000;
 char buffer[256];
 char choice;
 
 int sockfd = TCP_SOCKET_CLIENT();
 
 do{
	 if( TCP_CONNECT_SERVER(host, port_no, sockfd) ){
		 printf("CONNECTED WITH %s ON PORT %d \n", host, port_no);
		 while(1){
			printf("YOUR MESSAGE - "); 
			bzero(buffer,256);
			fgets(buffer,255,stdin);
			
			TCP_SEND_SERVER(sockfd, buffer);
			printf("SERVER REPLIED - %s", TCP_RECEIVE_SERVER(sockfd));
		}
		if(sockfd > 0) { TCP_CLOSE_SOCKET(sockfd); }
	 }else{
		 printf("NO CONNECTION MADE WITH SERVER. DO YOU WANT TO RETRY (Y/N) : ");
		 scanf("\n%c%*c", &choice);
	 }
 }while(choice == 'y' || choice == 'Y');
 
 return 0;
}

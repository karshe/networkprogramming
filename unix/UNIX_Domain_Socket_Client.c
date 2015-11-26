 #include "n_headers.h"

 #define DATA "NETWORK PROGRAMMING IS FUN!"
 
 int main(int argc, char *argv[]){
     int sock;
     struct sockaddr_un server;

     sock = socket(AF_UNIX, SOCK_STREAM, 0);
     if (sock < 0) {
         perror("CAN'T OPEN UNIX STREAM SOCKET");
         exit(1);
     }
     
     server.sun_family = AF_UNIX;
     strcpy(server.sun_path, "UN_SOCK"); /* FROM SERVER */
     
     if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
         close(sock);
         perror("ERROR IN CONNECTION WITH SERVER! ");
         exit(1);
     }
     if (write(sock, DATA, sizeof(DATA)) < 0){
         perror("WRITING ERROR ON SERVER SOCKET! ");
         exit(1);
	 }
	 
	 printf("WROTE TO SERVER!");
	 
     close(sock);
     return 0;
 }

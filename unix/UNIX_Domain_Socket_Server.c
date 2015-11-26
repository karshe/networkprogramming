 #include "n_headers.h"
 
 #define NAME "UN_SOCK"
 
 int main(int argc, char *argv[]){
     int sock, msgsock, rval;
     struct sockaddr_un server;
     char buf[1024];
     sock = socket(AF_UNIX, SOCK_STREAM, 0);
     if (sock < 0) {
         perror("CAN'T OPEN UNIX STREAM SOCKET");
         exit(1);
     }
     
     server.sun_family = AF_UNIX;
     strcpy(server.sun_path, NAME);
     
     if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
         perror("ERROR IN BINDING SOCKET!");
         exit(1);
     }
     
     printf("SOCKET NAME : %s\n", server.sun_path);
     listen(sock, 5);
     
     for (;;) {
         msgsock = accept(sock, 0, 0);
         if (msgsock == -1)
             perror("ERROR IN ACCEPTING");
         else do {
             bzero(buf, sizeof(buf));
             if ((rval = read(msgsock, buf, 1024)) < 0)
                 perror("READING CLIENT ERROR!");
             else if (rval == 0)
                 printf("CONNECTION TERMINATED\n");
             else
                 printf("CLIENT SAYS - %s\n", buf);
         } while (rval > 0);
         close(msgsock);
     }
     
     close(sock);
     unlink(NAME);
     return 0;
 }


/******************************************
OUTPUT 
SOCKET NAME : UN_SOCK
CLIENT SAYS - NETWORK PROGRAMMING IS FUN!
CONNECTION TERMINATED
******************************************/

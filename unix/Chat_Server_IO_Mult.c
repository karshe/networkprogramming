/***********************************
 * IO BASED MULTIPLEXING
 * NO NEED FOR WAIT
 * USES SELECT()
 * *********************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "networking.h"

#define PORT "4000"   // port we're listening on

/* HELPER FUNCTION FOR RETURNING IPv4 or IPv6 HOST ADDRESS */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    fd_set master;    /* ALL CLIENT WILL BE SAVED HERE */
    fd_set read_fds;  /* TEMP FD LIST FOR SELECT CALL */
    int fdmax;        /* MAXIMUM CLIENT FD */

    int listener;     /* LISTENER SOCKET */
    int newfd;        /* CLIENT SOCKET */
    struct sockaddr_storage remoteaddr; /* CLIENT ADDRESS */
    socklen_t addrlen;

    char buf[256];    /* BUFFER FOR CLIENT DATA */
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1; 
    /* i,j LOOPER, rv DATA CHUNKS RECIVIED COUNTER */
    int i, j, rv;

    struct addrinfo hints, *ai, *p;
    
    /* STEP 00 : CLEAT ALL FDS, MASTER + TEMP SETS */
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    /* STEP 01 : GET US A SOCKET & BIND IT */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* IPV4 OR IPV6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "ERROR: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    /* GETTING ALL LISTENERS */
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        
        // IF SOCKET CREATION FAILS
        if (listener < 0) { 
            continue;
        }
        
        // SUPRESS "address already in use" ERROR
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        
        // IF BINDING FAILS
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // IN CASE WE DIDN'T BINDED AT ALL
    if (p == NULL) {
        fprintf(stderr, "FAILED TO BIND!\n");
        exit(2);
    }

    freeaddrinfo(ai);

    /* LISTENING... */
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }
    
    printf("SERVER UP AND RUNNING \n");

    /* ADD IT TO MASTER SET */
    FD_SET(listener, &master);

    /* TRACK MAX_LISTENER */
    fdmax = listener;

    /* LOOP */
    for(;;) {
        
        /* COPY OF MASTER SET */
        read_fds = master;
        
        /* SELECT FD FROM SET */
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        
        /* RUNNING THROUGH EXISTING CONNECTIONS */
        /* LOOKING FOR DATA! */
        for(i = 0; i <= fdmax; i++) {
            
            if (FD_ISSET(i, &read_fds)) {
                /* LOOK, WE GOT ONE! */
                if (i == listener) {
                    
                    /* ACCEPT THE CLIENT CONNECTION */
                    addrlen = sizeof(remoteaddr);
                    newfd = accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        /* CONNECTION ESTABLISHED */
                        FD_SET(newfd, &master); /* ADDED TO MASTER DATA */
                        
                        if (newfd > fdmax) {    /* KEEP TRACK OF MAX */
                            fdmax = newfd;
                        }
                        
                        printf("NEW CONNECTION FROM %s SOCKET %d\n",inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
                        
                    }
                } else {
                    
                    /* HANDLE DATA FROM CLIENT */
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        
                        /* EITHER CONNECTION IS CLOSED OR CLIENT SHUT DOWN */
                        if (nbytes == 0) {
                            /* CONNECTION TERMINATED */
                            printf("INFO: CLIENT AT SOCKET %d HUNGED UP\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, &master); 
                    } else {
                        /* GOT DATA FROM CLIENT */
                        /* j = ALL CONNECTION TO SERVER */
                        /* i = WE! THE SERVER */
                        /* listener = CLIENT WHO SEND THE DATA! */
                        
                        for(j = 0; j <= fdmax; j++) {
                            /* BROADCAST IT, EVERYONE! */
                            if (FD_ISSET(j, &master)) {
                                /* TO ALL OTHER CLIENTS */
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        } 
					}                  
                    
                }
            }
        }
    }/* EVERYTHING HAS END, EVEN OUR SUN HAS! */
    
    return 0;
}

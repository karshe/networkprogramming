/***********************************************
*** SERVER APPLICATION
*** https://github.com/karshe
***********************************************/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

/* #pragma comment indicates to the linker that the Ws2_32.lib file is needed. */
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int ServerApp(){
    printf("SERVER APPLICATION STARTED!\n");

    /** STEP 0 : VARIABLES **/
    WSADATA wsaData; //Information of Windows Socket
    int iResult;

    //Socket addrinfo structure
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    //Sockets
    SOCKET ListenSocket = INVALID_SOCKET; /* SOCKET object for the server to listen for client connections. */
    SOCKET ClientSocket = INVALID_SOCKET; /* CLIENT LISTENER */

    //During communication
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    /** STEP 1 : INIT WINSOCK
    Call WSAStartup and return its value as an integer and check for errors.
    WSAStartup function is called to initiate use of WS2_32.dll.
    **/

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup FAILED WITH ERROR: %d\n", iResult);
        return 1;
    }

    /** STEP 2 : SOCKET ADDR INIT
    After initialization, a SOCKET object must be instantiated for use by the server.
    AF_INET is used to specify the IPv4 address family.
    SOCK_STREAM is used to specify a stream socket.
    IPPROTO_TCP is used to specify the TCP protocol .
    AI_PASSIVE flag indicates the caller intends to use the returned socket address structure in a call to the bind function.
    **/
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /** Resolve the local address and port to be used by the server **/
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("RESOLUTION FAILED! RETURN CODE : %d\n", iResult);
        WSACleanup();
        return 1;
    }

    /** STEP 3 : SOCKET OBJECT CREATION
    Call the socket function and return its value to the
    ListenSocket variable. For this server application,
    use the first IP address returned by the call to getaddrinfo
    that matched the address family, socket type,
    and protocol specified in the hints parameter.
    **/
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("ERROR IN SOCKET CREATION!\n %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    printf("SOCKET CREATED, NOW BINDING TO PORT "DEFAULT_PORT"\n");

    /** STEP 4 : BIND SOCKET
    The sockaddr structure holds information regarding the address family,
    IP address, and port number.
    Call the bind function, passing the created socket and sockaddr structure
    returned from the getaddrinfo function as parameters.
    **/
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("BINDING UNSUCCESSFUL! ERROR: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    /** Once the bind function is called, the address information
    returned by the getaddrinfo function is no longer needed.
    **/
    freeaddrinfo(result);
    printf("BINDED TO "DEFAULT_PORT"\n");

    /** STEP 5 : START LISTENING
    Call the listen function, passing as parameters the
    created socket and a value for the backlog, maximum
    length of the queue of pending connections to accept.
    In this example, the backlog parameter was set to SOMAXCONN.
    This value is a special constant that instructs the
    Winsock provider for this socket to allow a maximum
    reasonable number of pending connections in the queue.
    **/
    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "LISTEN FAILED WITH ERROR : %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    /** STEP 6 : ACCEPT CONNECTION **/

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("ACCEPT FAILED WITH CODE : %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    /** STEP 7 : LETS TALK!
    ** Receive until the peer shuts down the connection
    **/
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("BYTES GOT FROM CLIENT: %d\n", iResult);
            printf("CLIENT SAID :  %s\n", recvbuf);

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, "THANK YOU CLIENT!", sizeof("THANK YOU CLIENT!"), 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("SEND FAILED: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("BYTES SEND TO CLIENT : %d\n", iSendResult);
        } else if (iResult == 0)
            printf("CONNECTION CLOSING...\n");
        else {
            printf("LOOKS LIKE CLIENT HUNGED UP!\nSTATUS CODE : %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    /** STEP 9 : SHUTDOWN SOCKET **/
     iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("SHUTDOWN FAILED : %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    /** STEP 10 : CLEANUP! **/
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

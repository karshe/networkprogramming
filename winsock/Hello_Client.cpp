/***********************************************
*** CLIENT APPLICATION
*** https://github.com/karshe
***********************************************/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

/* #pragma comment indicates to the linker that the Ws2_32.lib file is needed. */
#pragma comment(lib, "Ws2_32.lib")

/* Every socket is binded to some port */
#define DEFAULT_PORT "27015"
#define DEFAULT_SERVERHOST "127.0.0.1"
#define DEFAULT_BUFLEN 512

int ClientApp(){
    printf("CLIENT APPLICATION STARTED!\n");

    WSADATA wsaData; /* Create WSADATA object */

    int iResult;

    /* Initialize Winsock */
    /* MAKEWORD(2,2)
    ** parameter of WSAStartup makes a request
    ** for version 2.2 of Winsock on the system
    */
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup FAILED WITH ERROR: %d\n", iResult);
        return 1;
    }

    printf("INITIALIZATION SUCCESSFUL WITH WSAStartup = %d\n", iResult);
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_SERVERHOST, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo FAILED WITH CODE : %d\n", iResult);
        WSACleanup();
        return 1;
    }
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr=result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("ERROR at socket() WITH CODE: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Connect to server.
    iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("UNABLE TO CONNECT TO SERVER!\n");
        WSACleanup();
        return 1;
    }

    /*** SEND & RECEIVE FROM CLIENT ***/
    int recvbuflen = DEFAULT_BUFLEN;

    char *sendbuf = "HELLO SERVER!";
    char recvbuf[DEFAULT_BUFLEN];

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("SEND FAILED: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("BYTES SENT : %ld\n", iResult);

    // Receive data until the server closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0){
            printf("BYTES RECEIVED : %d\n", iResult);
            printf("SERVER SAID :  %s\n", recvbuf);
        }
        else if (iResult == 0)
            printf("CONNECTION CLOSED!\n");
        else
            printf("recv FAILED WITH CODE : %d\n", WSAGetLastError());
    } while (iResult > 0);
    /** END LOGIC  ***/

    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("SHUTDOWN FAILED WITH CODE : %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;

}


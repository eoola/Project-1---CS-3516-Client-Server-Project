#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#define MAXPENDING 5 /* Maximum outstanding connection requests */
#define BUFFER_SIZE 1024
void DieWithError(char *errorMessage); /* Error handling function */
void HandleTCPClient(int clntSocket); /* TCP client handling function */

int main(int argc, char *argv[])
{ int servSock; /*Socket descriptor for server */
    int clntSock; /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort; /* Server port */
    unsigned int clntLen; /* Length of client address data structure */
    if (argc != 2) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }
    echoServPort = atoi(argv[1]); /* First arg: local port */
/* Create socket for incoming connections */
    if ((servSock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

/* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort); /* Local port */
/* Bind to the local address */
    if (bind (servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr))<0)
        DieWithError("bind() failed");
/* Mark the socket so it will listen for incoming connections */
    if (listen (servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;) /* Run forever */
    {
/* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr); /* Wait for a client to connect */
        if ((clntSock = accept (servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");
/* clntSock is connected to a client! */
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        HandleTCPClient(clntSock);
    }

}
void DieWithError(char *errorMessage)
{

    printf(errorMessage);
    fflush(stdout);

}
void HandleTCPClient(int clntSocket) /* TCP client handling function */
{
    char other[50];
    char otherTwo[50];
    char file[50];

    char response[59500];
    char buf[BUFFER_SIZE];
    int read = recv(clntSocket, buf, BUFFER_SIZE, 0);
    if (read < 0) DieWithError("Client read failed\n");
    strcpy(response, "HTTP/1.1 200 OK\r\n\r\n");

    sscanf(buf, "%[^/]/%s %s", other, file, otherTwo);

    fflush(stdout);

    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( file , "rb" );

    if(!fp) {
        perror("TMDG.html");
        exit(1);}

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

/* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer )
    {fclose(fp);
    fputs("memory alloc fails",stderr);
    exit(1);}

/* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp))
    {fclose(fp);
    free(buffer);
    fputs("entire read fails",stderr);
    exit(1);}

    printf(buffer);
    fclose(fp);
    strcat(response, buffer);
    free(buffer);

    fflush(stdout);

    int err = send(clntSocket, response, sizeof(response), 0);
    if (err < 0) DieWithError("Client write failed\n");
}
/* NOT REACHED */

#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <sys/types.h>
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */
#include <sys/time.h>

void DieWithError(char *errorMessage); /* Error handling function */

int main(int argc, char *argv[])
{
    int sock; /* Socket descriptor */
    struct addrinfo hints, *res, *p; /* Server address */
    int status; //Status of getaddrinfo
    unsigned short echoServPort; /* Echo server port */
    char *destURL; /* Server URL (dotted quad) */
    char hostName[50]; /* URL Hostname */
    char path[100]; /* URL path */
    char *portNumber; //Port number of server URL
    char *option; //option to print
    char buf[1024]; //Buffer
    int bytesRcvd, totalBytesRcvd; /* Bytes read in single recv()
and total bytes read */
    char ipstr[INET6_ADDRSTRLEN]; //String to hold IP address
    char message[100];
    int bytes_read;
    struct timeval beforeConnect;
    struct timeval afterConnect;
    double RTT;

    if ((argc < 3) || (argc > 4)) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s [-options] <Server URL> <Port Number>\n",
                argv[0]);
        exit(1);
    }

    if (argc == 3){
        destURL = argv[1]; /* First arg: Destination URL */
        sscanf(destURL, "%[^/]/%s", hostName, path);
        portNumber = argv[2]; /* Second arg: Port Number of Destination URL */
    }
    else
    {
        option = argv[1]; /* First arg: Option to print or not */
        destURL = argv[2]; /* Second arg: Destination URL */
        portNumber = argv[3]; /* Third arg: Port Number of Destination URL */
    }

/* Create a reliable, stream socket using TCP */
    if ((sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

/* Construct the server address structure */
    memset(&hints, 0, sizeof(hints)); /* Zero out structure */
    hints.ai_family = AF_INET; /* Internet address family */
    hints.ai_socktype = SOCK_STREAM; /* Server Socket Type */

    /*Gets address info for server url*/
    if ((status = getaddrinfo(hostName, portNumber, &hints, &res) != 0)){
        DieWithError("getaddrinfo() failed");
    }

/* Establish the connection to the server */

    gettimeofday(&beforeConnect, NULL);
    if (connect (sock, res->ai_addr, res->ai_addrlen) < 0)
        DieWithError("connect() failed");
    gettimeofday(&afterConnect, NULL);
    RTT = (afterConnect.tv_usec - beforeConnect.tv_usec);
    RTT = RTT/1000;
    printf("\nThe RTT is %d", RTT);
    freeaddrinfo(res);

    strcpy(message, "GET /");
    strcat(message, path);
    strcat(message, " ");
    strcat(message,  "HTTP/1.1\r\nHost: ");
    strcat(message, hostName);
    strcat(message, "\r\n\r\n");

/* Send the string to the server */

    if(send (sock, message, strlen(message), 0) < 0)
        DieWithError("send() sent a different number of bytes than expected");


    printf("\n\n");
    fflush(stdout);


    do {
        bytes_read = recv(sock, buf, 1024, 0);
        if (bytes_read == -1) {
            DieWithError("Message Received");
        } else{
            printf("%.*s", bytes_read, buf);
        }
    } while (bytes_read > 0);
    printf("\n"); /* Print a final linefeed */
    printf("%d", RTT);
    fflush(stdout);
    close (sock);
    exit(0);
}

void DieWithError(char *errorMessage)
{
    printf(errorMessage);
    printf("\n");
}

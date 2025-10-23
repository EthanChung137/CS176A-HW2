#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
//Used include statements from the example found at https://www.linuxhowtos.org/data/6/server_udp.c

//Error function, borrowed from example at https://www.linuxhowtos.org/data/6/server_udp.c
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

const char* msgToSum(const char *msg) {
    static char sumStr[130];
    int sum = 0;
    int i = 0;

    //Triming leading and trailing whitespace
    int start = 0;
    while (isspace((unsigned char)msg[start])) start++;

    int end = strlen(msg) - 1;
    while (end >= start && isspace((unsigned char)msg[end])) end--;

    if (start > end) {
        snprintf(sumStr, sizeof(sumStr), "Sorry, cannot compute!\n");
        return sumStr;
    }

    for (int i = start; i <= end; i++) {
        if (!isdigit((unsigned char)msg[i])) {
            snprintf(sumStr, sizeof(sumStr), "Sorry, cannot compute!\n");
            return sumStr;
        }
        sum += msg[i] - '0';
    }

    snprintf(sumStr, sizeof(sumStr), "%d\n", sum);
    return sumStr;
}

int main(int argc, char *argv[]){

    //1. Create the socket (the listening socket) 
    //This section of code was borrowed from the example at https://www.linuxhowtos.org/data/6/server_udp.c
    int sock, length, msgReceive, msgSend;
    
    char* msg;
    socklen_t clientlen;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buf[1024];

    if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0){
        error("Opening socket");
    } 

    length = sizeof(server);
    bzero(&server, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    //2. Bind the socket (Bind server IP address and port number with the socket)
    //This section of code was borrowed from the example at https://www.linuxhowtos.org/data/6/server_udp.c
    if (bind(sock, (struct sockaddr *)&server, length) < 0){
        error("binding");
    }

    //3. Listen on the port (wait for connection from client)
    //This section of code was borrowed from the example at https://www.linuxhowtos.org/data/6/server_udp.c
    clientlen = sizeof(struct sockaddr_in);
    while (1) {
        msgReceive = recvfrom(sock, buf, 128, 0, (struct sockaddr *)&client, &clientlen);
        if (msgReceive < 0){
            error("recvfrom");
        } 
        buf[msgReceive] = '\0';

    //4. Send/Receive


        //Read data from the socket (the client request)
        //write(1, "Received a datagram: ", 21);
        //write(1, buf, msgReceive);
        //Write data to the socket (the server response)
        //Loop here

        char currMsg[128];
        strncpy(currMsg, buf, sizeof(currMsg));
        currMsg[sizeof(currMsg)-1] = '\0';

        while(1) {
            msg = msgToSum(currMsg);
            msgSend = sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&client, clientlen);
            if (msgSend < 0) {
                error("sendto");
            }
            
            //Checking length of the message to see if it is single digit plus newline
            if (strlen(msg) == 2 || strlen(msg) == 0){
                break;
            }
            else if (strncmp(msg, "Sorry, cannot compute!", 22) == 0) {
                break;
            }
            strncpy(currMsg, msg, sizeof(currMsg));
            currMsg[sizeof(currMsg)-1] = '\0';
        }
        memset(buf, 0, sizeof(buf));
        memset(currMsg, 0, sizeof(currMsg));
        msg = NULL;
    }
    return 0;
}
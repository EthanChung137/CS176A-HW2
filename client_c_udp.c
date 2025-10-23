#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

//Error function from https://www.linuxhowtos.org/data/6/client_udp.c

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

//UDP Client skeleton from https://www.linuxhowtos.org/data/6/client_udp.c
int main(int argc, char *argv[])
{
    int sock, n;
    unsigned int length;
    struct sockaddr_in server, client;
    struct hostent *hp;
    char buffer[256];

    if (argc != 3) { printf("Usage: server port\n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0){
        error("socket");
    } 

    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0){
        error("Unknown host");
    } 

    bcopy((char *)hp->h_addr_list[0], (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);

    printf("Enter string: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    n=sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
    if (n < 0){
        error("Sendto");
    } 
        
    while(1){
        bzero(buffer, 256);
        n = recvfrom(sock, buffer, 256, 0, (struct sockaddr *)&server, &length);
        if (n < 0){
            error("recvfrom");
        } 

        buffer[n] = '\0';
        printf("From server: %s", buffer);
        fflush(stdout);
        if (strncmp(buffer, "Sorry, cannot compute!", 22) == 0) {
            break;
        }

        if (strlen(buffer) == 2 && isdigit(buffer[0])) {
            break;
        }
    }
    
    close(sock);
    return 0;
}

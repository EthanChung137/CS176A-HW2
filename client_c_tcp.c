#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

//Skeleton code from https://www.linuxhowtos.org/data/6/client.c
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    const int BUFFERSIZE = 1024;

    char buffer[BUFFERSIZE];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    //Sending initial message
    printf("Enter string: ");
    bzero(buffer,BUFFERSIZE);
    fgets(buffer,BUFFERSIZE-1,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    
    //Response loop
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd, buffer, sizeof(buffer) - 1);
        if (n < 0)
            error("ERROR reading from socket");
        if (n == 0) { 
            break;
        }

        buffer[n] = '\0';

        //Removing any '\r' characters due to its ability to "overwrite" the "From server: " message
        // int i, k = 0;
        // for (i = 0; buffer[i] != '\0'; i++) {
        //     if (buffer[i] != '\r' && buffer[i] != '\n') {
        //         buffer[k++] = buffer[i];
        //     }
        // }
        // buffer[k] = '\0';

        printf("From server: %s\n", buffer);
        fflush(stdout);

        if (strncmp(buffer, "Sorry, cannot compute!", 22) == 0) {
            break;
        }
        if (strlen(buffer) == 2 && isdigit((unsigned char)buffer[0])) {
            break;
        }
    }
    close(sockfd);
    return 0;
}

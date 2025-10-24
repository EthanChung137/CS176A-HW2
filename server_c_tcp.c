    /* A simple server in the internet domain using TCP
    The port number is passed as an argument */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h> 
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <ctype.h>

    //From https://www.linuxhowtos.org/data/6/server.c
    void error(const char *msg) {
        perror(msg);
        exit(1);
    }

    //Copied from server_c_udp.c
    const char* msgToSum(const char *msg) {
    static char sumStr[130];
    int sum = 0;

    //Triming leading and trailing whitespace
    int start = 0;
    while ((isspace((unsigned char)msg[start]))) {
        start++;
    } 

    int end = strlen(msg) - 1;
    while (end >= start && isspace((unsigned char)msg[end])) end--;

    if (start > end) {
        snprintf(sumStr, sizeof(sumStr), "Sorry, cannot compute!\n");
        return sumStr;
    }

    for (int i = start; i <= end; i++) {
        if (!(isdigit((unsigned char)msg[i]))) {
            snprintf(sumStr, sizeof(sumStr), "Sorry, cannot compute!\n");
            return sumStr;
        }
        sum += msg[i] - '0';
    }

    snprintf(sumStr, sizeof(sumStr), "%d", sum);
    return sumStr;
    }

    //Skeleton from https://www.linuxhowtos.org/data/6/server.c
    int main(int argc, char *argv[])
    {
        int sockfd, newsockfd, portno;
        socklen_t clilen;
        char buf[1024];
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        if (argc < 2) {
            fprintf(stderr,"ERROR, no port provided\n");
            exit(1);
        }

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
        error("ERROR opening socket");

        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0) 
                error("ERROR on binding");

        listen(sockfd,5);
        clilen = sizeof(cli_addr);

        while(1) {
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) 
                error("ERROR on accept");

            bzero(buf,1024);
            n = read(newsockfd,buf,1023);
            if (n < 0) {
                error("ERROR reading from socket");
            }
            buf[n] = '\0';

            char currMsg[130];
            strncpy(currMsg, buf, sizeof(currMsg) - 1);
            currMsg[sizeof(currMsg)-1] = '\0';
            
            while(1) {
                const char* msg = msgToSum(currMsg);
                char msgWithNewline[130];
                snprintf(msgWithNewline, sizeof(msgWithNewline), "%s\n", msg);
                n = write(newsockfd, msgWithNewline, strlen(msgWithNewline));
                if (n < 0) {
                    error("ERROR writing to socket");
                }
                if (strncmp(msg, "Sorry, cannot compute!", 22) == 0) {
                    break;
                }
                if (strlen(msg) == 1 && isdigit(msg[0])) {
                    break;
                }
                strncpy(currMsg, msg, sizeof(currMsg)-1);
                currMsg[sizeof(currMsg)-1] = '\0';
            }
            memset(buf, 0, sizeof(buf));
            memset(currMsg, 0, sizeof(currMsg));
            buf[n] = '\0';
            close(newsockfd);
        }
        close(sockfd);
        return 0; 
    }

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SA struct sockaddr

int parseArg(int argc, char **argv);

int main(int argc, char *argv[]){
    int sockfd, connfd, opt = 1, len = sizeof(opt), c;
    struct sockaddr_in servaddr, client;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("Socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created...\n");
    }
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,len);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(*argv[0]);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    }else {
        printf("Socket successfully binded...\n");
    }

    listen(sockfd, 8);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     do{
        connfd = accept(sockfd, (SA *) &client, (socklen_t * ) & c);
        if (connfd < 0) {
            printf("Accept failed...\n");
            exit(0);
        } else {
            printf("Connection accepted...\n");
        }

    }while(1);
    return 0;
}

int parseArg(int argc, char **argv){

}
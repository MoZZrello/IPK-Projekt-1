/*
 * Project: Server
 * Author: Richard Harman
 * Date: 6.3.2022
 * VUT FIT - IPK
 */

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SA struct sockaddr

int parseArg(int argc, char **argv);
void parseResponse(char *buffer, char *response);
void hostnameResponse(char *responseText, char *response);
void cpuLoadResponse(char *responseText, char *response);
void cpuNameResponse(char *responseText, char *response);
void undefinedResponse(char *responseText, char *response);

int main(int argc, char *argv[]){
    struct sockaddr_in servaddr, client;
    int sockfd, connfd, opt = 1, len = sizeof(opt), c, readfd, sockaddrlen = sizeof(servaddr);
    char buffer[128] = {0}, response[1024] = {0};

    parseArg(argc,argv);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("Socket creation failed...\n");
        exit(0);
    }else{
        printf("Socket successfully created...\n");
    }
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,len);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(atoi(argv[1]));

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        perror("Socket bind failed...\n");
        exit(0);
    }else {
        printf("Socket successfully binded...\n");
    }

    listen(sockfd, 8);

     while(1){
        connfd = accept(sockfd, (SA *) &client, (socklen_t * )&sockaddrlen);
        if (connfd < 0) {
            perror("Accept failed...\n");
            exit(0);
        } else {
            printf("Connection accepted...\n");
        }
        readfd = read(connfd, buffer, 128);
        parseResponse(buffer, response);
        send(connfd, response, strlen(response), 0);
        close(connfd);
    }
    return 0;
}

int parseArg(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Wrong number of parameters.\n Expected: 1\n You entered: %d\n", argc-1);
        exit(1);
    }
}

void parseResponse(char *buffer, char *response){
    char responseText[512] = {0};

    if(strstr(buffer, " /hostname ")){
        hostnameResponse(responseText, response);
    }else if(strstr(buffer, " /cpu-name ")){
        cpuNameResponse(responseText, response);
    }else if(strstr(buffer, " /load ")){
        cpuLoadResponse(responseText, response);
    }else{
        undefinedResponse(responseText, response);
    }
}

void hostnameResponse(char *responseText, char *response){
    memset(responseText, 0, 512);
    gethostname(responseText, 512);
    snprintf(responseText + strlen(responseText), 512, "\n");
    snprintf(response, 512,
             "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n%s",
             strlen(responseText), responseText);

}

void cpuLoadResponse(char *responseText, char *response){
    int loadings = 2, count = 0;
    char *token;
    double load = 0.0;
    double a[4] = {0.0,0.0,0.0,0.0}, b[4] = {0.0,0.0,0.0,0.0};
    while(loadings > 0) {
        FILE *stat = popen("cat /proc/stat | head -1 | sed -r 's/cpu\\s{1,}//g'", "r");
        fgets(responseText, 512, stat);
        fclose(stat);
        token = strtok(responseText, " ");
        count = 0;
        if(loadings == 2) {
            while (token != NULL) {
                if(count > 3){
                    break;
                }
                a[count] = atoi(token);
                count++;
                token = strtok(NULL, " ");
            }
        }else{
            while (token != NULL) {
                if(count > 3){
                    break;
                }
                b[count] = atoi(token);
                count++;
                token = strtok(NULL, " ");
            }
        }
        loadings--;
        usleep(1000);
    }
    load = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
    snprintf(responseText, 512, "%.0f%%\n", load * 100);
    snprintf(response, 512,
             "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n%s\r",
             strlen(responseText), responseText);
}

void cpuNameResponse(char *responseText, char *response){
    FILE *cpuInfo = popen("lscpu | grep \"Model name:\" | sed -r 's/Model name:\\s{1,}//g'", "r");
    fgets(responseText, 512, cpuInfo);
    fclose(cpuInfo);
    if (strlen(responseText) == 0) {
        FILE *cpuInfo = popen("lscpu | grep \"Název modelu\" | sed -r 's/Název modelu:\\s{1,}//g'", "r");
        fgets(responseText, 512, cpuInfo);
        fclose(cpuInfo);
    }
    snprintf(response, 512,
             "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n%s\r",
             strlen(responseText), responseText);
}

void undefinedResponse(char *responseText, char *response){
    strncpy(responseText, "404 Not Found\n", 511);
    snprintf(response, 512,
             "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: %ld\n\n%s",
             strlen(responseText), responseText);
}
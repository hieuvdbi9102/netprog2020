#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
    //     printf("connection with the server failed...\n");
    //     exit(0);
    // }
    // else
    //     printf("connected to the server..\n");

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == 0)
    {
        printf("Connection established\n");
        while (1)
        {
            char buffer[512];
            printf("Client> ");
            fflush(stdin);
            scanf("%s", buffer);
            if (strcmp(buffer, "/quit") == 0){
                break;
            }
            int n = send(sockfd, buffer, strlen(buffer) + 1, 0); // +1 NULL characer
            printf("sent %d\n", n, strlen(buffer));
            memset(buffer, 0, 512);
            if (recv(sockfd, buffer, 512, 0) == 0)
            {
                printf("Connection terminated");
                exit(1);
            }
            printf("Server> %s\n", buffer);
        }
    }
    else
    {
        printf("Connection error\n");
        exit(1);
    }
    //chat(sockfd);
    close(sockfd);
}
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd)
{
    char buff[MAX];
    int n;

    for (;;)
    {
        bzero(buff, MAX);
        read(sockfd, buff, strlen(buff));
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;

        while ((buff[n++] = getchar()) != '\n')
            ;

        write(sockfd, buff, sizeof(buff));

        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd, len;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bounce..\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd == -1)
    {
        printf("Oops! Something wrong");
        exit(1);
    }
    else
    {
        printf("Client Connected\n");
        while (1)
        {
            char buffer[512];
            int n = recv(connfd, buffer, 512, 0);
            if (n == 0)
            {
                printf("Connection terminated");
                exit(1);
            }

            int first = 0;
            for (int i = 0; i < n; i++)
            {
                if (buffer[i] == '\0')
                {
                    printf("Client> %s\n", &buffer[first]);
                    first = i + 1;
                }
            }

            printf("Server> ");
            memset(buffer, 0, 512);
            scanf("%s", buffer);
            send(connfd, buffer, strlen(buffer), 0);
        }
    }
    chat(connfd);
    close(sockfd);
}

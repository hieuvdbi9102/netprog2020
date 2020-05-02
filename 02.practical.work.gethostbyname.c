#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
extern int h_errno;

int main(int argc, char *argv[]){
  if (argc == 1){
    printf("Hostname: %s ", argv[0]);
	  exit(1);
  } 
  struct hostent *ip = gethostbyname(argv[1]);
  if (!ip){
   	printf("Not exits!\n");
    exit(1);	
  }
  struct in_addr ip_addr = *(struct in_addr*)(ip->h_addr);
  printf("%s: %s\n",argv[1],inet_ntoa(ip_addr));   
  return 0; 
}

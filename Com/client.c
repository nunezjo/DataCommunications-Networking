#define _DEFAULT_SOURCE //added to include h_addr macro definition in hostent

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>




void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1000];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if(connect(sockfd,(struct sockaddr *) &serv_addr,
    sizeof(serv_addr))<0)
        error("ERROR connecting");
    
    while(1) {

      
        char buffer[1000];
  
        printf("Please enter the message: ");
        bzero(buffer,1000);
        fgets(buffer,999,stdin);
   
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
         error("ERROR writing to socket");

        bzero(buffer,1000);
        n = read(sockfd,buffer,999);

        if (n < 0) error("ERROR reading from socket");
        printf("Server:%s\n",buffer);

        int i = strncmp("Bye",buffer,3);
        if (i == 0)
        break;
    }
    close(sockfd);
     return 0; 
    
}

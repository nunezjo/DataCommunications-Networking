/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
        if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     int sockfd, newsockfd, portno, clilen, n;
     char buffer[1000];
     struct sockaddr_in serv_addr, cli_addr;
    
  
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
    
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     
     while (1) {
     
          bzero(buffer,1000);
          n = read(newsockfd,buffer,999);
          if (n < 0) 
               error("ERROR reading from socket");
          printf("Client: %s\n",buffer);
          
          printf("Please enter the message:  ");
          bzero(buffer,1000);
          fgets(buffer,999,stdin);

          n = write(newsockfd,buffer,strlen(buffer));
          if (n < 0) 
               error("ERROR writing to socket");
          
          int i = strncmp("Bye",buffer,3);
          if (i == 0)
          break;
     
     } 
     close(newsockfd);
     close(sockfd);
     return 0;
}


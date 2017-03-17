#define _BSD_SOURCE
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

using namespace std;
extern int errno;

int port;

int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;	
  char msg[100];		

  if (argc != 3)
    {
      printf ("[client] Syntax: %s <server_address> <port>\n", argv[0]);
      return -1;
    }


  port = atoi (argv[2]);


  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] 'socket()' error.\n");
      return errno;
    }
  


  server.sin_family = AF_INET;

  server.sin_addr.s_addr = inet_addr(argv[1]);

  server.sin_port = htons (port);
  

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
  {
     perror ("[client]'connect()' error.\n");
     return errno;
  }
	
  bzero (msg, 100);
  printf ("[client]Insert a username and a password: \n");
  fflush (stdout);
  
  while(1) 
  {

  	bzero (msg, 100);

  	fflush (stdout);
  	read (0, msg, 100);
  

  	if (write (sd, msg, 100) <= 0)
    	{
      		perror ("[client] 'write()' error to server.\n");
      		return errno;
    	}


  	if (read (sd, msg, 100) < 0)
    	{
      		perror ("[client] 'read()' error from server.\n");
      		return errno;
    	}

  	printf ("%s\n", msg);

  }
  close(sd); 
}

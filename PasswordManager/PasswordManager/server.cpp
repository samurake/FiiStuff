/* concServer.c - Exemplu de server TCP concurent
   Asteapta un nume de la clienti; intoarce clientului sirul
   "Hello nume".
*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "utility.h"
using namespace std;

/* password manager part */
user users[100];
int usersIndex;

char commands[100][100];
int commandsIndex;



/* portul folosit */
#define PORT 11111

/* codul de eroare returnat de anumite apeluri */
extern int errno;

void sighandler(int signo)
{
	if (signo == SIGCHLD) 
	{
		printf("[server-%d]Child terminated\n", getpid()); fflush(stdout);
   		while( waitpid(-1, NULL, WNOHANG) > 0  ) ;
  	}
}

int main ()
{
	struct sockaddr_in server;	// structura folosita de server
	struct sockaddr_in from;	
	char msg[100];		//mesajul primit de la client 
	char msgrasp[100]=" ";        //mesaj de raspuns pentru client
	int sd;			//descriptorul de socket 
	int optval = 1;		//optiune folosita pentru setsockopt()
	int buffsize = 1024;
	char buffer[buffsize];
	char dummy[100];
	/* crearea unui socket */
	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{	
		perror ("[server]Eroare la socket().\n");
		return errno;
	}

	/*setam pentru socket optiunea SO_REUSEADDR */ 
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	/* pregatirea structurilor de date */
	bzero (&server, sizeof (server));
	bzero (&from, sizeof (from));
  
	/* umplem structura folosita de server */
	/* stabilirea familiei de socket-uri */
	server.sin_family = AF_INET;	
  
	/* acceptam orice adresa */
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	
	/* utilizam un port utilizator */
	server.sin_port = htons (PORT);
  
	/* atasam socketul */
	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
	{
		perror ("[server]Eroare la bind().\n");
		return errno;
	}

	/* punem serverul sa asculte daca vin clienti sa se conecteze */
	if (listen (sd, 5) == -1)
	{
		perror ("[server]Eroare la listen().\n");
		return errno;
	}

	printf("[server-%d]Asteptam la portul %d...\n", getpid(), PORT);
	fflush(stdout);

	signal(SIGCHLD, sighandler);
	//signal(SIGCHLD, SIG_IGN);

	int client;
	int length = sizeof (from);
	int pid;                        // id-ul procesului copil
	char address[INET_ADDRSTRLEN];  // adresa IP a clientului    	

	/*password manager part */
	chargeUsersFile("users.txt",users,usersIndex);
	chargeCommandsFile("commands.txt",commands,commandsIndex);
	
	/* servim in mod concurent clientii... */  	
	while (1)
	{
		
		bzero(&from, sizeof(from));

		/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
		client = accept (sd, (struct sockaddr *) &from,(socklen_t*) &length);
	
		/* eroare la acceptarea conexiunii de la un client */
		if (client < 0)
		{
			perror ("[server]Eroare la accept().\n");
			continue;
		}
		else 
		{
			inet_ntop(AF_INET, &from.sin_addr.s_addr, address, INET_ADDRSTRLEN);
			printf("[server-%d]S-a conectat clientul: %s:%d\n", getpid(), address, ntohs(from.sin_port));
			fflush(stdout);
		}
		

		if ( (pid = fork()) < 0 ) 
		{
			perror("fork() error");
			continue;
		}
		else if (pid == 0) 
		{    
			do {
				recv(client,buffer,buffsize,0); 
				strcpy(dummy,buffer);
			}while(strlen(dummy)<2);
			/*while(1) 
			//{
				cout<<"WORKS"<<endl;
								
				// procesul fiu care se va ocupa de client
	
				// s-a realizat conexiunea, se astepta mesajul 
				bzero (msg, 100);
				printf ("[server-%d]Asteptam mesajul...\n", getpid());
				fflush (stdout);
      				
				// citirea mesajului 
				if (read (client, msg, 100) <= 0)
				{
					printf ("[server-%d]Eroare la read() de la client.\n", getpid());
					close (client);	// inchidem conexiunea cu clientul //
					continue;		// continuam sa ascultam //
				}
				
				cout<<msg;
				
				if(loginCheck(msg,users[0])==true)
					cout<<"DA S-A LOGAT CU SUCCES"<<endl;
	
				printf ("[server-%d]Mesajul a fost receptionat...%s\n", getpid(), msg);
      
				//pregatim mesajul de raspuns 
				bzero(msgrasp,100);
				strcat(msgrasp,"Hello ");
				strcat(msgrasp,msg);
      
				printf("[server-%d]Trimitem mesajul inapoi...%s\n", getpid(), msgrasp);
      
				// returnam mesajul clientului //
				if (write (client, msgrasp, 100) <= 0)
				{
					printf ("[server-%d]Eroare la write() catre client.\n", getpid());
					continue;		// continuam sa ascultam 
				}
				else
					printf ("[server-%d]Mesajul a fost transmis cu succes.\n", getpid());
			//}*/	
      		} 
		//close(client);
	}				// while 
}
					// main 

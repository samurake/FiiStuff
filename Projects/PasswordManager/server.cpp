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
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "utility.h"
using namespace std;

#define PORT 9024

extern int errno;

int main ()
{

	struct sockaddr_in server;
	struct sockaddr_in from;	
	char msg[100];		
	char msgrasp[100]=" ";       
	int sd;			
	int optval = 1;	
  
  
	struct user users[100];
	int userIndex = 0;
	int userID = -1;  


  	char commands[100][100];
  	int commandIndex = 0;
  
  
  	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    	{
      		perror ("[server] 'socket()' error.\n");
      		return errno;
    	}

	loadUsersFile("users.txt", users, userIndex);
	loadCommandsFile("commands.txt", commands, commandIndex);
	cout<<"Loaded Users: "<<userIndex<<endl;

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	bzero (&server, sizeof (server));
	bzero (&from, sizeof (from));
  
  	server.sin_family = AF_INET;	
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port = htons (PORT);
  
  	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    	{
      		perror ("[server] 'bind()' error.\n");
      		return errno;
    	}

  	if (listen (sd, 5) == -1)
    	{
      		perror ("[server]'listen()' error.\n");
      		return errno;
    	}

  	printf("[server-%d]Waiting for port %d...\n", getpid(), PORT);
  	fflush(stdout);

  	while (1)
    	{
      		int client;
      		int length = sizeof (from);
      		int pid;                       
      		char address[INET_ADDRSTRLEN]; 

      		bzero(&from, sizeof(from));

      		client = accept (sd, (struct sockaddr *) &from, (socklen_t *) &length);

      		if (client < 0)
        	{
          		perror ("[server] 'accept()' error.\n");
          		continue;
        	}
      		else 
        	{
          		inet_ntop(AF_INET, &from.sin_addr.s_addr, address, INET_ADDRSTRLEN);
          		printf("[server-%d]A client connected with IP: %s:%d\n", getpid(), address, ntohs(from.sin_port));
          		fflush(stdout);
       		}

      		if ( (pid = fork()) < 0 ) {
			perror("fork() error");
			continue;
     		 }
      		else if (pid == 0) {  
			bzero (msg, 100);
        		fflush (stdout);
	while(1) 
	{
         	if (read (client, msg, 100) <= 0)
	   	{
	     		printf ("[server-%d] 'read()' error from client..\n", getpid());
	     		close (client);
	     		break;	
	   	}

        	char aux[100];
		strcpy(aux,msg);
		if(commandUsed(aux))
		{	
        		if(loginCheck(aux, users, userIndex, userID) == true)
			{	
				bzero(msgrasp,100);
				strcat(msgrasp,"Login succeded!\n");
	         		strcat(msgrasp,"Welcome ");
	         		strcat(msgrasp,users[userID].name);
				strcat(msgrasp," ! You are now logged in.");        		
				
	         		if (write (client, msgrasp, 100) <= 0)
		   		{	
		     			printf ("[server-%d] 'write()' error from client..\n", getpid());
		     			continue;	
		  	 	}
				char confBuff[100];
				bzero(confBuff,100);				
				strcat(confBuff,"users/");
				strcat(confBuff,users[userID].name);
				strcat(confBuff,".txt");
				loadUserDataFile(confBuff,users[userID],users[userID].passIndex);
				while(1)
				{
					if (read (client, msg, 100) <= 0)
		   			{
		     				printf ("[server-%d] 'read()' error from client..\n", getpid());
		     				close (client);
		     				break;	
		   			}
					char uBuffer[100];
					strcpy(uBuffer,msg);
					if(usercommandUsed(uBuffer))
					{				
						
						if(strstr(uBuffer,"insert") != NULL)
						{
							char title[100];
							char password[100];
							char category[100];

							bzero(msgrasp,100);
	         					strcat(msgrasp,"Enter a title for the pass you want to insert:\n");         		
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
								printf ("[server-%d] 'write()' error from client..\n", getpid());
						     		continue;	
		  				 	}
							bzero(msg,100);
							if (read (client, msg, 100) <= 0)
		   					{
		     						printf ("[server-%d] 'read()' error from client..\n", getpid());
		     						close (client);
		     						break;	
		   					}
							strcpy(title,msg);
							
							bzero(msgrasp,100);
	         					strcat(msgrasp,"Enter the actual password:\n");         		
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
								printf ("[server-%d] 'write()' error from client..\n", getpid());
						     		continue;	
		  				 	}
							bzero(msg,100);
							if (read (client, msg, 100) <= 0)
		   					{
		     						printf ("[server-%d] 'read()' error from client..\n", getpid());
		     						close (client);
		     						break;	
		   					}
							strcpy(password,msg);
							
							bzero(msgrasp,100);
	         					strcat(msgrasp,"Enter the category of the pass:\n");         		
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
								printf ("[server-%d] 'write()' error from client..\n", getpid());
						     		continue;	
		  				 	}
							bzero(msg,100);
							if (read (client, msg, 100) <= 0)
		   					{
		     						printf ("[server-%d] 'read()' error from client..\n", getpid());
		     						close (client);
		     						break;	
		   					}
							strcpy(category,msg);
					
							addEntry(title,password,category,users[userID]);							
						
							char confBuff[100];
							bzero(confBuff,100);				
							strcat(confBuff,"users/");
							strcat(confBuff,users[userID].name);
							strcat(confBuff,".txt");	
							updateUserFile(confBuff, users[userID], users[userID].passIndex);							
								
							bzero(msgrasp,100);
	         					strcat(msgrasp,"Your entry is saved.\n");         		
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
								printf ("[server-%d] 'write()' error from client..\n", getpid());
						     		continue;	
		  				 	}
								
						}						
						if(strstr(uBuffer,"edit") != NULL)
						{
							int counter;							
							char bindex[100];
							char index[100];
							bzero(msgrasp,100);
	         					strcat(msgrasp,"Please enter the index of the password you want to edit:\n");        		

				         		if (write (client, msgrasp, 100) <= 0)
					   		{
					     			printf ("[server-%d] 'write()' error from client..\n", getpid());
					     			continue;	
		  				 	}
							
							bzero(msg,100);
							if (read (client, msg, 100) <= 0)
		   					{
		     						printf ("[server-%d] 'read()' error from client..\n", getpid());
		     						close (client);
		     						break;	
		   					}
							strcpy(bindex,msg);
							CopyRightWord(bindex,index);
							if(atoi(index) < 0 || atoi(index) >= users[userID].passIndex)
							{	
								bzero(msgrasp,100);
	         						strcat(msgrasp,"Incorrect index.\n");        		

				         			if (write (client, msgrasp, 100) <= 0)
					   			{
					     				printf ("[server-%d] 'write()' error from client..\n", getpid());
					     				continue;	
		  				 		}
							}
							else
							{
								char editbuffer[100];
								counter = atoi(index);
								bzero(msgrasp,100);
	         						strcat(msgrasp,"Edit Options: \n");
								strcat(msgrasp," 'title' 'password' 'category' 'url' 'notes' \n");
								strcat(msgrasp,"Select one: \n");
				         			if (write (client, msgrasp, 100) <= 0)
					   			{
					     				printf ("[server-%d] 'write()' error from client..\n", getpid());
					     				continue;	
		  				 		}

								bzero(msg,100);
								if (read (client, msg, 100) <= 0)
		   						{
		     							printf ("[server-%d] 'read()' error from client..\n", getpid());
		     							close (client);
		     							break;	
		   						}
								strcpy(editbuffer,msg);
									
								if(editcommandParams(editbuffer))
								{
									if(strstr(editbuffer,"title") != NULL)
									{
										char bufftitle[100];										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Enter the new 'title' value:\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}

										bzero(msg,100);
										
										if (read (client, msg, 100) <= 0)
		   								{
		     									printf ("[server-%d] 'read()' error from client..\n", getpid());
		     									close (client);
		     									break;	
		   								}	
										
										char convfx[100];
										strcpy(convfx,msg);
										CopyRightWord(convfx,bufftitle);
										strcpy(users[userID].passwords[counter].title,bufftitle);
										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Title changed.\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}

										
									}
									if(strstr(editbuffer,"password") != NULL)
									{
										char buffpassword[100];										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Enter the new 'title' value:\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}

										bzero(msg,100);
										
										if (read (client, msg, 100) <= 0)
		   								{
		     									printf ("[server-%d] 'read()' error from client..\n", getpid());
		     									close (client);
		     									break;	
		   								}	
										char convfx[100];
										strcpy(convfx,msg);
										CopyRightWord(convfx,buffpassword);
										strcpy(users[userID].passwords[counter].password,buffpassword);
										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Password changed.\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}
								
									}
									if(strstr(editbuffer,"category") != NULL)
									{
										char buffcategory[100];										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Enter the new 'title' value:\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}

										bzero(msg,100);
										
										if (read (client, msg, 100) <= 0)
		   								{
		     									printf ("[server-%d] 'read()' error from client..\n", getpid());
		     									close (client);
		     									break;	
		   								}	
										
										char convfx[100];
										strcpy(convfx,msg);
										CopyRightWord(convfx,buffcategory);
										strcpy(users[userID].passwords[counter].category,buffcategory);
										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Title changed.\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}	
								
									}
									if(strstr(editbuffer,"url") != NULL)
									{
										char buffurl[100];										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Enter the new 'url' value:\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}

										bzero(msg,100);
										
										if (read (client, msg, 100) <= 0)
		   								{
		     									printf ("[server-%d] 'read()' error from client..\n", getpid());
		     									close (client);
		     									break;	
		   								}	
										
										char convfx[100];
										strcpy(convfx,msg);
										CopyRightWord(convfx,buffurl);
										users[userID].passwords[counter].b_url = true;
										strcpy(users[userID].passwords[counter].url,buffurl);
										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Url changed.\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}
								
									}
									if(strstr(editbuffer,"notes") != NULL)
									{
										char buffnotes[100];										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Enter the new 'notes' value:\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}

										bzero(msg,100);
										
										if (read (client, msg, 100) <= 0)
		   								{
		     									printf ("[server-%d] 'read()' error from client..\n", getpid());
		     									close (client);
		     									break;	
		   								}	
										
										char convfx[100];
										strcpy(convfx,msg);
										CopyRightWord(convfx,buffnotes);
										users[userID].passwords[counter].b_notes = true;
										strcpy(users[userID].passwords[counter].notes,buffnotes);
										
										bzero(msgrasp,100);
	         								strcat(msgrasp,"Notes changed.\n");        		
	
				         					if (write (client, msgrasp, 100) <= 0)
					   					{
					     						printf ("[server-%d] 'write()' error from client..\n", getpid());
					     						continue;	
		  						 		}
									}																	
								} 
								else
								{
									bzero(msgrasp,100);
	         							strcat(msgrasp,"Invalid 'edit' parameter.\n");        		
	
				         				if (write (client, msgrasp, 100) <= 0)
					   				{
					     					printf ("[server-%d] 'write()' error from client..\n", getpid());
					     					continue;	
		  						 	}	
								}	
							}
							char confBuff[100];
							bzero(confBuff,100);				
							strcat(confBuff,"users/");
							strcat(confBuff,users[userID].name);
							strcat(confBuff,".txt");	
							updateUserFile(confBuff, users[userID], users[userID].passIndex);
						}
						if(strstr(uBuffer,"help") != NULL)
						{
							bzero(msgrasp,100);
	         					strcat(msgrasp,"\n###Help Advisor###\n");
							strcat(msgrasp,"Commands List: 'insert' 'edit' 'help' 'entries' 'logout'\n");         		
							 
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
					     			printf ("[server-%d] 'write()' error from client..\n", getpid());
					     			continue;	
		  				 	}	
						}
						if(strstr(uBuffer,"entries") != NULL)
						{
							bzero(msgrasp,100);
	         					strcat(msgrasp,"\nEntry List(id starts with 0): \n");
							
							for(int cnt = 0; cnt < users[userID].passIndex; cnt++)
							{		
									strcat(msgrasp,"-");					
									strcat(msgrasp,users[userID].passwords[cnt].title);
									strcat(msgrasp,"\n");
							}         		
							 
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
					     			printf ("[server-%d] 'write()' error from client..\n", getpid());
					     			continue;	
		  				 	}
						}
	
						if(strstr(uBuffer,"show") != NULL)
						{
							int counter;							
							char showbuff[100];
							char bshowbuff[100];							
							bzero(msgrasp,100);
	         					strcat(msgrasp,"\nSelect the id of the password you want to see. \n");         		
							 
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
					     			printf ("[server-%d] 'write()' error from client..\n", getpid());
					     			continue;	
		  				 	}
							
							bzero(msg,100);
										
							if (read (client, msg, 100) <= 0)
		   					{										     									printf ("[server-%d] 'read()' error from client..\n", getpid());
								close (client);
		     						break;	
		   					}
							
							strcpy(bshowbuff,msg);
							CopyRightWord(bshowbuff,showbuff);
							if(atoi(showbuff) < 0 || atoi(showbuff) >= users[userID].passIndex)
							{	
								bzero(msgrasp,100);
	         						strcat(msgrasp,"Incorrect index.\n");        		

				         			if (write (client, msgrasp, 100) <= 0)
					   			{
					     				printf ("[server-%d] 'write()' error from client..\n", getpid());
					     				continue;	
		  				 		}
							}
							else
							{
								bzero(msgrasp,100);
								counter = atoi(showbuff);
								strcat(msgrasp,"Entry: \n");
								strcat(msgrasp,users[userID].passwords[counter].title);
								strcat(msgrasp,"\n");
								strcat(msgrasp,users[userID].passwords[counter].password);
								strcat(msgrasp,"\n");
								strcat(msgrasp,users[userID].passwords[counter].category);
								strcat(msgrasp,"\n");
								if(users[userID].passwords[counter].b_url == true)
								{	strcat(msgrasp,users[userID].passwords[counter].url);
									strcat(msgrasp,"\n");}
								if(users[userID].passwords[counter].b_notes == true)
								{	strcat(msgrasp,users[userID].passwords[counter].notes);
									strcat(msgrasp,"\n");}
																
								if (write (client, msgrasp, 100) <= 0)
					   			{
					     				printf ("[server-%d] 'write()' error from client..\n", getpid());
					     				continue;	
		  				 		}
							}
						}
						if(strstr(uBuffer,"logout") != NULL)
						{
							bzero(msgrasp,100);
	         					strcat(msgrasp,"\nGoodbye ");
							strcat(msgrasp,users[userID].name);
							strcat(msgrasp,"!\n");         		
							 
				         		if (write (client, msgrasp, 100) <= 0)
					   		{
					     			printf ("[server-%d] 'write()' error from client..\n", getpid());
					     			continue;	
		  				 	}
							users[userID].logged_in = false;
							break;	
						}
						
					}
					else 
					{
						bzero(msgrasp,100);
		         			strcat(msgrasp,"Invalid input command. Try 'help' for more info.\n");
	
	         				if (write (client, msgrasp, 100) <= 0)
		   				{
		     					printf ("[server-%d] 'write()' error from client..\n", getpid());
		     					continue;	
		  	 			}
					}
				}
			}
		 	else if(strstr(aux,"login") != NULL)
			{
				bzero(msgrasp,100);
	         		strcat(msgrasp,"\nLogin failed! Your account is invalid.\n");
				strcat(msgrasp,"Try 'help' if you don't have one.\n");         		
		
	         		if (write (client, msgrasp, 100) <= 0)
		   		{
		     			printf ("[server-%d] 'write()' error from client..\n", getpid());
		     			continue;	
		  	 	}
			}
			
			if(strstr(aux,"help") != NULL)
			{
				bzero(msgrasp,100);
				strcat(msgrasp,"\n###Help Advisor###\n");
				strcat(msgrasp,"Commands List: 'login <user> <pass>' 'register' 'help'\n");            		
		
	         		if (write (client, msgrasp, 100) <= 0)
		   		{
		     			printf ("[server-%d] 'write()' error from client..\n", getpid());
		     			continue;	
		  	 	}	
			}

			if(strstr(aux,"register") != NULL)
			{
				char username[100];
				char password[100];		
				bzero(msgrasp,100);
	         		strcat(msgrasp,"\nPlease enter a username for your new account:\n");         		
	         		if (write (client, msgrasp, 100) <= 0)
		   		{
		     			printf ("[server-%d] 'write()' error from client..\n", getpid());
		     			continue;	
		  	 	}
				bzero (msg, 100);
				if (read (client, msg, 100) <= 0)
	   			{
					printf ("[server-%d] 'read()' error from client..\n", getpid());
		     			close (client);
		     			break;	
		   		}

				strcpy(username,msg);

				bzero(msgrasp,100);
	         		strcat(msgrasp,"\nPlease enter a password:\n");         		
	         		if (write (client, msgrasp, 100) <= 0)
		   		{
		     			printf ("[server-%d] 'write()' error from client..\n", getpid());
		     			continue;	
		  	 	}
				bzero (msg, 100);
				if (read (client, msg, 100) <= 0)
	   			{
					printf ("[server-%d] 'read()' error from client..\n", getpid());
		     			close (client);
		     			break;	
		   		}

				strcpy(password,msg);
				addInFile("users.txt",username,password);
				
				char fileBuff[100];
				CopyRightWord(username,fileBuff);
				string str(fileBuff);
  				string path;
  				path = "users/" + str + ".txt"; 
  				ofstream o(path.c_str());
				
				loadUsersFile("users.txt", users, userIndex);				

				bzero(msgrasp,100);
	         		strcat(msgrasp,"\nYour account has been created. Proceed to the login.\n");         		
	         		if (write (client, msgrasp, 100) <= 0)
		   		{
		     			printf ("[server-%d] 'write()' error from client..\n", getpid());
		     			continue;	
		  	 	}
								
			}			
		} else	
		{
			bzero(msgrasp,100);
	         	strcat(msgrasp,"\nInvalid Entry. Try 'help' for more info.");         		
	
        		if (write (client, msgrasp, 100) <= 0)
	   		{
	     			printf ("[server-%d] 'write()' error from client..\n", getpid());
	     			continue;	
	  	 	}
		}
	}
	users[userID].logged_in = false;
	close (client);
	exit(0);
      }		
      close(client);
    }		
}				

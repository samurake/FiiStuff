struct password_entries
{
	char title[100];	
	char password[100];
	char category[100];

	char url[100];
	bool b_url;

	char notes[200];
	bool b_notes;
};

struct user 
{
	char name[100];
	bool logged_in;
	char masterpass[100];
	bool are_entries;
	int passIndex;	
	struct password_entries passwords[5];	
};

void CopyRightWord(char initialWord[1000],char Word[1000])
{
	strcpy(Word,"");
	char c[3]="";

	for(int i=0;i<strlen(initialWord)-1;i++)
	{
		c[0]=initialWord[i];
		strcat(Word,c);
	}
}

void loadUsersFile(const char* infile, struct user (&users)[100], int &index)
{

	FILE *in = fopen(infile,"r");
	char charr[1000];
	char sep[]= "\n";
	char sep2[]= " ";
	char defaulth[]= "default";
	char *p;
	char *k;
	int count = 0;
	index = 0;
	while(fgets(charr,1000,in))
	{
		p=strtok(charr,sep);
		while(p!=NULL)
		{
			count = 0;
			k=strtok(p,sep2);
			while(k!=NULL)
			{
				if(count == 0)
				{
					strcpy(users[index].name,k);
				}
				if(count == 1)
				{
					strcpy(users[index].masterpass,k);
				}
				count++;
				users[index].logged_in = false;
				users[index].are_entries = false;
				users[index].passIndex = 0;
				k=strtok(NULL,sep2);
			}			
			p=strtok(NULL,sep);
			index++;
		}
	}
	fclose(in);
}



void addInFile(const char* outfile, char buff1[100], char buff2[100])
{
	char username[100];
	char password[100];
	CopyRightWord(buff1,username);
	CopyRightWord(buff2,password);
	FILE *out = fopen(outfile,"a");
	char sep[]= "\n";
	char sep2[]= " ";
	fprintf(out,"%s",username);
	fprintf(out,"%s",sep2);
	fprintf(out,"%s",password);
	fprintf(out,"%s",sep);
	fclose(out);
}


void loadCommandsFile(const char* infile, char (&commands)[100][100], int &index)
{
	FILE *in = fopen(infile,"r");
	char charr[1000];
	char sep[]= "\n";
	char *p;
	index = 0;
	while(fgets(charr,1000,in))
	{
		p=strtok(charr,sep);
		while(p!=NULL)
		{
			strcpy(commands[index],p);
			index++;
			p=strtok(NULL,sep);
		}
	}
	fclose(in);
}

bool checkInFile(const char* infile, char buffer[100])
{

	FILE *in = fopen(infile,"r");
	char charr[1000];
	char sep[]= "\n";
	char *p;
	while(fgets(charr,1000,in))
	{
		p=strtok(charr,sep);
		while(p!=NULL)
		{
			if(strcmp(p,buffer)==0)
			{
				fclose(in);
				return true;
			}
			p=strtok(NULL,sep);
		}
	}
	fclose(in);
	return false;
}

bool commandUsed(char buffer[100])
{
	int result = 0;
	if(strstr(buffer,"login") != NULL)
		result++;
	if(strstr(buffer,"register") != NULL)
		result++;
	if(strstr(buffer,"help") != NULL)
		result++;
	if(result==1)
		return true;
	return false;
}


bool editcommandParams(char buffer[100])
{
	int result = 0;
	if(strstr(buffer,"title") != NULL)
		result++;
	if(strstr(buffer,"password") != NULL)
		result++;
	if(strstr(buffer,"category") != NULL)
		result++;
	if(strstr(buffer,"url") != NULL)
		result++;
	if(strstr(buffer,"notes") != NULL)
		result++;
	if(result==1)
		return true;
	return false;
}


bool usercommandUsed(char buffer[100])
{
	int result = 0;
	if(strstr(buffer,"insert") != NULL)
		result++;
	if(strstr(buffer,"edit") != NULL)
		result++;
	if(strstr(buffer,"help") != NULL)
		result++;
	if(strstr(buffer,"entries") != NULL)
		result++;
	if(strstr(buffer,"show") != NULL)
		result++;
	if(strstr(buffer,"logout") != NULL)
		result++;
	if(result==1)
		return true;
	return false;
}

bool loginCheck(char inBuffer[100], struct user users[100], int userIndex, int &userID)
{
	
	char tokBuffer[100];
	CopyRightWord(inBuffer,tokBuffer);
	char *p;
	char login[] = "login";
	char sep[]= " ";
	bool command = false;
	bool user = false;
	bool password = false;
	
	if(strstr(inBuffer,"login") == NULL)
		return false;

	for(userID = 0; userID<userIndex; userID++)
	{
		char buffer[100];
		strcpy(buffer,tokBuffer);
		p=strtok(buffer,sep);
		while(p!=NULL)
		{
			if(strcmp(login,p)==0)
			{	
				command = true;
			}
			if(strcmp(p,users[userID].name)==0)
			{
				user = true;
			}
			if(strcmp(p,users[userID].masterpass)==0)
			{	
				password = true;
			}		
			p=strtok(NULL,sep);						
		}
		if(command == true && user == true && password == true)
		{	
			if(users[userID].logged_in == true)
				return false;
			else users[userID].logged_in = true;	
			return true;
		}
	}	
	return false;
}


void loadUserDataFile(const char* infile, struct user &player, int &index)
{
	FILE *in = fopen(infile,"r");
	char charr[1000];
	char sep[]= "$";
	char sep2[]= "\n";
	char *p;
	char *k;
	int count = -1;
	index = -1;
	while(fgets(charr,1000,in))
	{
		k=strtok(charr,sep2);
		while(k!=NULL)
		{
			if(strstr(k,"#") != NULL)
			{
				count=-1;
				index++;
			}
			
			if(count == 0)
			{
				if(atoi(k)==1)
					player.passwords[index].b_url = true;
				else	player.passwords[index].b_url = false;
			}
			if(count == 1)
			{
				if(atoi(k)==1)
					player.passwords[index].b_notes = true;
				else	player.passwords[index].b_notes = false;
			}
			if(count == 2)
			{
				strcpy(player.passwords[index].title,k);
			}
			if(count == 3)
			{
				strcpy(player.passwords[index].password,k);
			}
			if(count == 4)
			{
				strcpy(player.passwords[index].category,k);
			}
			
			if(count == 5)
			{
				
				if(player.passwords[index].b_url == true)
					strcpy(player.passwords[index].url,k);
				else count++;
			}
			if(count == 6)
			{
				if(player.passwords[index].b_notes == true)
				{	strcpy(player.passwords[index].notes,k);
					
				}
			}	
			count++;
			k=strtok(NULL,sep2);
		}
	}
	player.passIndex = index+1;
	fclose(in);
}

void updateUserFile(const char* outfile, struct user player, int index)
{
	FILE *out = fopen(outfile,"w");
	char sep[]= "#";
	char sep2[]= "\n";
	for(int counter = 0; counter < index; counter++)
	{	
		fprintf(out,"%s",sep);
		fprintf(out,"%s",sep2);
		
		if(player.passwords[counter].b_url == true)
			fprintf(out,"1\n");
		else fprintf(out,"0\n");
		
		if(player.passwords[counter].b_notes == true)
			fprintf(out,"1\n");
		else fprintf(out,"0\n");
		
		fprintf(out,"%s\n",player.passwords[counter].title);
		fprintf(out,"%s\n",player.passwords[counter].password);
		fprintf(out,"%s\n",player.passwords[counter].category);
		
		if(player.passwords[counter].b_url == true)
			fprintf(out,"%s\n",player.passwords[counter].url);
		if(player.passwords[counter].b_notes == true)
			fprintf(out,"%s\n",player.passwords[counter].notes);
		fprintf(out,"\n");	
	}
	fclose(out);
}

void addEntry(char btitle[100], char bpassword[100], char bcategory[100], struct user &player)
{
	char title[100];
	char password[100];
	char category[100];
	CopyRightWord(btitle,title);
	CopyRightWord(bpassword,password);
	CopyRightWord(bcategory,category);	
	int index = 0;
	if(player.passIndex > 0)
	{
		index = player.passIndex;
		player.passIndex = player.passIndex + 1;
	} else player.passIndex = player.passIndex + 1;
	strcpy(player.passwords[index].title,title);
	strcpy(player.passwords[index].password,password);
	strcpy(player.passwords[index].category,category);
	player.passwords[index].b_url = false;
	player.passwords[index].b_notes = false;
}

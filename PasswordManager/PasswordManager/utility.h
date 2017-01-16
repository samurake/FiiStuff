struct user 
{
	char name[100];
	bool logged_in;
	char masterpass[100];	
};


void chargeUsersFile(const char* infile, struct user (&users)[100], int &index)
{

	FILE *in = fopen(infile,"r");
	char charr[1000];
	char sep[]= "\n";
	char sep2[]= " ";
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
				k=strtok(NULL,sep2);
			}			
			p=strtok(NULL,sep);
			index++;
		}
	}
	fclose(in);
}

void chargeCommandsFile(const char* infile, char (&commands)[100][100], int &index)
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

bool loginCheck(char buffer[100], struct user player)
{
	char *p;
	char login[] = "login";
	char sep[]= " ";
	bool command = false;
	bool user = false;
	bool password = false;

	p=strtok(buffer,sep);
	while(p!=NULL)
	{
		if(strcmp(login,p)==0)
			command = true;
		if(strcmp(p,player.name)==0)
			user = true;
		if(strcmp(p,player.masterpass)==0)
			password = true;		
		p=strtok(NULL,sep);						
	}
	if(command == true && user == true && password == true)
	{	
		player.logged_in = true;	
		return true;
	}
	return false;
}


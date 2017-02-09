#include <string.h>
#include <stdio.h>

struct variable {
	int value;
	char name[100];
	int initialized;
};

struct variable variables[100];
int variableCounter = 0;
char buffer[3000] = "";

void varInit(char x[],int value) {
	strcpy(variables[variableCounter].name, x);
	variables[variableCounter].initialized = 1;
	variables[variableCounter].value = value;
	variableCounter++;
}
void varNeInit(char x[]) {
	strcpy(variables[variableCounter].name,x);
	variables[variableCounter].initialized=0;
	variableCounter++;
}
int declared(char x[]) {
    int i;
    for(i = 0; i <= variableCounter; i++)
        if(strcmp(x, variables[i].name) == 0) return i;
    return -1;
}
void parse(char x[], int value) {
    int position = declared(x);
    variables[position].value = value;
    variables[position].initialized = 1;
}

int initialized(char x[]) {
    int i;
    for(i = 0; i <= variableCounter; i++)
        if(strcmp(x, variables[i].name) == 0)
        	if(variables[i].initialized == 0) return 0;
        	else return 1;
    return 0;
}

void write_buffer_int(int number) {
	char buffer1[30];
	sprintf(buffer1, "%d", number);
	strcat(buffer, buffer1);
	strcat(buffer, "\n");
}

void write_buffer(char show[]) {
	strcat(buffer, show);
}

void printFinal(char x[])
{
    int position = declared(x);
    write_buffer("\nid ");
    write_buffer(variables[position].name);
    write_buffer(" = ");
    write_buffer_int(variables[position].value);
    write_buffer("\n");
}

int maxim(int a,int b) {
	if(a>b) return a;
	else return b;
}

int minim(int a,int b) {
	if(a<b) return a;
	else return b;
}

int modul(int a) {
	if(a<0) return (-a);
	else return a;
}

int cmmdc(int a, int b)
{
	while(a != b)
		if(a > b) a -= b;
		else b -= a;
	return b;
}

int cmmmc(int a, int b) {
	int rez;
	rez = a * b / cmmdc(a, b);
	return rez;
}

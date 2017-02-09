#include <string.h>
#include <stdio.h>

struct variable {
	int value;
	char name[100];
	int initialized;
};

struct string {
	char contains[100];
	char name[100];
	int initialized;
};

struct variable variables[100];
struct string strings[100];
void remove_all_chars(char* str, char c);

int variableCounter = 0;
int stringCounter = 0;
char buffer[3000] = "";


void varInit(char x[],int value) {
	strcpy(variables[variableCounter].name, x);
	variables[variableCounter].initialized = 1;
	variables[variableCounter].value = value;
	variableCounter++;
}

void stringInit(char x[],char value[]) {
	strcpy(strings[stringCounter].name, x);
	strings[stringCounter].initialized = 1;
	strcpy(strings[stringCounter].contains,value);
	stringCounter++;
}

void varNeInit(char x[]) {
	strcpy(variables[variableCounter].name,x);
	variables[variableCounter].initialized=0;
	variableCounter++;
}

void stringNeInit(char x[]) {
	strcpy(strings[stringCounter].name,x);
	strings[stringCounter].initialized=0;
	stringCounter++;
}

int declared(char x[]) {
    int i;
    for(i = 0; i <= variableCounter; i++)
        if(strcmp(x, variables[i].name) == 0) return i;
    return -1;
}

int varORstring(char x[]) {
    int i;
    for(i = 0; i <= variableCounter; i++)
        if(strcmp(x, variables[i].name) == 0) return 0;
    for(i = 0; i <= stringCounter; i++)
        if(strcmp(x, strings[i].name) == 0) return 1;
return -1;
}
int declaredString(char x[]) {
    int i;
    for(i = 0; i <= stringCounter; i++)
        if(strcmp(x, strings[i].name) == 0) return i;
    return -1;
}

void parse(char x[], int value) {
    int position = declared(x);
    variables[position].value = value;
    variables[position].initialized = 1;
}

void parseString(char x[], char value[]) {
    int position = declaredString(x);
    strcpy(strings[position].contains,value);
    strings[position].initialized = 1;
}

void parsevar(char x[], char y[]) {
    int positionx = declared(x);
    int positiony = declared(y);
    variables[positionx].value = variables[positiony].value;
    variables[positionx].initialized = 1;
}

void parseSstring(char x[], char y[]) {
    int positionx = declaredString(x);
    int positiony = declaredString(y);
    strcpy(strings[positionx].contains,strings[positiony].contains);
    strings[positionx].initialized = 1;
}

int initialized(char x[]) {
    int i;
    for(i = 0; i <= variableCounter; i++)
        if(strcmp(x, variables[i].name) == 0)
        	if(variables[i].initialized == 0) return 0;
        	else return 1;
    return 0;
}

int initializedString(char x[]) {
    int i;
    for(i = 0; i <= stringCounter; i++)
        if(strcmp(x, strings[i].name) == 0)
        	if(strings[i].initialized == 0) return 0;
        	else return 1;
    return 0;
}

void write_buffer_int(int number) {
	char buffer1[30];
	sprintf(buffer1, "%d", number);
	strcat(buffer, buffer1);
	strcat(buffer, "\n");
}


void write_buffer_string(char value[]) {
	char buffer1[30];
	int a = 39;
	remove_all_chars(value,a);
	sprintf(buffer1, "%s", value);
	strcat(buffer, buffer1);
	strcat(buffer, "\n");
}


void write_buffer(char show[]) {
	strcat(buffer, show);
}

void printFinal(char x[])
{
    int position = declared(x);
    write_buffer("\nvar ");
    write_buffer(variables[position].name);
    write_buffer(" = ");
    write_buffer_int(variables[position].value);
    write_buffer("\n");
}

void printString(char x[])
{
    int position = declaredString(x);
    write_buffer("\nstring ");
    write_buffer(strings[position].name);
    write_buffer(" = ");
    write_buffer_string(strings[position].contains);
    write_buffer("\n");
}

void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
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

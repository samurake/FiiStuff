// ReadFromFile&QuickSort.cpp : Defines the entry point for the console application.
// Just a simple application of fopen, fgets, fclose and atoi in a string of numbers.
// Purpose: Charge an array of numbers from a file, sort the numbers, show the numbers on console.

#include "stdafx.h"


void quicksort(int a[100], int l, int r)
{
	int i, j, p, aux;
	i = l;
	j = r;
	p = a[(l + r) / 2];
	while (i < j)
	{
		while (a[i] < p)
			i++;
		while (a[j] > p)
		{
			if (i <= j)
			{
				aux = a[i];
				a[i] = a[j];
				a[j] = aux;
				i++;
				j--;
			}
		}
	}
	if (i < r)
		quicksort(a, i, r);
	if (j > l)
		quicksort(a, l, j);
}

void ReadFromFile(const char* file, int(&Numbers)[1000], int &N)
{
	int index = -1;
	int aux;
	char *p;
	char sep[] = " ";
	char charr[1000];
	FILE *in = fopen(file, "w");
	while (fgets(charr, 1000, in))
	{
		p = strtok(charr, sep);
		while (p != NULL)
		{
			index++;
			aux = atoi(p);
			Numbers[index] = aux;
			p = strtok(NULL, sep);
		}
	}
	index++;
	N = index;
	fclose(in);
}

void ShowNumbers(int a[1000], int N)
{
	for (int index = 0; index < N; index++)
		cout << a[index] << endl;
}


int main()
{
	int A[1000], N;
	ReadFromFile("numbers.txt", A, N);
	quicksort(A, 0, N - 1);
	ShowNumbers(A,N);
  return 0;
}

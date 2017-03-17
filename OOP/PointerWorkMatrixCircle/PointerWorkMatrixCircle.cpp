// PointerWorkMatrixCircle.cpp : Defines the entry point for the console application.
// Stage: Final. 
// Documentation: https://www.tutorialspoint.com/cprogramming/c_pointer_arithmetic.htm
// What is solved? With a given point(circle origin) and a ray(circle ray) create a circle around that point in matrix.

#include "stdafx.h"

#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 5

int Matrix[MATRIX_HEIGHT][MATRIX_WIDTH]; // Global Definition. All elements are 0 by default.

void Circle(int* ptr, int cx, int cy, int ray)
{
	for (int i = 0; i < MATRIX_HEIGHT; i++)
	{
		for (int j = 0; j < MATRIX_WIDTH; j++)
		{
			double dist = sqrt((j - cx)*(j - cx) + (i - cy)*(i - cy));
			
			//Uncomment for Debug;
			//cout << ray << "==" << dist << " ";
			
			if (ray == ceil(dist))//"ceil(value) used for rounding value up.
			{
				*ptr = 1;//Used * in front of pointed value to access the the value stored in memory and modify it.
			}
			//Debug purpose;
			//cout << ptr << " ";
			ptr++;//Moving to the next pointed value in the Matrix.
		}
		cout << endl;
	}
	cout << endl;
}

void ShowMatrix()
{
	for (int i = 0; i < MATRIX_HEIGHT; i++)
	{
		for (int j = 0; j < MATRIX_WIDTH; j++)
			cout << Matrix[i][j] << " ";
		cout << endl;
	}
}

void main() 
{
	Circle(&Matrix[0][0], 2, 2, 2);//"&" to point the address of the first Matrix element.
	ShowMatrix();
}


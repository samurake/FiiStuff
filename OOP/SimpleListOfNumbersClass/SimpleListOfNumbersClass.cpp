// SimpleListOfNumbersClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	//definition;
	IntList A, B, C, D;
	
	//adding;
	A.Add(5);
	A.Add(6);
	A.Add(7);
	
	//test,working;
	A.Show();
	
	//insert,working;
	A.Insert(1, 12);
	A.Show();
	
	//delete,working;
	A.Delete(2);
	A.Show();
	
	//indexof,working;
	cout << A.IndexOf(12, 0) << endl;
	
	//more adding;
	A.Add(6);
	A.Add(5);
	A.Add(6);
	A.Add(7);
	
	//add test,working;
	A.Show();
	
	//removeduplicates,working;
	A.RemoveDuplicates();
	A.Show();
	
	//getcount, working;
	cout << A.GetCount() << endl;
	
	//more adding;
	B.Add(12);
	B.Add(14);
	B.Add(17);
	B.Add(14);
	
	//intersection,working;
	A.Intersection(&B, &C);
	C.Show();
	cout << C.GetCount() << endl;

	//reunion, working;
	A.Reunion(&B, &D);
	D.Show();
	cout << D.GetCount() << endl;

	
	
	//sort, working;
	D.Sort();
	D.Show();
	cout << endl;
	
	//reverse,working;
	cout << A.GetCount();
	A.Reverse();
	A.Show();


    return 0;
}


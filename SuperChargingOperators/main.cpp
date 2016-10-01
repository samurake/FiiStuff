// SuperChargingOperators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SuperChargingOperators.h"

int main()
{
	BigNumber A(202214);
	BigNumber B("32145");
	BigNumber C(672214);
	BigNumber D("786545");

	BigNumber E, F;

	A.Set(2);
	C.Set("4");

	E = C+ B;
	cout << C << "+" << B << "=" << E << endl;

	F = B - C;
	cout << B << "-" << C << "=" << F << endl;

	if (A < B)
		cout << "Da" << endl;
	
	
	cout << D[3] << endl;
	cout << D(0, 2) << endl;

	
}


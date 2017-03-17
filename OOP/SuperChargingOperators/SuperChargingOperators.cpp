#include "stdafx.h"
#include "SuperChargingOperators.h"

BigNumber::BigNumber()
{
	CharactersCount = 0;
}

BigNumber::BigNumber(int value)
{
	_itoa(value, Number, 10);
	CharactersCount = strlen(Number);
}

BigNumber::BigNumber(const char * number)
{
	strcpy(Number, number);
	CharactersCount = strlen(Number);
}

bool BigNumber::Set(int value)
{
	_itoa(value, Number, 10);
	CharactersCount = strlen(Number);
	return true;
}

bool BigNumber::Set(const char * number)
{
	strcpy(Number, number);
	CharactersCount = strlen(Number);
	return true;
}

BigNumber BigNumber::operator+ (const BigNumber & number)
{
	BigNumber result;
	int buffer, number_buffer, result_buffer;
	buffer = atoi(Number);
	number_buffer = atoi(number.Number);
	result_buffer = buffer + number_buffer;
	_itoa(result_buffer, result.Number, 10);
	result.CharactersCount = strlen(result.Number);
	return result;
}

BigNumber BigNumber::operator* (const BigNumber & number)
{
	BigNumber result;
	int buffer, number_buffer, result_buffer;
	buffer = atoi(Number);
	number_buffer = atoi(number.Number);
	result_buffer = buffer * number_buffer;
	_itoa(result_buffer, result.Number, 10);
	result.CharactersCount = strlen(result.Number);
	return result;
}

BigNumber BigNumber::operator/ (const BigNumber & number)
{
	BigNumber result;
	int buffer, number_buffer, result_buffer;
	buffer = atoi(Number);
	number_buffer = atoi(number.Number);
	result_buffer = buffer / number_buffer;
	_itoa(result_buffer, result.Number, 10);
	result.CharactersCount = strlen(result.Number);
	return result;
}

BigNumber BigNumber::operator- (const BigNumber & number)
{
	BigNumber result;
	int buffer, number_buffer, result_buffer;
	buffer = atoi(Number);
	number_buffer = atoi(number.Number);
	result_buffer = buffer - number_buffer;
	_itoa(result_buffer, result.Number, 10);
	result.CharactersCount = strlen(result.Number);
	return result;
}

bool operator == (const BigNumber & n1, const BigNumber & n2)
{
	if (n1.CharactersCount != n2.CharactersCount)
		return false;
	for (int index = 0; index < n1.CharactersCount; index++)
	{
		if (n1.Number[index] != n2.Number[index])
			return false;
	}
	return true;
}

bool operator != (const BigNumber & n1, const BigNumber & n2)
{
	if (n1.CharactersCount != n2.CharactersCount)
		return true;
	for (int index = 0; index < n1.CharactersCount; index++)
	{
		if (n1.Number[index] != n2.Number[index])
			return true;
	}
	return false;
}

bool operator <  (const BigNumber & n1, const BigNumber & n2)
{
	int n1_buffer, n2_buffer;
	if (n1.CharactersCount < n2.CharactersCount)
		return true;
	if (n1.CharactersCount > n2.CharactersCount)
		return false;
	n1_buffer = atoi(n1.Number);
	n2_buffer = atoi(n2.Number);
	if (n1_buffer < n2_buffer)
		return true;
	else return false;
}

bool operator >  (const BigNumber & n1, const BigNumber & n2)
{
	int n1_buffer, n2_buffer;
	if (n1.CharactersCount > n2.CharactersCount)
		return true;
	if (n1.CharactersCount < n2.CharactersCount)
		return false;
	n1_buffer = atoi(n1.Number);
	n2_buffer = atoi(n2.Number);
	if (n1_buffer > n2_buffer)
		return true;
	else return false;
}

bool operator >= (const BigNumber & n1, const BigNumber & n2)
{
	int n1_buffer, n2_buffer;
	if (n1.CharactersCount > n2.CharactersCount)
		return true;
	if (n1.CharactersCount < n2.CharactersCount)
		return false;
	n1_buffer = atoi(n1.Number);
	n2_buffer = atoi(n2.Number);
	if (n1_buffer >= n2_buffer)
		return true;
	else return false;
}

bool operator <= (const BigNumber & n1, const BigNumber & n2)
{
	int n1_buffer, n2_buffer;
	if (n1.CharactersCount < n2.CharactersCount)
		return true;
	if (n1.CharactersCount > n2.CharactersCount)
		return false;
	n1_buffer = atoi(n1.Number);
	n2_buffer = atoi(n2.Number);
	if (n1_buffer <= n2_buffer)
		return true;
	else return false;
}

BigNumber::operator const int()
{
	int number;
	number = atoi(Number);
	return number;
}

char BigNumber::operator[] (int index)
{
	if (index < 0 || index >= CharactersCount)
		return '\0';
	return Number[index];
}

BigNumber BigNumber::operator() (int start, int end)
{
	BigNumber result;
	int counter = 0;
	if (start < 0 || end >= CharactersCount)
		return 0;
	for (int index = start; index <= end; index++)
	{
		result.Number[counter] = Number[index];
		result.CharactersCount++;
		counter++;
	}
	return result;
}
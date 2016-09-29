#include "stdafx.h"

IntList::IntList()
{
	Count = 0;
}

bool IntList::Add(int element)
{
	if (Count >= 1000)
		return false;
	Elemente[Count] = element;
	Count++;
	return true;
}

bool IntList::Insert(int index, int element)
{
	if (Count == 1000)
		return false;

	for (int i = Count - 1; i >= index; i--)
	{
		Elemente[i + 1] = Elemente[i];
	}
	Elemente[index] = element;
	Count++;
	return true;
}

bool IntList::Delete(int index)
{
	if (Count == 0)
		return false;
	Elemente[index] = Elemente[index + 1];
	for (int i = index + 1; i < Count; i++)
		Elemente[i] = Elemente[i + 1];
	Count--;
	return true;
}

void IntList::Clear()
{
	Count = 0;
}

int IntList::IndexOf(int valoare, int startPosition)
{
	for (int i = startPosition; i < Count; i++)
		if (Elemente[i] == valoare)
			return i;
	return -1;
}

void IntList::RemoveDuplicates()
{
	for (int i = 0; i < Count; i++)
		for (int j = 0; j < Count; j++)
			if (i != j)
				if (Elemente[i] == Elemente[j])
				{
					Elemente[i] = Elemente[i + 1];
					for (int k = i + 1; k < Count; k++)
						Elemente[k] = Elemente[k + 1];
					Count--;
				}
	
}

void IntList::Intersection(IntList* withList, IntList * resultList)
{
	for (int index = 0; index < Count; index++)
		for (int counter = 0; counter < Count; counter++)
			if (Elemente[index] == withList->Elemente[counter])
				resultList->Add(Elemente[index]);
}

void IntList::Reunion(IntList* withList, IntList * resultList)
{
	for (int index = 0; index < Count; index++)
	{
		resultList->Elemente[index] = Elemente[index];
	}
	resultList->Count = Count;
	for (int index = 0; index < Count; index++)
	{
		resultList->Add(withList->Elemente[index]);
	}
	resultList->RemoveDuplicates();
}

void IntList::Sort()
{
	//Bubble Sort
	bool sorted = false;
	do
	{
		sorted = true;
		for (int counter = 0; counter < Count; counter++)
		{
			if (Elemente[counter] > Elemente[counter + 1])
			{
				sorted = false;
				swap(Elemente[counter], Elemente[counter + 1]);
			}
		}
	} while (sorted == false);

}

void IntList::Reverse()
{
	int counter = 0, index = Count-1;
	int aux;
	while (counter < Count && index >= 0 && counter)
	{
		if (counter != index)
		{
			swap(Elemente[counter], Elemente[index]);
			counter++;
			index--;
		}
	}
}

int IntList::Get(int index)
{
	if (index >= 1000 || index < 0)
		return -1;
	return Elemente[index];
}

int IntList::GetCount()
{
	return Count;
}

void IntList::Show()
{
	cout << endl;
	for (int index = 0; index < Count; index++)
	{
		cout << Elemente[index] << " ";
	}
	cout<<endl;
}

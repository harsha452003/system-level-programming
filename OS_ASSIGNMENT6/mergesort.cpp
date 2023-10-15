#include <iostream>
#include <chrono>

#include "goodmalloc.hpp"

using namespace std;

// C++ program for Merge Sort

#include <iostream>
using namespace std;

void merge(char *array, int const left, int const mid, int const right)
{
	scope_start();
	auto const sA1 = mid - left + 1;
	auto const sA2 = right - mid;

	// Create temp arrays
	char *lA = (char *)malloc(100);
	strcpy(lA, "left");
	char *rA = (char *)malloc(100);
	strcpy(rA, "right");

	createList(sA1, lA);
	createList(sA2, rA);

	for (auto i = 0; i < sA1; i++)
		assignVal(lA, i, getVal(array, left + i));

	for (auto j = 0; j < sA2; j++)
		assignVal(rA, j, getVal(array, mid + 1 + j));

	auto indexOfsA1 = 0,indexOfsA2 = 0;
	int indexOfMA = left;

	
	while (indexOfsA1 < sA1 && indexOfsA2 < sA2)
	{
		if (getVal(lA, indexOfsA1) <= getVal(rA, indexOfsA2))
		{
			assignVal(array, indexOfMA, getVal(lA, indexOfsA1));
			indexOfsA1++;
		}
		else
		{
			assignVal(array, indexOfMA, getVal(rA, indexOfsA2));
			indexOfsA2++;
		}
		indexOfMA++;
	}
	while (indexOfsA1 < sA1)
	{
		assignVal(array, indexOfMA, getVal(lA, indexOfsA1));
		indexOfsA1++;
		indexOfMA++;
	}
	while (indexOfsA2 < sA2)
	{
		assignVal(array, indexOfMA, getVal(rA, indexOfsA2));
		indexOfsA2++;
		indexOfMA++;
	}
	freeElem();
}

void mergeSort(char *array, int const begin, int const end)
{
	if (begin >= end)
		return;

	auto mid = begin + (end - begin) / 2;
	mergeSort(array, begin, mid);
	mergeSort(array, mid + 1, end);
	merge(array, begin, mid, end);
}
void printArray(char *array, int size)
{
	for (auto i = 0; i < size; i++)
		cout << getVal(array, i) << "   ";
}

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	size_t s2 = 250 * 1024 * 1024;
	createMem(s2);

	scope_start();
	int size = 50000;
	char *list_name = (char *)malloc(10 * sizeof(char));
	strcpy(list_name, "chandu");
	createList(size, list_name);
	
	srand(time(NULL));
	for (int i = 0; i < size; i++)
	{
		printf("i = %d\n", i);
		assignVal(list_name, i, rand() % 100000 + 1);
	}

	// printf("%s\n",list_name);
	// list*temp=list_name;

	cout << "Linked List after sorting\n";

	mergeSort(list_name, 0, size - 1);
	printArray(list_name, size);
	freeElem(list_name);
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	cout << "Time measured: " << elapsed.count() << " micro seconds." << std::endl;

	return 0;
}

#ifndef ALGORITHEMS_H
#define ALGORITHEMS_H

#include "AbstactAlgorithm.h"

class HeapSort : public Algorithm
{
public:
	HeapSort();
	HeapSort(std::string inputFile);
	HeapSort(std::string inputFile, std::string outputFile);
	uint64_t *  SortFile(std::string fileName, DWORD sizeToRead) override;
private:
	// Changing positions of elements in this array.
	inline void SwapItems(uint64_t *arr, uint64_t left, uint64_t right);

	// To heapify a subtree rooted with node i which is 
	// an index in arr[]. n is size of heap 
	void heapify(uint64_t arr[], uint64_t n, uint64_t i);

	// main function to do heap sort 
	void heapSort(uint64_t arr[], uint64_t n);
};

class QuiekSort : public Algorithm
{
public:
	QuiekSort();
	QuiekSort(std::string inputFile);
	QuiekSort(std::string inputFile, std::string outputFile);
	uint64_t * SortFile(std::string fileName, DWORD sizeToRead) override;
private:
	// Тип сортируемых элементов
	typedef uint64_t item;
	// Тип ключей, по которым сортируем
	typedef LONG tblIndex;

	/**************************
	*  split array a[lb..ub]  *
	**************************/
	tblIndex partition(uint64_t *a, tblIndex lb, tblIndex ub);

	/**************************
	*  sort array  a[lb..ub]  *
	**************************/
	void quickSort(uint64_t *a, tblIndex lb, tblIndex ub);
};

#endif
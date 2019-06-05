#ifndef HEAP_SORT_H
#define HEAP_SORT_H

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
#endif
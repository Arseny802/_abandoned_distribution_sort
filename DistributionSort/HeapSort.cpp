#include "Algorithms.h"

HeapSort::HeapSort()
{
	inputFile = DEFAULT_INPUT_FILE;
	outputFile = DEFAULT_OUTPUT_FILE;
	statistics.methodName = L"heap sort";
}
HeapSort::HeapSort(std::string inputFile)
{
	this->inputFile = inputFile;
	std::wstring wstr(inputFile.begin(), inputFile.end());
	statistics.inputFile = wstr;
	outputFile = DEFAULT_OUTPUT_FILE;
	statistics.methodName = L"heap sort";
}
HeapSort::HeapSort(std::string inputFile, std::string outputFile)
{
	this->inputFile = inputFile;
	this->outputFile = outputFile;
	std::wstring wstr1(inputFile.begin(), inputFile.end());
	statistics.inputFile = wstr1;
	std::wstring wstr2(outputFile.begin(), outputFile.end());
	statistics.outputFile = wstr2;
	statistics.methodName = L"heap sort";
}

uint64_t * HeapSort::SortFile(std::string fileName, DWORD sizeToRead)
{
	uint64_t * result = (uint64_t *)malloc(sizeToRead);
	timePoint_1 = clock();
	_ReadFile(fileName, result, sizeToRead, 0, 0);
	timePoint_2 = clock();
	statistics.totalReadTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
	timePoint_1 = clock();
	heapSort(result, sizeToRead / ONE_BYTE);
	timePoint_2 = clock();
	statistics.totalSortTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
	return result;
}

// Changing positions of elements in this array.
inline void HeapSort::SwapItems(uint64_t *arr, uint64_t left, uint64_t right)
{
	uint64_t tmp = arr[left];
	arr[left] = arr[right];
	arr[right] = tmp;
}

// To heapify a subtree rooted with node i which is 
// an index in arr[]. n is size of heap 
void HeapSort::heapify(uint64_t arr[], uint64_t n, uint64_t i)
{
	uint64_t largest = i; // Initialize largest as root 
	uint64_t l = 2 * i + 1; // left = 2*i + 1 
	uint64_t r = 2 * i + 2; // right = 2*i + 2 

					   // If left child is larger than root 
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// If right child is larger than largest so far 
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// If largest is not root 
	if (largest != i)
	{
		SwapItems(arr, i, largest);

		// Recursively heapify the affected sub-tree 
		heapify(arr, n, largest);
	}
}

// main function to do heap sort 
void HeapSort::heapSort(uint64_t arr[], uint64_t n)
{
	// Build heap (rearrange array) 
	for (long i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap 
	for (long i = n - 1; i >= 0; i--)
	{
		// Move current root to end 
		SwapItems(arr, 0, i);

		// call max heapify on the reduced heap 
		heapify(arr, i, 0);
	}
}
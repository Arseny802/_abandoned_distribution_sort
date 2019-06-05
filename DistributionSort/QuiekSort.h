#ifndef QUIK_SORT_H
#define QUIK_SORT_H

#include "AbstactAlgorithm.h"

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
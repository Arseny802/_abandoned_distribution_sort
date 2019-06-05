#include "Algorithms.h"

QuiekSort::QuiekSort()
{
	inputFile = DEFAULT_INPUT_FILE;
	outputFile = DEFAULT_OUTPUT_FILE;
	statistics.methodName = L"quiek sort";
}

QuiekSort::QuiekSort(std::string inputFile)
{
	this->inputFile = inputFile;
	std::wstring wstr(inputFile.begin(), inputFile.end());
	statistics.inputFile = wstr;
	outputFile = DEFAULT_OUTPUT_FILE;
	statistics.methodName = L"quiek sort";
}

QuiekSort::QuiekSort(std::string inputFile, std::string outputFile)
{
	this->inputFile = inputFile;
	this->outputFile = outputFile;
	std::wstring wstr1(inputFile.begin(), inputFile.end());
	statistics.inputFile = wstr1;
	std::wstring wstr2(outputFile.begin(), outputFile.end());
	statistics.outputFile = wstr2;
	statistics.methodName = L"quiek sort";
}

uint64_t * QuiekSort::SortFile(std::string fileName, DWORD sizeToRead)
{
	uint64_t * result = (uint64_t *)malloc(sizeToRead);
	timePoint_1 = clock();
	_ReadFile(fileName, result, sizeToRead, 0, 0);
	timePoint_2 = clock();
	statistics.totalReadTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
	timePoint_1 = clock();
	quickSort(result, 0, sizeToRead / ONE_BYTE);
	timePoint_2 = clock();
	statistics.totalSortTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
	return result;
}

QuiekSort::tblIndex QuiekSort::partition(uint64_t *a, tblIndex lb, tblIndex ub)
{
	item t, pivot;
	tblIndex i, j, p;

	/* ¬ыбираем центр - pivot */
	p = lb + ((ub - lb) >> 1);
	pivot = a[p];
	a[p] = a[lb];

	/* сортируем lb+1..ub относительно центра */
	i = lb + 1;
	j = ub;
	while (1)
	{
		while (i < j && (pivot > a[i])) i++;
		while (j >= i && (a[j] > pivot)) j--;
		if (i >= j) break;
		t = a[i];
		a[i] = a[j];
		a[j] = t;
		j--; i++;
	}

	/* центр в a[j] */
	a[lb] = a[j];
	a[j] = pivot;

	return j;
}

void QuiekSort::quickSort(uint64_t *a, tblIndex lb, tblIndex ub)
{
	tblIndex m;

	while (lb < ub)
	{
		/* разделение пополам */
		m = partition(a, lb, ub);

		/* ”меньшаем требовани€ к пам€ти:    */
		/*  меньший сегмент сортируем первым */
		if (m - lb <= ub - m)
		{
			quickSort(a, lb, m - 1);
			lb = m + 1;
		}
		else
		{
			quickSort(a, m + 1, ub);
			ub = m - 1;
		}
	}
}
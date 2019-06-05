#ifndef ABSTRACT_ALGORITHM_H
#define ABSTRACT_ALGORITHM_H

#include "Settings.h"
#include "Functions.cpp"
#include "WinFiles.h"

#include <iostream>
#include <math.h>
#include <ctime>

typedef struct Statistics
{
	std::wstring methodName = L"abstract method";
	std::wstring inputFile = TEXT("not set");
	std::wstring outputFile = TEXT("not set");
	double fullTimeElapsed = 0;
	double totalReadTime = 0;
	double totalWriteTime = 0;
	double totalConvertTime = 0;
	double totalDistributeTime = 0;
	double totalSortTime = 0;
	double totalVerifyTime = 0;
	double totalValidateTime = 0;

	void PrintStatistics()
	{
		_tprintf(TEXT("---------------------\n"));
		_tprintf(TEXT("Full sort statistics:\n"));
		_tprintf(TEXT("---------------------\n"));
		_tprintf(TEXT("\tSort statistics method:    %s\n"), methodName.c_str());
		_tprintf(TEXT("\tInput file name:           %s\n"), inputFile.c_str());
		_tprintf(TEXT("\tOutput file name:          %s\n"), outputFile.c_str());
		_tprintf(TEXT("\tTotal time elapsed:        %.3lf sec;\n"), fullTimeElapsed);
		_tprintf(TEXT("\tTotal distribute time:     %.3lf sec;\n"), totalDistributeTime);
		_tprintf(TEXT("\tTotal sorting time:        %.3lf sec;\n"), totalSortTime);
		_tprintf(TEXT("\tTotal data read time:      %.3lf sec;\n"), totalReadTime);
		_tprintf(TEXT("\tTotal data write time:     %.3lf sec;\n"), totalWriteTime);
		_tprintf(TEXT("\tTotal data convert time:   %.3lf sec;\n"), totalWriteTime);
		_tprintf(TEXT("\tTotal input validate time: %.3lf sec;\n"), totalValidateTime);
		_tprintf(TEXT("\tTotal output verify time:  %.3lf sec;\n"), totalVerifyTime);
	}
} SortStatistics;

class Algorithm
{
private:
	const uint64_t _MaxIntegerValue = 0xFFFFFFFF;
	const int defaultPivot = 49 * CONVERT_DIV;

	std::vector<std::string> bufferFileNames;
	DWORD sqrtPivot = 0;
	LARGE_INTEGER fileSize;
	uint64_t integerStep;

	clock_t startTimePoint;
	clock_t endTimePoint;

	// Check input file for correct contents.
	bool _ValidateDataFile(std::string fileName = "");
public:
	SortStatistics statistics;

	// Constructor
	Algorithm();

	// sets input file name
	void SetInputFileName(std::string inputFile);

	// Sets output file name.
	void SetOutputFileName(std::string outputFile);

	// Begin distribution sort.
	void DisributeFile();

	// End distribution sort.
	void ConcatenateBuckets();

	// not released
	BOOL VerifyOutputFile();

	virtual uint64_t * SortFile(std::string fileName, DWORD sizeToRead) = 0;

protected:
	std::string inputFile;
	std::string outputFile;
	clock_t timePoint_1;
	clock_t timePoint_2;

	BOOL _ReadFile(std::string fileName, uint64_t * integers,
		DWORD length, DWORD offset, DWORD integersOffset = 0);
};

#endif
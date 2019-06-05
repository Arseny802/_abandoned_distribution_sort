#pragma once
#include "pch.h"
#include <iostream>
#include <math.h>
#include <ctime>
#include "settings.h"
#include "functions.h"
#include "files.h"

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
	bool _ValidateDataFile(std::string fileName = "")
	{
		if (fileName.length() == 0) fileName = inputFile;
		char correctData[] = {'0', '1', '2' , '3' , 
			'4' , '5' , '6' , '7' , '8' , '9' , 
			'A' , 'B' , 'C' , 'D' , 'E' , 'F' };
		startTimePoint = clock();
		
		// file size must multiple 8
		if (GetFileSizeInBytes(fileName).QuadPart % ONE_BYTE != 0)
		{
			std::wstring wstr(fileName.begin(), fileName.end());
			_tprintf(TEXT("ERROR: file \"%s\" has wrong size!\n"), wstr.c_str());
			return FALSE;
		}

		char *buffer = (char*)malloc(REED_FILE__BUFFER_SIZE);
		uint64_t readIter = 0;

		// file data must contain specific characters
		/*while (ReadDataFile(fileName, buffer, REED_FILE__BUFFER_SIZE, readIter))
		{
			char *nextChar = buffer;
			while (*nextChar)
			{
				if (!strchr(correctData, *nextChar))
				{
					std::wstring wstr(fileName.begin(), fileName.end());
					_tprintf(TEXT("ERROR: file \"%s\" has wrong character - '%c'!\n"), wstr.c_str(), nextChar);
				}
			}
			readIter += REED_FILE__BUFFER_SIZE;
		}*/
		free(buffer);

		endTimePoint = clock();
		statistics.totalValidateTime = double(endTimePoint - startTimePoint) / CLOCKS_PER_SEC;
		return TRUE;
	}
public:
	SortStatistics statistics;

	// Constructor
	Algorithm()
	{
		statistics.inputFile = TEXT(DEFAULT_INPUT_FILE);
		statistics.outputFile = TEXT(DEFAULT_OUTPUT_FILE);
	}

	// sets input file name
	void SetInputFileName(std::string inputFile)
	{
		std::wstring wstr(inputFile.begin(), inputFile.end());
		this->inputFile = inputFile;
		statistics.inputFile = wstr;
	}

	// Sets output file name.
	void SetOutputFileName(std::string outputFile)
	{
		std::wstring wstr(inputFile.begin(), inputFile.end());
		this->outputFile = outputFile;
		statistics.outputFile = wstr;
	}

	// Begin distribution sort.
	void DisributeFile()
	{
		if (!_ValidateDataFile()) // check contents
		{
			_tprintf(TEXT("Input file is incorrect or unreadable!\n"));
			return;
		} 
		startTimePoint = clock();

		fileSize = GetFileSizeInBytes(inputFile);
		sqrtPivot = (DWORD)RoundDouble(2 * FileSizeSQRT(fileSize));
		if (sqrtPivot < defaultPivot) sqrtPivot = defaultPivot;
		integerStep = sqrtPivot / ONE_BYTE;
		CreateLocalDirectory(DISTRIBUTION_SORT_TMP_DIR);

		uint64_t * integers = new uint64_t[2 * integerStep];
		char * arrayToWrite = new char[sqrtPivot];

		int leftIter, rightIter, iterBufferFileRead;
		std::string newBufferFN;
		uint64_t integerPivot, tmp;


		for (unsigned long passNum = 0; passNum < fileSize.QuadPart / sqrtPivot; ++passNum)
		{
			iterBufferFileRead = 0;
			timePoint_1 = clock();
			if (passNum == 0) // read left part array
				_ReadFile(inputFile, integers, sqrtPivot, 0, 0);
			else
				_ReadFile(bufferFileNames[iterBufferFileRead + passNum], integers, sqrtPivot, 0, 0);
			timePoint_2 = clock();
			statistics.totalReadTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;

			unsigned long long iterator = 0;
			for (iterator = sqrtPivot * (passNum + 1); iterator < fileSize.QuadPart; iterator += sqrtPivot)
			{
				++iterBufferFileRead;
				timePoint_1 = clock();
				if (passNum == 0) // read right part array
					_ReadFile(inputFile, integers, sqrtPivot, iterator, integerStep);
				else
					_ReadFile(bufferFileNames[iterBufferFileRead + passNum], integers, sqrtPivot, 0, integerStep);
				timePoint_2 = clock();
				statistics.totalReadTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;

				timePoint_1 = clock();
				leftIter = 0;
				rightIter = 2 * integerStep - 1;
				integerPivot = _MaxIntegerValue / integerStep * (iterator / sqrtPivot);
				while (TRUE) // quiek sort buckets
				{
					while (leftIter < rightIter &&
						(integerPivot > integers[leftIter])) ++leftIter;
					while (rightIter >= leftIter &&
						(integers[rightIter] > integerPivot)) --rightIter;

					if (leftIter >= rightIter) break;

					tmp = integers[leftIter];
					integers[leftIter] = integers[rightIter];
					integers[rightIter] = tmp;

					rightIter--; leftIter++;
				}
				timePoint_2 = clock();
				statistics.totalDistributeTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;

				if (passNum == 0) // get tmp file name and remember
				{
					newBufferFN = Concat(Concat(DISTRIBUTION_SORT_TMP_DIR, iterator), DISTRIBUTION_SORT_TMP_FN);
					bufferFileNames.push_back(newBufferFN);
				}
				else // get remembered file name and rewrite file
				{
					newBufferFN = bufferFileNames[iterBufferFileRead + passNum];
					DeleteDataFile(newBufferFN);
				}

				// write left part of buffer to tmp file
				timePoint_1 = clock();
				ConvertUint64ArrayToCharArray(integers, arrayToWrite, integerStep, 0);
				timePoint_2 = clock();
				statistics.totalConvertTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
				timePoint_1 = clock();
				WriteToDataFile(newBufferFN, arrayToWrite, sqrtPivot);
				timePoint_2 = clock();
				statistics.totalWriteTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
			}

			if (passNum == 0) // get tmp file name and remember
			{
				newBufferFN = Concat(Concat(DISTRIBUTION_SORT_TMP_DIR, fileSize.QuadPart), DISTRIBUTION_SORT_TMP_FN);
				bufferFileNames.push_back(newBufferFN);
			}
			else // get remembered file name and rewrite file
			{
				newBufferFN = bufferFileNames[iterBufferFileRead + passNum];
				DeleteDataFile(newBufferFN);
			}

			// write last right part of buffer to tmp file
			DWORD writeLastFileLength = sqrtPivot - (iterator - fileSize.QuadPart);
			timePoint_1 = clock();
			ConvertUint64ArrayToCharArray(integers, arrayToWrite, writeLastFileLength / ONE_BYTE, integerStep - 1);
			timePoint_2 = clock();
			statistics.totalConvertTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
			timePoint_1 = clock();
			WriteToDataFile(newBufferFN, arrayToWrite, writeLastFileLength);
			timePoint_2 = clock();
			statistics.totalWriteTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
		}

		delete[] integers;
		delete[] arrayToWrite;
	}

	// End distribution sort.
	void ConcatenateBuckets()
	{
		std::string bufferFN;
		uint64_t * sortedData;
		char * arrayToWrite = (char *)malloc(sqrtPivot);

		for (unsigned int iter = 0; iter < bufferFileNames.size(); ++iter)
		{
			sortedData = this->SortFile(bufferFileNames[iter], sqrtPivot);
			ConvertUint64ArrayToCharArray(sortedData, arrayToWrite, integerStep);
			timePoint_1 = clock();
			WriteToDataFile(outputFile, arrayToWrite, sqrtPivot);
			timePoint_2 = clock();
			statistics.totalSortTime += double(timePoint_2 - timePoint_1) / CLOCKS_PER_SEC;
			DeleteDataFile(bufferFileNames[iter]);
		}
		bufferFileNames.clear();
		DeleteLocalDirectory(DISTRIBUTION_SORT_TMP_DIR);

		endTimePoint = clock();
		statistics.fullTimeElapsed = double(endTimePoint - startTimePoint) / CLOCKS_PER_SEC;
	}

	// not released
	BOOL VerifyOutputFile()
	{
		_ValidateDataFile(outputFile);
		startTimePoint = clock();

		uint64_t * bufferIntegers = (uint64_t*)malloc(REED_FILE__BUFFER_SIZE / ONE_BYTE);
		uint64_t readIter = 0;

		// file data must be sorted
		/*while (_ReadFile(outputFile, bufferIntegers, REED_FILE__BUFFER_SIZE, readIter))
		{
			for (DWORD iter = 1; iter < REED_FILE__BUFFER_SIZE / ONE_BYTE; ++iter)
			{
				if (bufferIntegers[iter] < bufferIntegers[iter - 1])
				{
					std::wstring wstr(outputFile.begin(), outputFile.end());
					_tprintf(TEXT("ERROR: file \"%s\" is NOT sorted!\n"), wstr.c_str());
					return FALSE;
				}
			}
			readIter += REED_FILE__BUFFER_SIZE;
		}*/
		free(bufferIntegers);

		endTimePoint = clock();
		statistics.totalVerifyTime = double(endTimePoint - startTimePoint) / CLOCKS_PER_SEC;
		return TRUE;
	}

	virtual uint64_t * SortFile(std::string fileName, DWORD sizeToRead) = 0;

protected:
	std::string inputFile;
	std::string outputFile;
	clock_t timePoint_1;
	clock_t timePoint_2;

	BOOL _ReadFile(std::string fileName, uint64_t * integers, 
		DWORD length, DWORD offset, DWORD integersOffset = 0)
	{
		char *buffer = (char*)malloc(length + 1);
		if (ReadDataFile(fileName, buffer, length, offset))
		{
			clock_t convertTimeStart = clock();
			ConvertCharArrayToUint64Array(buffer, integers, 
				length / ONE_BYTE, integersOffset);
			clock_t convertTimeEnd = clock();
			statistics.totalConvertTime += double(convertTimeEnd - convertTimeStart) / CLOCKS_PER_SEC;
			free(buffer);
			return TRUE;
		}
		else
		{
			free(buffer);
			return FALSE;
		}
	}
};
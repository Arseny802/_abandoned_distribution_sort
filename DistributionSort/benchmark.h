#pragma once
#include <tchar.h>
#include <ctime>
#include "files.h"
#include "settings.h"
#include "functions.h"

// Local function to write benchmark file.
// Returns success.
BOOL WriteBenchmarkFile()
{
	clock_t begin = clock();
	char * writeData = (char*)malloc(BENCHMARK__FILE_SIZE);
	GenerateHexIntegers_KB(writeData, BENCHMARK__FILE_SIZE / ONE_BYTE);
	clock_t end_1 = clock();
	LARGE_INTEGER dwTotalFileSize = WriteToDataFile(
		BENCHMARK__FILE_NAME, writeData, BENCHMARK__FILE_SIZE);
	clock_t end_2 = clock();
	free(writeData);
	if (dwTotalFileSize.QuadPart == BENCHMARK__FILE_SIZE)
	{
		double elapsed_secs_dataGen = double(end_1 - begin) / CLOCKS_PER_SEC;
		double elapsed_secs_dataWrite = double(end_2 - end_1) / CLOCKS_PER_SEC;
		_tprintf(TEXT("\tTest file size: %d (bytes);\n"), BENCHMARK__FILE_SIZE - 1);
		_tprintf(TEXT("\tData generation time: %.3lf sec;\n"), elapsed_secs_dataGen);
		_tprintf(TEXT("\tData file write time: %.3lf sec;\n"), elapsed_secs_dataWrite);
		return TRUE;
	}
	else
	{
		_tprintf(TEXT("Benchmark fail! Test file name: %s.\n"), BENCHMARK__FILE_NAME);
		if (dwTotalFileSize.QuadPart == -1)
		{
			_tprintf(TEXT("Error: can't open test file!\n"));
		}
		else if (dwTotalFileSize.QuadPart == -2)
		{
			_tprintf(TEXT("Error: can't write data test file!\n"));
		}
		else
		{
			_tprintf(TEXT("Error: unknown error!\n"));
		}
	}
	return FALSE;
}

// Start benchmark to get latency
// and read/write speed of drive.
inline void CurrentDriveBenchmark()
{
	_tprintf(TEXT("-----------------------\n"));
	_tprintf(TEXT("Current drive benchmark\n"));
	_tprintf(TEXT("-----------------------\n"));
	if (WriteBenchmarkFile())
	{
		_tprintf(TEXT("\tData file read count: %d;\n"), BENCHMARK__FILE_READ_COUNT);
		_tprintf(TEXT("\tData file read buffer size: %d (bytes);\n"), BENCHMARK__FILE_READ_SIZE);
		char * readBuffer = (char *)malloc(BENCHMARK__FILE_READ_SIZE);
		clock_t begin = clock();
		for (unsigned short iterator = 0; iterator < BENCHMARK__FILE_READ_COUNT; ++iterator)
		{
			ReadDataFile(BENCHMARK__FILE_NAME, readBuffer, BENCHMARK__FILE_READ_SIZE,
				rand() % (BENCHMARK__FILE_SIZE - BENCHMARK__FILE_READ_SIZE));
		}
		clock_t end_1 = clock();
		free(readBuffer);
		//delete[] readBuffer;
		double elapsed_secs_read = double(end_1 - begin) / CLOCKS_PER_SEC;
		_tprintf(TEXT("\tData file read time: %.3lf sec.\n"), elapsed_secs_read);
		DeleteDataFile(BENCHMARK__FILE_NAME);
	}
	_tprintf(TEXT("_______________________\n"));
	_tprintf(TEXT("\n\n"));
}
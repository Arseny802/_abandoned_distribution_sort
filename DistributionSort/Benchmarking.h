#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include "WinFiles.h"
#include "Settings.h"
#include "Functions.cpp"

#include <tchar.h>
#include <ctime>

class Benchmarking
{
public:
	Benchmarking();
	~Benchmarking();

	// Local function to write benchmark file.
	// Returns success.
	BOOL WriteBenchmarkFile();

	// Start benchmark to get latency
	// and read/write speed of drive.
	inline void CurrentDriveBenchmark();
private:
	WinFiles * fileManager;
};

#endif
#include "pch.h"
#include "info.h"
#include "benchmark.h"
#include "QuiekSort.cpp"
#include "HeapSort.cpp"

void TestDefaultFile(float size);

int main(int argc, char *argv[])
{
	PrintDeviceInfo();
	//CurrentDriveBenchmark();

	if (argc > 1)
	{
		std::string input = argv[1];
		std::string output = DEFAULT_OUTPUT_FILE;
		if (argc >= 3) output = argv[2];

		QuiekSort solution_1 = QuiekSort(input, output);
		solution_1.DisributeFile();
		solution_1.ConcatenateBuckets();
		solution_1.VerifyOutputFile();
		solution_1.statistics.PrintStatistics();
		PrintDeviceInfoRAM();
	}
	else
	{
		// Test 100 MB file
		TestDefaultFile(0.2);

		// Test 5 GB file
		//TestDefaultFile(5);

		// Test 20 GB file
		//TestDefaultFile(20);
	}

	int a;
	std::cin >> a;
}

// Work function
void TestDefaultFile(float size)
{
	std::cout << "\n\nStart test file creating.\n\n";
	GenerateDataFile(DEFAULT_INPUT_FILE, size);

	PrintDeviceInfoRAM();
	std::cout << "\n\nStart sortintg.\n\n";

	QuiekSort solution_1 = QuiekSort(
		DEFAULT_INPUT_FILE, Concat("1_", DEFAULT_OUTPUT_FILE));
	solution_1.DisributeFile();
	solution_1.ConcatenateBuckets();
	solution_1.statistics.PrintStatistics();
	solution_1.VerifyOutputFile();
	PrintDeviceInfoRAM();

	HeapSort solution_2 = HeapSort(
		DEFAULT_INPUT_FILE, Concat("2_", DEFAULT_OUTPUT_FILE));
	solution_2.DisributeFile();
	solution_2.ConcatenateBuckets();
	solution_2.statistics.PrintStatistics();
	solution_2.VerifyOutputFile();
	PrintDeviceInfoRAM();
}
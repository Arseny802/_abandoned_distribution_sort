#include "DeviceInfo.h"
#include "FileGenerator.h"
#include "Algorithms.h"

void TestDefaultFile(float size);

int main(int argc, char *argv[])
{
	DeviceInfo * deviceInfo = new DeviceInfo();

	deviceInfo->PrintDeviceInfo();
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
		deviceInfo->PrintDeviceInfoRAM();
		delete deviceInfo;
	}
	else
	{
		delete deviceInfo;

		// Test 100 MB file
		TestDefaultFile(0.1);

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
	DeviceInfo * deviceInfo = new DeviceInfo();
	std::cout << "\n\nStart test file creating.\n\n";
	FileGenerator * fileGenerator = new FileGenerator();
	fileGenerator->GenerateDataFile(DEFAULT_INPUT_FILE, size);
	delete fileGenerator;

	deviceInfo->PrintDeviceInfoRAM();
	std::cout << "\n\nStart sortintg.\n\n";

	QuiekSort solution_1 = QuiekSort(
		DEFAULT_INPUT_FILE, Concat("1_", DEFAULT_OUTPUT_FILE));
	solution_1.DisributeFile();
	solution_1.ConcatenateBuckets();
	solution_1.statistics.PrintStatistics();
	solution_1.VerifyOutputFile();
	deviceInfo->PrintDeviceInfoRAM();

	HeapSort solution_2 = HeapSort(
		DEFAULT_INPUT_FILE, Concat("2_", DEFAULT_OUTPUT_FILE));
	solution_2.DisributeFile();
	solution_2.ConcatenateBuckets();
	solution_2.statistics.PrintStatistics();
	solution_2.VerifyOutputFile();
	deviceInfo->PrintDeviceInfoRAM();
	delete deviceInfo;
}
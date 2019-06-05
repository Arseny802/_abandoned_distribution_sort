#include "FileGenerator.h"


FileGenerator::FileGenerator()
{
	fileManager = new WinFiles();
}


FileGenerator::~FileGenerator()
{
	delete fileManager;
}


// Generate datafile with this name and with this size in GB.
inline void FileGenerator::GenerateDataFile(std::string filename, double size_GB)
{
	LARGE_INTEGER dwTotalFileSize = { (DWORD)(size_GB * (double)GB_TO_BYTES) };
	LARGE_INTEGER currentFileSize = fileManager->WriteToDataFile(filename, NULL, NULL);
	DWORD dwWroteBytes = 0, dwCurrentWritingBytes = 0;
	LONG difference = 0;

	_tprintf(TEXT("Starting generating data file.\n"));
	if (currentFileSize.LowPart == -1)
	{
		_tprintf(TEXT("ERROR: unable to open data file for write.\n"));
		return;
	}
	else if (dwTotalFileSize.QuadPart <= currentFileSize.QuadPart)
	{
		_tprintf(TEXT("Data file is already bigger than needed (%.3f GB >= %.3f GB).\n"),
			FileSizeToGB(currentFileSize), size_GB);
		return;
	}

	_tprintf(TEXT("Need to write %.3f GB, current file size is %.3f GB.\n"),
		size_GB, FileSizeToGB(currentFileSize));

	while (dwTotalFileSize.QuadPart > currentFileSize.QuadPart)
	{
		difference = dwTotalFileSize.QuadPart - currentFileSize.QuadPart;
		dwCurrentWritingBytes = WRITE_FILE__BUFFER_SIZE < difference ?
			WRITE_FILE__BUFFER_SIZE : difference;

		char * DataBuffer = (char*)malloc(dwCurrentWritingBytes);

		GenerateHexIntegers_KB(DataBuffer, dwCurrentWritingBytes / ONE_BYTE);
		currentFileSize = fileManager->WriteToDataFile(filename, DataBuffer, dwCurrentWritingBytes);
		free(DataBuffer);

		if (currentFileSize.LowPart == -1)
		{
			_tprintf(TEXT("ERROR: unable to write data to file.\n"));
			break;
		}
		else dwWroteBytes += dwCurrentWritingBytes;
	}
	_tprintf(TEXT("Wrote data file. File size is %.3f GB, wrote %.3f GB.\n"),
		FileSizeToGB(currentFileSize), (float)dwWroteBytes / GB_TO_BYTES);
}
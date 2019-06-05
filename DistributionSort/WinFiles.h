#ifndef WIN_FILES_H
#define WIN_FILES_H

#include "Settings.h"
#include "Functions.cpp"

#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <random>

class WinFiles
{
public:
	WinFiles();
	~WinFiles();
	// Creates directory with this name in local directory
	// Returns success.
	inline BOOL CreateLocalDirectory(std::string dirName);

	// Delete directory with this name in local directory
	// Returns success.
	inline BOOL DeleteLocalDirectory(std::string dirName);

	// Write this data to file with this name.
	// Returns result file size in bytes, 
	// or -1 on open file error,
	// or -2 on write file error.
	inline LARGE_INTEGER WriteToDataFile(std::string filename,
		char * DataBuffer, DWORD dwBytesToWrite);

	// Generate datafile with this name and with this size in GB.
	inline void GenerateDataFile(std::string filename, double size_GB);

	// Read from file by this filename data to this ReadBuffer this length 
	// of bytes from this position. Returnes ContinuenationPoint (is there more data).
	inline BOOL ReadDataFile(std::string filename, char *ReadBuffer, DWORD length, uint64_t distanceFrom);

	// Returns file size in bytes by it's name,
	// or -1 if file doesn't exist,
	// or -2 if couldn't open file.
	inline LARGE_INTEGER GetFileSizeInBytes(std::string filename);

	// Deletes file by it's name. Returns success.
	inline BOOL DeleteDataFile(std::string filename);
};

#endif
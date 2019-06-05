#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include "WinFiles.h"

class FileGenerator
{
public:
	FileGenerator();
	~FileGenerator();
	// Generate datafile with this name and with this size in GB.
	void GenerateDataFile(std::string filename, double size_GB);
private:
	WinFiles * fileManager;
};

#endif
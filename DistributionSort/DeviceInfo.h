#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include "settings.h"
#include "Functions.cpp"

#include <Windows.h>
#include <Ntddscsi.h> 
#include <tchar.h>
#include <iostream>

class DeviceInfo
{
public:
	// print's all RAM statistics
	void PrintDeviceInfoRAM();

	// Get's drive geometry to get params
	BOOL GetDriveGeometry(LPCWSTR wszPath, DISK_GEOMETRY *pdg);

	// Guess it's SSD or HDD
	// Warining: code needs admin rights!
	std::wstring GetFisicalDriveType(LPCWSTR wszPath);

	// print fisical drive info by it's name
	BOOL PrintDeviceInfoDrive(std::string drive);

	// print all important device info
	void PrintDeviceInfo();
};

#endif
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
	inline void PrintDeviceInfoRAM();

	// Get's drive geometry to get params
	inline BOOL GetDriveGeometry(LPCWSTR wszPath, DISK_GEOMETRY *pdg);

	// Guess it's SSD or HDD
	// Warining: code needs admin rights!
	inline std::wstring GetFisicalDriveType(LPCWSTR wszPath);

	// print fisical drive info by it's name
	inline BOOL PrintDeviceInfoDrive(std::string drive);

	// print all important device info
	inline void PrintDeviceInfo();
};

#endif
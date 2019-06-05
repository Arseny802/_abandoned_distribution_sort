#include <Windows.h>
#include <Ntddscsi.h> 
#include <tchar.h>
#include <iostream>
#include "settings.h"
#include "functions.h"

// print's all RAM statistics
inline void PrintDeviceInfoRAM()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	_tprintf(TEXT("----------------------\n"));
	_tprintf(TEXT("Total RAM information:\n"));
	_tprintf(TEXT("----------------------\n"));
	_tprintf(TEXT("\tThere is %*ld percent of memory in use.\n"),
		WIDTH_INFO_OUTPUT, statex.dwMemoryLoad);
	_tprintf(TEXT("\tThere are %*I64d total MB of physical memory.\n"),
		WIDTH_INFO_OUTPUT, statex.ullTotalPhys / CONVERT_DIV);
	_tprintf(TEXT("\tThere are %*I64d free  MB of physical memory.\n"),
		WIDTH_INFO_OUTPUT, statex.ullAvailPhys / CONVERT_DIV);
	_tprintf(TEXT("\tThere are %*I64d total MB of paging file.\n"),
		WIDTH_INFO_OUTPUT, statex.ullTotalPageFile / CONVERT_DIV);
	_tprintf(TEXT("\tThere are %*I64d free  MB of paging file.\n"),
		WIDTH_INFO_OUTPUT, statex.ullAvailPageFile / CONVERT_DIV);
	_tprintf(TEXT("\tThere are %*I64d total MB of virtual memory.\n"),
		WIDTH_INFO_OUTPUT, statex.ullTotalVirtual / CONVERT_DIV);
	_tprintf(TEXT("\tThere are %*I64d free  MB of virtual memory.\n"),
		WIDTH_INFO_OUTPUT, statex.ullAvailVirtual / CONVERT_DIV);
	//_tprintf(TEXT("----------------------\n"));
}

// Get's drive geometry to get params
inline BOOL GetDriveGeometry(LPCWSTR wszPath, DISK_GEOMETRY *pdg)
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	BOOL bResult = FALSE;
	DWORD junk = 0;

	hDevice = CreateFileW(wszPath,// drive to open
		0,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return (FALSE);
	}

	bResult = DeviceIoControl(hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL, 0,
		pdg, sizeof(*pdg),
		&junk,
		(LPOVERLAPPED)NULL);

	CloseHandle(hDevice);

	return (bResult);
}

// Guess it's SSD or HDD
// Warining: code needs admin rights!
inline std::wstring GetFisicalDriveType(LPCWSTR wszPath)
{
	//As an example, let's test 1st physical drive 
	HANDLE hDevice = CreateFile(L"\\\\?\\PhysicalDrive0",
		GENERIC_READ | GENERIC_WRITE,  //We need write access to send ATA command to read RPMs 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == 5) return L"ERROR: need admin rights";
		else return L"ERROR: can't reed driver info";
	}

	DWORD bytesReturned;
	BOOL trim, seekPenalty, rpm;
	unsigned char result = 0;

	//Check TRIM -- should be Y for SSD 
	std::string testInfo = "TRIM=";

	STORAGE_PROPERTY_QUERY spqTrim;
	spqTrim.PropertyId = (STORAGE_PROPERTY_ID)StorageDeviceTrimProperty;
	spqTrim.QueryType = PropertyStandardQuery;

	bytesReturned = 0;
	DEVICE_TRIM_DESCRIPTOR dtd = { 0 };
	if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&spqTrim, sizeof(spqTrim), &dtd, sizeof(dtd), &bytesReturned, NULL) &&
		bytesReturned == sizeof(dtd))
	{
		testInfo = Concat(testInfo, (dtd.TrimEnabled ? "Y" : "N"));
		if (dtd.TrimEnabled) result += 2;
		else result += 1;
		trim = TRUE;
	}
	else
	{
		testInfo = Concat(testInfo, "?");
		trim = FALSE;
	}

	//Check the seek-penalty value -- should be N for SSD 
	testInfo = Concat(testInfo, ";seekPenalty=");

	STORAGE_PROPERTY_QUERY spqSeekP;
	spqSeekP.PropertyId = (STORAGE_PROPERTY_ID)StorageDeviceSeekPenaltyProperty;
	spqSeekP.QueryType = PropertyStandardQuery;

	bytesReturned = 0;
	DEVICE_SEEK_PENALTY_DESCRIPTOR dspd = { 0 };
	if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&spqSeekP, sizeof(spqSeekP), &dspd, sizeof(dspd), &bytesReturned, NULL) &&
		bytesReturned == sizeof(dspd))
	{
		testInfo = Concat(testInfo, (dspd.IncursSeekPenalty ? "Y" : "N"));
		if (!dspd.IncursSeekPenalty) result += 2;
		else result += 1;
		seekPenalty = TRUE;
	}
	else
	{
		testInfo = Concat(testInfo, "?");
		seekPenalty = FALSE;
	}

	struct ATAIdentifyDeviceQuery
	{
		ATA_PASS_THROUGH_EX header;
		WORD data[256];
	};

	//Get drive's RPMs reading -- should be 1 for SSD 
	testInfo = Concat(testInfo, ";RPM=");

	ATAIdentifyDeviceQuery id_query;
	memset(&id_query, 0, sizeof(id_query));

	id_query.header.Length = sizeof(id_query.header);
	id_query.header.AtaFlags = ATA_FLAGS_DATA_IN;
	id_query.header.DataTransferLength = sizeof(id_query.data);
	id_query.header.TimeOutValue = 5; //Timeout in seconds 
	id_query.header.DataBufferOffset = offsetof(ATAIdentifyDeviceQuery, data[0]);
	id_query.header.CurrentTaskFile[6] = 0xec; // ATA IDENTIFY DEVICE 

	bytesReturned = 0;
	if (DeviceIoControl(hDevice, IOCTL_ATA_PASS_THROUGH,
		&id_query, sizeof(id_query), &id_query, sizeof(id_query), &bytesReturned, NULL) &&
		bytesReturned == sizeof(id_query))
	{
		//   Value   Description 
		//   -------------------------------- 
		//   0000h   Rate not reported 
		//   0001h   Non-rotating media (e.g., solid state device) 
		//   0002h-0400h  Reserved 
		//   0401h-FFFEh  Nominal media rotation rate in rotations per minute (rpm) 
		//         (e.g., 7 200 rpm = 1C20h) 
		//   FFFFh   Reserved 
		UINT rpm_value = (UINT)id_query.data[kNominalMediaRotRateWordIndex];
		testInfo = Concat(testInfo, rpm_value);
		if (rpm_value == 1) result += 2;
		else result += 1;
		rpm = TRUE;
	}
	else
	{
		testInfo = Concat(testInfo, "?");
		rpm = FALSE;
	}

	CloseHandle(hDevice);

	// sum of three points is more than 5 
	// (without of undefined)
	if (result >= (trim + seekPenalty + rpm) * 2 - 1)
	{
		std::string str = Concat("SSD: ", testInfo);
		std::wstring wstrBuffer(str.begin(), str.end());
		return wstrBuffer;
	}
	else
	{
		std::string str = Concat("HDD: ", testInfo);
		std::wstring wstrBuffer(str.begin(), str.end());
		return wstrBuffer;
	}
}

// print fisical drive info by it's name
inline BOOL PrintDeviceInfoDrive(std::string drive)
{
	DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
	ULONGLONG DiskSize = 0;    // size of the drive, in bytes

	std::string str = Concat("\\\\.\\", drive);
	std::wstring wstrBuffer1(str.begin(), str.end());
	LPCWSTR wszDrivePath = wstrBuffer1.c_str();

	if (GetDriveGeometry(wszDrivePath, &pdg))
	{
		str = Concat(drive, " information:\n");
		std::wstring wstrBuffer2(str.begin(), str.end());

		str = Concat("\\\\?\\", drive);
		std::wstring wstrBuffer3(str.begin(), str.end());
		std::wstring deviceType = GetFisicalDriveType(wstrBuffer3.c_str());

		_tprintf(TEXT("---------------------------\n"));
		_tprintf(wstrBuffer2.c_str());
		_tprintf(TEXT("---------------------------\n"));
		wprintf(L"\tDrive path      = %ws\n", wszDrivePath);
		wprintf(L"\tDrive type      = %ws\n", deviceType.c_str());
		wprintf(L"\tCylinders       = %I64d\n", pdg.Cylinders);
		wprintf(L"\tTracks/cylinder = %ld\n", (ULONG)pdg.TracksPerCylinder);
		wprintf(L"\tSectors/track   = %ld\n", (ULONG)pdg.SectorsPerTrack);
		wprintf(L"\tBytes/sector    = %ld\n", (ULONG)pdg.BytesPerSector);

		DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
			(ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
		wprintf(L"\tDisk size       = %I64d (Bytes)\n"
			L"\t                = %.2f (Gb)\n",
			DiskSize, (double)DiskSize / (1024 * 1024 * 1024));
		//_tprintf(TEXT("---------------------------\n"));
		return TRUE;
	}
	else
	{
		//wprintf(L"\tGetDriveGeometry failed. Error %ld.\n", GetLastError());
		//_tprintf(TEXT("---------------------------\n"));
		return FALSE;
	}
}

// print all important device info
inline void PrintDeviceInfo()
{
	_tprintf(TEXT("__________________\n"));
	_tprintf(TEXT("DEVICE INFORMATION\n"));
	PrintDeviceInfoRAM();
	for (int iter = 0; iter < 10; ++iter)
	{
		if (!PrintDeviceInfoDrive(Concat(
			"PhysicalDrive", iter))) break;
	}
	_tprintf(TEXT("__________________\n"));
	_tprintf(TEXT("\n\n"));
}
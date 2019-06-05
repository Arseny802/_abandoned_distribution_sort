#include "WinFiles.h"

WinFiles::WinFiles()
{

}

WinFiles::~WinFiles()
{

}

// Creates directory with this name in local directory
// Returns success.
inline BOOL WinFiles::CreateLocalDirectory(std::string dirName)
{
	std::wstring wstr1(dirName.begin(), dirName.end());
	LPCWSTR wszDirName = wstr1.c_str();
	return CreateDirectory(wszDirName, NULL);
}

// Delete directory with this name in local directory
// Returns success.
inline BOOL WinFiles::DeleteLocalDirectory(std::string dirName)
{
	std::wstring wstr1(dirName.begin(), dirName.end());
	LPCWSTR wszDirName = wstr1.c_str();
	return RemoveDirectory(wszDirName);
}

// Write this data to file with this name.
// Returns result file size in bytes, 
// or -1 on open file error,
// or -2 on write file error.
inline LARGE_INTEGER WinFiles::WriteToDataFile(std::string filename,
	char * DataBuffer, DWORD dwBytesToWrite)
{
	HANDLE fileHandle;
	DWORD dwBytesWritten = 0;
	LARGE_INTEGER lFileSize = { 0 };
	PLARGE_INTEGER plFileSize = &lFileSize;
	BOOL bErrorFlag = FALSE;
	std::wstring wstr1(filename.begin(), filename.end());
	LPCWSTR wszPath = wstr1.c_str();

	// if file already exists
	if (GetFileAttributes(wszPath) != DWORD(-1))
	{
		fileHandle = CreateFileW(wszPath,
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		GetFileSizeEx(fileHandle, plFileSize);
		SetFilePointer(fileHandle, NULL, NULL, FILE_END);
	}
	else
	{
		fileHandle = CreateFile(wszPath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_FLAG_WRITE_THROUGH,
			NULL);
	}

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		//_tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), wszPath);
		return { (DWORD)-1 };
	}
	else
	{
		//_tprintf(TEXT("File %s opened successfully. Current file size is %.3f GB.\n"), 
		//	wszPath, FileSizeToGB(lFileSize));

	}

	// if data buffer to write exists
	if (DataBuffer != NULL && dwBytesToWrite != NULL)
	{
		bErrorFlag = WriteFile(
			fileHandle,
			DataBuffer,      // start of data to write
			dwBytesToWrite,  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure
		FlushFileBuffers(fileHandle);

		if (FALSE == bErrorFlag)
		{
			//_tprintf(TEXT("Terminal failure: Unable to write to file.\n"));
			return { (DWORD)-2 };
		}
		/*else
		{
		if (dwBytesWritten != dwBytesToWrite)
		{
		// This is an error because a synchronous write that results in
		// success (WriteFile returns TRUE) should write all data as
		// requested. This would not necessarily be the case for
		// asynchronous writes.
		printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
		_tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten, wszPath);
		}
		}*/
	}

	GetFileSizeEx(fileHandle, plFileSize);
	CloseHandle(fileHandle);

	return lFileSize;
}

// Read from file by this filename data to this ReadBuffer this length 
// of bytes from this position. Returnes ContinuenationPoint (is there more data).
inline BOOL WinFiles::ReadDataFile(std::string filename, char *ReadBuffer, DWORD length, uint64_t distanceFrom)
{
	HANDLE fileHandle;
	DWORD dwBytesRead = 0;
	LPDWORD bytesRead = &dwBytesRead;
	BOOL continuenationPoint = TRUE;
	//char ReadBuffer[REED_FILE__BUFFER_SIZE] = { 0 };
	std::wstring wstr1(filename.begin(), filename.end());
	LPCWSTR wszPath = wstr1.c_str();

	fileHandle = CreateFile(wszPath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), wszPath);
		continuenationPoint = FALSE;
		return continuenationPoint;
	}

	LARGE_INTEGER liDistance;
	liDistance.QuadPart = distanceFrom;
	SetFilePointer(fileHandle, liDistance.LowPart, &liDistance.HighPart, FILE_BEGIN);
	if (liDistance.LowPart == INVALID_SET_FILE_POINTER &&
		GetLastError() != NO_ERROR)
	{
		continuenationPoint = FALSE;
		CloseHandle(fileHandle);
		return continuenationPoint;
	}

	//do
	//{
	if (!ReadFile(fileHandle, ReadBuffer, length - 1, bytesRead, NULL))
	{
		_tprintf(TEXT("Terminal failure: Unable to read from file.\n GetLastError=%08x\n"), GetLastError());
		CloseHandle(fileHandle);
		continuenationPoint = FALSE;
		return continuenationPoint;
	}

	if (dwBytesRead > 0 && dwBytesRead <= length - 1)
	{
		ReadBuffer[dwBytesRead - 1] = '\0'; // NULL character
											//std::string readBufferString = Concat(ReadBuffer, "\n");
											//std::wstring wstr2(readBufferString.begin(), readBufferString.end());

											//_tprintf(TEXT("Data read from %s (%d bytes): \n"), wszPath, dwBytesRead);
											//_tprintf(wstr2.c_str());
		continuenationPoint = TRUE;
	}
	else if (dwBytesRead == 0)
	{
		//_tprintf(TEXT("No data read from file %s\n"), wszPath);
		continuenationPoint = FALSE;
	}
	else
	{
		_tprintf(TEXT("\n ** Terminal failure: error while reading file. ** \n"));
		continuenationPoint = FALSE;
	}
	//} while (continuenationPoint);

	CloseHandle(fileHandle);

	return continuenationPoint;
}

// Returns file size in bytes by it's name,
// or -1 if file doesn't exist,
// or -2 if couldn't open file.
inline LARGE_INTEGER WinFiles::GetFileSizeInBytes(std::string filename)
{
	HANDLE fileHandle;
	LARGE_INTEGER lFileSize = { 0 };
	PLARGE_INTEGER plFileSize = &lFileSize;
	std::wstring wstr1(filename.begin(), filename.end());
	LPCWSTR wszPath = wstr1.c_str();

	if (GetFileAttributes(wszPath) != DWORD(-1))// file exists
	{
		fileHandle = CreateFileW(wszPath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		GetFileSizeEx(fileHandle, plFileSize);
	}
	else return { (DWORD)-1 };

	if (fileHandle == INVALID_HANDLE_VALUE) // can't open file
	{
		return { (DWORD)-2 };
	}

	GetFileSizeEx(fileHandle, plFileSize);
	CloseHandle(fileHandle);

	return lFileSize;
}

// Deletes file by it's name. Returns success.
inline BOOL WinFiles::DeleteDataFile(std::string filename)
{
	std::wstring wstr1(filename.begin(), filename.end());
	LPCWSTR wszPath = wstr1.c_str();
	BOOL result;

	try
	{
		result = DeleteFile(wszPath);
	}
	catch (int error)
	{
		if (error = ERROR_FILE_NOT_FOUND)
		{
			_tprintf(TEXT("Can't delete file %s - file not found!\n"), wszPath);
		}
		else if (error = ERROR_ACCESS_DENIED)
		{
			_tprintf(TEXT("Can't delete file %s - access denied!\n"), wszPath);
			_tprintf(TEXT("Make sure that file isn't marked as read-only.\n"), wszPath);
		}
	}
	return result;
}
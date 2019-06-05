
#include "settings.h"

#include <sstream>
#include <random>
#include <Windows.h>
#include <unordered_map>

// concatinate string with somw object
template <typename T>
inline std::string Concat(std::string str, T value)
{
	std::ostringstream sstm;
	sstm << str << value;
	return sstm.str();
}

inline double RoundDouble(double val)
{
	if (val < 0) return ceil(val - 0.5);
	return floor(val + 0.5);
}

/*inline uint64_t ConvertHexToUint64(char * input)
{
uint64_t toReturn;
std::stringstream ss;
ss << std::hex << input;
ss >> toReturn;
return toReturn;
}*/

inline void ConvertCharArrayToUint64Array(char * input, uint64_t * result,
	unsigned long length, long offset = 0)
{
	std::unordered_map<char, int> hexConvTable{
		{ '0', 0 },{ '1', 1 },{ '2', 2 },
	{ '3', 3 },{ '4', 4 },{ '5', 5 },
	{ '6', 6 },{ '7', 7 },{ '8', 8 },
	{ '9', 9 },{ 'a', 10 },{ 'A', 10 },
	{ 'b', 11 },{ 'B', 11 },{ 'c', 12 },
	{ 'C', 12 },{ 'd', 13 },{ 'D', 13 },
	{ 'e', 14 },{ 'E', 14 },{ 'f', 15 },
	{ 'F', 15 },{ 'x', 0 },{ 'X', 0 } };

	for (unsigned long itreator = 0; itreator < length; ++itreator)
	{
		result[offset + itreator] = hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 0]] << 28;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 1]] << 24;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 2]] << 20;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 3]] << 16;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 4]] << 12;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 5]] << 8;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 6]] << 4;
		result[offset + itreator] += hexConvTable[(std::size_t)input[itreator * ONE_BYTE + 7]];
	}
}

inline void ConvertUint64ToCharArray(uint64_t input, char * result, long offset = 0)
{
	char bufferHEX[ONE_BYTE + 1] = { 0 };
	sprintf_s(bufferHEX, "%08lX", input);
	result[offset * ONE_BYTE + 0] = bufferHEX[0];
	result[offset * ONE_BYTE + 1] = bufferHEX[1];
	result[offset * ONE_BYTE + 2] = bufferHEX[2];
	result[offset * ONE_BYTE + 3] = bufferHEX[3];
	result[offset * ONE_BYTE + 4] = bufferHEX[4];
	result[offset * ONE_BYTE + 5] = bufferHEX[5];
	result[offset * ONE_BYTE + 6] = bufferHEX[6];
	result[offset * ONE_BYTE + 7] = bufferHEX[7];
}

// Generate char array data of integers in HEX.
inline void ConvertUint64ArrayToCharArray(uint64_t * input,
	char * result, long length = 0, long offset = 0)
{
	DWORD dwIntegersToWrite = length > 0 ? length :
		(DWORD)(sizeof(input) / sizeof(*input) - offset);
	//result = (char*)malloc(dwIntegersToWrite * ONE_BYTE);

	for (unsigned long iterator = 0; iterator < dwIntegersToWrite; ++iterator)
	{
		ConvertUint64ToCharArray(input[offset + iterator], result, iterator);
	}
}

// Generate char array data of integers in HEX.
inline void GenerateHexIntegers_KB(char * result, DWORD length)
{
	uint64_t random_variable = 0;
	std::random_device random;
	std::mt19937_64 gen(random());

	//DWORD dwIntegersToWrite = (DWORD)(strlen(result) / KB_OF_INTEGERS);
	for (unsigned long iterator = 0; iterator < length; ++iterator)
	{
		/*char bufferHEX[ONE_BYTE + 1] = { 0 };
		std::uniform_int_distribution<uint64_t> distribution;
		random_variable = distribution(gen);
		sprintf_s(bufferHEX, "%08lX", random_variable);
		result[itreator * ONE_BYTE + 0] = bufferHEX[0];
		result[itreator * ONE_BYTE + 1] = bufferHEX[1];
		result[itreator * ONE_BYTE + 2] = bufferHEX[2];
		result[itreator * ONE_BYTE + 3] = bufferHEX[3];
		result[itreator * ONE_BYTE + 4] = bufferHEX[4];
		result[itreator * ONE_BYTE + 5] = bufferHEX[5];
		result[itreator * ONE_BYTE + 6] = bufferHEX[6];
		result[itreator * ONE_BYTE + 7] = bufferHEX[7];*/
		std::uniform_int_distribution<uint64_t> distribution;
		ConvertUint64ToCharArray(distribution(gen), result, iterator);
	}
}

// Gets file size in GB (float) by this byte length.
inline float FileSizeToGB(LARGE_INTEGER fileSize)
{
	float low = (float)fileSize.LowPart / GB_TO_BYTES;
	float high = (float)fileSize.HighPart * 2147483648 / GB_TO_BYTES;
	return high + low;
}

// Gets file size in GB (float) by this byte length.
inline double FileSizeSQRT(LARGE_INTEGER fileSize)
{
	double low = sqrt((double)fileSize.LowPart);
	double high = sqrt((double)fileSize.HighPart * 2147483648);
	return high + low;
}

/*template <typename T>
void LogToConsole(std::string message, T * objects = NULL)
{
if (objects != NULL)
{
for each (T object in objects)
{

}
}

_tprintf(TEXT("Make sure that file isn't marked as read-only.\n"), message);
}

template <typename T>
void LogToConsole(const std::string& message)
{
LogToConsole(str(message), objects);
}

template <typename T>
void LogToConsole(const char message[])
{
LogToConsole(str(message), objects);
}

template <typename T>
void LogToConsole(const char *message)
{
LogToConsole(str(message), objects);
}

template <typename T>
void LogToConsole1(char * message)
{
LogToConsole(str(message), objects);
}*/
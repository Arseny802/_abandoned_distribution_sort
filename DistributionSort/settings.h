#ifndef SETTINGS_H
#define SETTINGS_H

// create test file or use existing
#define CREATE_TEST_FILE TRUE

// one byte length
#define ONE_BYTE 8

// 1 KB of integers
#define KB_OF_INTEGERS 128

// 1 MB of integers
#define MB_OF_INTEGERS 131072

// byte to MB or KB to GB
#define CONVERT_DIV 1048576

// convert one GB to bytes
#define GB_TO_BYTES 1073741824

// width of text output
#define WIDTH_INFO_OUTPUT 5

// read file buffer
#define WRITE_FILE__BUFFER_SIZE 10 * MB_OF_INTEGERS * ONE_BYTE

// read file buffer
#define REED_FILE__BUFFER_SIZE KB_OF_INTEGERS * ONE_BYTE + 1

// file with integers
#define DEFAULT_INPUT_FILE "default_input.bin"

// sorted file
#define DEFAULT_OUTPUT_FILE "default_output.bin"

// distributed sort tmp file directory name
#define DISTRIBUTION_SORT_TMP_DIR "DS_TMP\\"

// distributed sort tmp file mask name
#define DISTRIBUTION_SORT_TMP_FN "_tmp_DistributionSort.bin"

// benchmark work file name
#define BENCHMARK__FILE_NAME "DS_benchmark.bin"

// benchmark work file name
#define BENCHMARK__FILE_SIZE 512 * MB_OF_INTEGERS * ONE_BYTE

// benchmark work file name
#define BENCHMARK__FILE_READ_SIZE 512 * ONE_BYTE + 1

// benchmark work file name
#define BENCHMARK__FILE_READ_COUNT 10000

// index of RMP value in ATAIdentifyDeviceQuery
#define kNominalMediaRotRateWordIndex 217 

#endif
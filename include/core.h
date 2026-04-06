
#ifndef _CORE_H_
#define _CORE_H_

// MACROS ////////////////////////////////////////////////////////////////////////////////
#define OUT_FEXT ".out"
#define MAX_FPATH_LEN 100

// STRUCTS ////////////////////////////////////////////////////////////////////////////////
typedef struct _Elements_
{
    int count;
    double* array;
    int* binIdxs;
} Elements;

typedef struct _Bin_
{
    double sum;
    int elementCount;
} Bin;

typedef struct _BinsCollection_
{
    int count;
    Bin* array;
} BinsCollection;

typedef struct _OutputData_
{
    int validBinCount;     // number of valid bins (el sum >= 1)
    int* binOffsets;  // per bin element offsets
    int* sortedElementIdxs; // element indices sorted by bin idx
} OutputData;


// USER DEFINES ////////////////////////////////////////////////////////////////////////////
// #define PRINT_DEBUG_INFO
#define SAVE_OUTPUT_FILE

// FUNCTIONS ///////////////////////////////////////////////////////////////////////////////
void mainAlgorithm(char* fname);

#endif  // _CORE_H_

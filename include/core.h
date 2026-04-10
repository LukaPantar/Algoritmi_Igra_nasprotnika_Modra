
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
    int idx;
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
// #define DEBUG_TRACE
// #define DEBUG_TRACE_BST
#define SAVE_OUTPUT_FILE

// Define algorith for solving (if more than one is defined, the first defined will be used)
// #define NAIVE_ALG
// #define THRESHOLD_ALG
#define BINARY_TREE_ALG

// FUNCTIONS ///////////////////////////////////////////////////////////////////////////////
void mainAlgorithm(char* fname);

#endif  // _CORE_H_

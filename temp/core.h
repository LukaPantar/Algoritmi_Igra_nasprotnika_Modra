
#ifndef _CORE_H_
#define _CORE_H_

// MACROS ////////////////////////////////////////////////////////////////////////////////
#define OUT_FEXT ".out"
#define MAX_FPATH_LEN 100

// STRUCTS ////////////////////////////////////////////////////////////////////////////////
typedef struct _InputData_
{
    int elementCount;
    double* elementArray;
} InputData;

typedef struct _OutputData_
{
    int binCount;     // number of valid bins (el sum >= 1)
    int* binOffsets;  // per bin element offsets
    int* sortedElemementIdxs; // element indices sorted by bin idx
} OutputData;


// USER DEFINES ////////////////////////////////////////////////////////////////////////////
// #define PRINT_DEBUG_INFO
#define SAVE_OUTPUT_FILE

// FUNCTIONS ///////////////////////////////////////////////////////////////////////////////
void mainAlgorithm(char* fname);

#endif  // _CORE_H_


#ifndef _CORE_H_
#define _CORE_H_

// MACROS ////////////////////////////////////////////////////////////////////////////////
#define OUT_FEXT ".out"
#define MAX_FPATH_LEN 100

// STRUCTS ////////////////////////////////////////////////////////////////////////////////
typedef struct _Elements_
{
    int len;       // Number of elements
    double* el;    // Array of elements
} Elements;

typedef struct _Bin_
{
    double size;   // Current size of bin
    int lenEl;     // Number of elements in the bin
    int* elIdx;   // Indexes of elements in the bin
} Bin;

typedef struct _Bins_
{
    int len;       // Number of bins
    Bin* b;       // Array of bins
} Bins;


// USER DEFINES ////////////////////////////////////////////////////////////////////////////
// #define PRINT_DEBUG_INFO
#define SAVE_OUTPUT_FILE

// FUNCTIONS ///////////////////////////////////////////////////////////////////////////////
void mainAlgorithm(char* fname);

#endif  // _CORE_H_

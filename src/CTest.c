
// SYSTEM LIBS //////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IMPORTED LIBS //////////////////////////////////////////////////////////////////////////
#include "CTest.h"
#include "core.h"

void testBins(CTest *test, char* fname)
{
    FILE* inFptr;
    char inFullFpath[MAX_FPATH_LEN];
    snprintf(inFullFpath, MAX_FPATH_LEN, "%s%s%s", TESTS_FPATH, fname, TXT_FEXT);

    // MAIN ALGORITHM /////////////////////////////////////////////////////////////////////
    mainAlgorithm(inFullFpath);
    ///////////////////////////////////////////////////////////////////////////////////////

    // Read input file
    inFptr = fopen(inFullFpath, "r");
    if (inFptr == NULL)
    {
        printf("%s\n", inFullFpath);
        printRed("The input file is not exist!");
        exit(EXIT_FAILURE);
    }

    int numElements;
    fscanf(inFptr, "%d", &numElements);
    double elements[numElements];
    for (int i = 0; i < numElements; i++)
    {
        fscanf(inFptr, "%lf", &elements[i]);
    }
    fclose(inFptr);

    // Read output file
    FILE* outFptr;
    char outFullFpath[MAX_FPATH_LEN];
    snprintf(outFullFpath, MAX_FPATH_LEN, "%s%s%s", TESTS_FPATH, fname, OUT_FEXT);

    outFptr = fopen(outFullFpath, "r");
    if (outFptr == NULL)
    {
        printRed("The output file is not exist!");
        exit(EXIT_FAILURE);
    }

    int numBins;
    fscanf(outFptr, "%d", &numBins);
    int numBinsEl[numBins];

    int** bins = malloc(numBins * sizeof(int*));
    for (int i = 0; i < numBins; i++)
        bins[i] = malloc(numElements * sizeof(int));

    int ch;
    while ((ch = fgetc(outFptr)) != '\n' && ch != EOF);
    
    char line[1024];
    for (int i = 0; i < numBins; i++) 
    {
        int j = 0;
        fgets(line, sizeof(line), outFptr);

        char* token = strtok(line, " \n");
        while (token != NULL) 
        {
            bins[i][j] = atoi(token);
            j++;

            token = strtok(NULL, " \n");
        }
        numBinsEl[i] = j;
    }
    fclose(outFptr);

    // Main tests
    assertBinElements(test, numElements, numBins, numBinsEl, bins, "Check bin elements", false);
    assertBinSizes(test, numElements, elements, numBins, numBinsEl, bins, "Check bin size", false);

    // Free
    for (int i = 0; i < numBins; i++)
        free(bins[i]);
    free(bins);
}

int main()
{
    CTest *test = createCTest();  // Setup test
    
    testBins(test, "base");
    testBins(test, "uniform");
    testBins(test, "constant");
    testBins(test, "linear");
    testBins(test, "normal");
    testBins(test, "exponential");
    
    // Custom file for manual tests
    testBins(test, "manual_test");
      
    run(test);                  // Run test
}

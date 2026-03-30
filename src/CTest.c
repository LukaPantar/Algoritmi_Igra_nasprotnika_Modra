
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CTest.h"
#include "core.h"

#define IN_FNAME "in_"
#define OUT_FNAME "out_"
#define TXT_FILE_EXT ".txt"

#define MAX_FNAME_LEN 50


void testBins(CTest *test, char* fname)
{
    // TODO execute the core function

    FILE* inFptr;
    char inFullFname[MAX_FNAME_LEN];
    strcpy(inFullFname, IN_FNAME);
    strcat(inFullFname, fname); 
    strcat(inFullFname, TXT_FILE_EXT);

    inFptr = fopen(inFullFname, "r");
    if (inFptr == NULL)
    {
        printRed("The input file is not exist!");
        exit(1);
    }

    int numElements;
    fscanf(inFptr, "%d", &numElements);
    double elements[numElements];
    for (int i = 0; i < numElements; i++)
    {
        fscanf(inFptr, "%lf", &elements[i]);
    }
    fclose(inFptr);

    FILE* outFptr;
    char outFullFname[MAX_FNAME_LEN];
    strcpy(outFullFname, OUT_FNAME);
    strcat(outFullFname, fname); 
    strcat(outFullFname, TXT_FILE_EXT); 

    outFptr = fopen(outFullFname, "r");
    if (outFptr == NULL)
    {
        printRed("The output file is not exist!");
        exit(1);
    }

    int numBins;
    fscanf(outFptr, "%d", &numBins);
    int numBinsEl[numBins];

    int** bins = malloc(numBins * sizeof(int*));
    for (int i = 0; i < numBins; i++)
        bins[i] = malloc(numElements * sizeof(int));

    int ch;
    while ((ch = fgetc(outFptr)) != '\n' && ch != EOF);
    
    char line[256];  // TODO
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
    
    testBins(test, "1");
      
    run(test);                  // Run test
}

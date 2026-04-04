
// SYSTEM LIBS //////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// IMPORTED LIBS //////////////////////////////////////////////////////////////////////////
#include "core.h"

void readInputFile(char* inFpath, InputData* input)
{
    FILE* fPtr;
    fPtr = fopen(inFpath, "r");
    if (fPtr == NULL)
    {
        printf("The input file is not exist!");
        exit(EXIT_FAILURE);
    }

    fscanf(fPtr, "%d", &(input->elementCount));
    input->elementArray = malloc(input->elementCount * sizeof(double));
    for (int i = 0; i < input->elementCount; i++)
    {
        fscanf(fPtr, "%lf", &(input->elementArray[i]));
    }
    fclose(fPtr);
}

void saveOutputFile(char* inFpath, OutputData* output)
{
    char fullFpath[MAX_FPATH_LEN];
    strcpy(fullFpath, inFpath);

    char* dot = strrchr(fullFpath, '.'); // find last '.'
    if (dot != NULL)
    {
        strcpy(dot, OUT_FEXT); // replace extension
    }
    else
    {
        strcat(fullFpath, OUT_FEXT); // no extension so append
    }

    FILE* fPtr;
    fPtr = fopen(fullFpath, "w");
    if (fPtr == NULL)
    {
        printf("The output file is not exist!");
        exit(EXIT_FAILURE);
    }

    fprintf(fPtr, "%d\n", output->binCount); // bin count
    for (int i = 0; i < output->binCount; i++)
    {
        int startIdx = output->binOffsets[i];
        int end = output->binOffsets[i + 1];
        for (int j = startIdx; j < end; j++)
        {
            fprintf(fPtr, "%d ", output->sortedElemementIdxs[j]);
        }

        fprintf(fPtr, "\n");
    }
    fclose(fPtr);
}

void processOutput(InputData* input, int* outBinIdxs, OutputData* output)
{
    // count bin count
    int maxBinIdx = 0;
    for (int i = 0; i < input->elementCount; i++)
    {
        if (outBinIdxs[i] > maxBinIdx)
        {
            maxBinIdx = outBinIdxs[i];
        }
    }
    output->binCount = maxBinIdx + 1;

    // count bin offsets
    output->binOffsets = malloc((output->binCount + 1) * sizeof(int)); // bin idx -> element offset
    for (int i = 0; i < output->binCount + 1; i++)
    {
        output->binOffsets[i] = 0;
    }
    for (int i = 0; i < input->elementCount; i++)
    {
        int binIdx = outBinIdxs[i];
        output->binOffsets[binIdx + 1]++;
    }
    for (int i = 1; i < output->binCount + 1; i++)
    {
        output->binOffsets[i] += output->binOffsets[i - 1];
    }

    // sort element indices by bin idx
    output->sortedElemementIdxs = malloc(input->elementCount * sizeof(int));
    for (int i = 0; i < input->elementCount; i++)
    {
        int binIdx = outBinIdxs[i];
        int offset = output->binOffsets[binIdx]++;
        output->sortedElemementIdxs[offset] = i;
    }
}

/**
 * @brief Naive implementaiton of sort. If the previous bin is not filled, the next element is added.
 *
 */
void naiveSort(InputData* input, OutputData* output)
{
    ////// solve problem:
    int currentBinIdx = 0;
    double currentBinSize = 0;

    output->binOffsets = malloc(input->elementCount * sizeof(int)); // bin idx -> element offset
    output->binOffsets[0] = 0;

    for (int i = 0; i < input->elementCount; i++)
    {
        double element = input->elementArray[i];

        // insert element in bin
        currentBinSize += element;

        if (currentBinSize > 1.0)
        {
            // init new bin
            currentBinIdx++;
            currentBinSize = 0;
            output->binOffsets[currentBinIdx] = i + 1;
        }
    }

    // discard last bin if not valid
    if (currentBinSize <= 1.0)
    {
        currentBinIdx--;
    }
    output->binCount = currentBinIdx + 1;
}

void naiveSort2(InputData* input, int* outBinIdxs)
{
    int currentBinIdx = 0;
    double currentBinSize = 0;

    for (int i = 0; i < input->elementCount; i++)
    {
        double element = input->elementArray[i];

        // insert element in bin
        currentBinSize += element;
        outBinIdxs[i] = currentBinIdx;

        if (currentBinSize > 1.0)
        {
            // init new bin
            currentBinIdx++;
            currentBinSize = 0;
        }
    }
}

void mainAlgorithm(char* inFpath)
{
    clock_t startInFileTime = clock();
    InputData* input = malloc(sizeof(InputData));
    readInputFile(inFpath, input);
    clock_t stopInFileTime = clock();
    printf("Open and parse input file: %lf s\n", (double)(stopInFileTime - startInFileTime) / CLOCKS_PER_SEC);

    clock_t startNaiveSortTime = clock();
    int* outBinIdxs = malloc(input->elementCount * sizeof(int));
    naiveSort2(input, outBinIdxs);
    clock_t stopNaiveSortTime = clock();
    printf("Naive sorting: %lf s\n", (double)(stopNaiveSortTime - startNaiveSortTime) / CLOCKS_PER_SEC);

#ifdef SAVE_OUTPUT_FILE
    clock_t startOutFileTime = clock();
    OutputData* output = malloc(sizeof(OutputData));
    processOutput(input, outBinIdxs, output);
    saveOutputFile(inFpath, output);
    clock_t stopOutFileTime = clock();
    printf("Save the output file: %lf s\n", (double)(stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC);
#endif  // SAVE_OUTPUT_FILE

    // Print results ////////////////////////////////////////////////////////////////////////////////////////
    double readFileTime = (double) (stopInFileTime - startInFileTime) / CLOCKS_PER_SEC;
    double computeTime = (double) (stopNaiveSortTime - startNaiveSortTime) / CLOCKS_PER_SEC;
    double writeFileTime = (double) (stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC;

    printf("\n");
    printf("Number of bins: %d\n", output->binCount);
    printf("Total time: %lf s\n", readFileTime + computeTime + writeFileTime);

    // Free /////////////////////////////////////////////////////////////////////////////////////////////////
    free(output->sortedElemementIdxs);
    free(output->binOffsets);
    free(output);
    free(outBinIdxs);
    free(input->elementArray);
    free(input);
}

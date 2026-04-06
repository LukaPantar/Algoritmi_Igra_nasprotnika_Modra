
// SYSTEM LIBS //////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// IMPORTED LIBS //////////////////////////////////////////////////////////////////////////
#include "core.h"

void readInputFile(char* inFpath, Elements* elementsData)
{
    FILE* fPtr;
    fPtr = fopen(inFpath, "r");
    if (fPtr == NULL)
    {
        printf("The input file is not exist!");
        exit(EXIT_FAILURE);
    }

    fscanf(fPtr, "%d", &(elementsData->count));
    elementsData->array = malloc(elementsData->count * sizeof(double));
    for (int i = 0; i < elementsData->count; i++)
    {
        fscanf(fPtr, "%lf", &(elementsData->array[i]));
    }
    elementsData->binIdxs = malloc(elementsData->count * sizeof(int));
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

    fprintf(fPtr, "%d\n", output->validBinCount);
    for (int i = 0; i < output->validBinCount; i++)
    {
        int startIdx = output->binOffsets[i];
        int end = output->binOffsets[i + 1];
        for (int j = startIdx; j < end; j++)
        {
            fprintf(fPtr, "%d ", output->sortedElementIdxs[j]);
        }

        fprintf(fPtr, "\n");
    }
    fclose(fPtr);
}

void processOutput(Elements* elementsData, BinsCollection* bins, OutputData* outData)
{
    // count valid bins
    outData->validBinCount = 0;
    for (int i = 0; i < bins->count; i++)
    {
        if (bins->array[i].sum >= 1.0)
        {
            outData->validBinCount++;
        }
    }

    // count bin offsets
    int offsetsCount = bins->count + 1;
    outData->binOffsets = malloc(offsetsCount * sizeof(int));
    outData->binOffsets[0] = 0;
    for (int i = 1; i < offsetsCount; i++)
    {
        outData->binOffsets[i] = outData->binOffsets[i - 1] + bins->array[i - 1].elementCount;
    }

    // sort element indices by bin idx
    int binInsertedElementCount[bins->count];
    memset(binInsertedElementCount, 0, sizeof(binInsertedElementCount));
    outData->sortedElementIdxs = malloc(elementsData->count * sizeof(int));
    for (int i = 0; i < elementsData->count; i++)
    {
        int binIdx = elementsData->binIdxs[i];
        int offset = outData->binOffsets[binIdx] + binInsertedElementCount[binIdx];
        // insert element at offset (binOffset + elementOffset)
        outData->sortedElementIdxs[offset] = i + 1;
        
        binInsertedElementCount[binIdx]++; // increment element offset
    }
}

// bin sorting utils:
void insertElement(Elements* elementsData, BinsCollection* bins, int binIdx, int elementIdx)
{
    elementsData->binIdxs[elementIdx] = binIdx;
    bins->array[binIdx].sum += elementsData->array[elementIdx];
    bins->array[binIdx].elementCount++;
}

void initBin(BinsCollection* bins, int binIdx)
{
    bins->array[binIdx].sum = 0.0;
    bins->array[binIdx].elementCount = 0;
}

// bin sorting algorithms:
void naiveSort(Elements* elementsData, BinsCollection* bins)
{
    int currentBinIdx = 0;
    initBin(bins, 0);

    for (int elIdx = 0; elIdx < elementsData->count; elIdx++)
    {
        if (bins->array[currentBinIdx].sum >= 1.0)
        {
            initBin(bins, ++currentBinIdx);
        }
        insertElement(elementsData, bins, currentBinIdx, elIdx);
    }

    bins->count = currentBinIdx + 1;
}

void mainAlgorithm(char* inFpath)
{
    printf("------------------------------------------------------------\n");
    printf("File path %s\n", inFpath);

    clock_t startInFileTime = clock();
    Elements elementsData;
    readInputFile(inFpath, &elementsData);
    clock_t stopInFileTime = clock();
    printf("Open and parse input file: %lf s\n", (double)(stopInFileTime - startInFileTime) / CLOCKS_PER_SEC);

    clock_t startNaiveSortTime = clock();
    BinsCollection bins;
    bins.count = 0;
    bins.array = malloc(elementsData.count * sizeof(Bin));
    naiveSort(&elementsData, &bins);
    clock_t stopNaiveSortTime = clock();
    printf("Naive sorting: %lf s\n", (double)(stopNaiveSortTime - startNaiveSortTime) / CLOCKS_PER_SEC);

#ifdef SAVE_OUTPUT_FILE
    clock_t startOutFileTime = clock();
    OutputData outData;
    processOutput(&elementsData, &bins, &outData);
    saveOutputFile(inFpath, &outData);
    clock_t stopOutFileTime = clock();
    printf("Save the output file: %lf s\n", (double)(stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC);
#endif  // SAVE_OUTPUT_FILE

    // Print results ////////////////////////////////////////////////////////////////////////////////////////
    double readFileTime = (double) (stopInFileTime - startInFileTime) / CLOCKS_PER_SEC;
    double computeTime = (double) (stopNaiveSortTime - startNaiveSortTime) / CLOCKS_PER_SEC;
    double writeFileTime = (double) (stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC;

    printf("\n");
    printf("Number of bins: %d\n", outData.validBinCount);
    printf("Total time: %lf s\n", readFileTime + computeTime + writeFileTime);
    printf("------------------------------------------------------------\n");

    // Free /////////////////////////////////////////////////////////////////////////////////////////////////
    free(outData.sortedElementIdxs);
    free(outData.binOffsets);
    free(bins.array);
    free(elementsData.binIdxs);
    free(elementsData.array);
}

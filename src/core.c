
// SYSTEM LIBS //////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// IMPORTED LIBS //////////////////////////////////////////////////////////////////////////
#include "mathutils.h"
#include "collectionutils.h"
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

#ifdef DEBUG_TRACE
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*\n");
    printf("Element %d (value: %lf) -> Bin %d\n", elementIdx + 1, elementsData->array[elementIdx], binIdx);
    printf("Bin %d: sum = %lf, elementCount = %d\n", binIdx, bins->array[binIdx].sum, bins->array[binIdx].elementCount);
#endif
}

Bin* initBin(BinsCollection* bins, int binIdx)
{
    bins->array[binIdx].idx = binIdx;
    bins->array[binIdx].sum = 0.0;
    bins->array[binIdx].elementCount = 0;

    int newCount = binIdx + 1;
    bins->count = max(newCount, bins->count);

#ifdef DEBUG_TRACE
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*\n");
    printf("Init bin %d\n", binIdx);
#endif

    return &(bins->array[binIdx]);
}

void naiveAlg(Elements* elementsData, BinsCollection* bins)
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
}

void thresholdAlg(Elements* elementsData, BinsCollection* bins)
{
    LinkedList* openBins = linkedListCreate();
    int currentBinIdx = 0;
    linkedListPushBack(openBins, initBin(bins, 0));

    const double MAX_OVERFLOW = 1.4;

    // insert first element to first bin
    insertElement(elementsData, bins, 0, 0);

    for (int elIdx = 1; elIdx < elementsData->count; elIdx++)
    {
        double item = elementsData->array[elIdx];

        // calculate dynamic threshold
        double progress = (double)elIdx / elementsData->count;
        double currentThreshold = MAX_OVERFLOW * progress;

        // find best fit
        Node* bestBinNode = NULL;
        double minOverflow = 1.0;

        LINKEDLIST_FOREACH(binNode, openBins)
        {
            Bin* b = (Bin*)binNode->data;
            double newSum = b->sum + item;
            if (newSum < 1.0)
            {
                // insert here
                bestBinNode = binNode;
                break;
            }

            double overflow = newSum - 1.0;
            if (overflow < minOverflow && overflow <= currentThreshold)
            {
                minOverflow = overflow;
                bestBinNode = binNode;
            }
        }

        // insert element to best fit bin or init new
        if (bestBinNode != NULL)
        {
            // insert to best fit bin
            Bin* b = (Bin*)bestBinNode->data;
            insertElement(elementsData, bins, b->idx, elIdx);
            if (b->sum >= 1.0)
            {
                likedListRemoveNode(openBins, bestBinNode, false);
            }
        }
        else
        {
            // init new bin
            Bin* newBin = initBin(bins, ++currentBinIdx);
            insertElement(elementsData, bins, newBin->idx, elIdx);
            linkedListPushBack(openBins, newBin);
        }
    }

    linkedlistDestroy(openBins, false);
}

void printResults(char* algorithmName, OutputData outData, double computeTime, double readFileTime, double writeFileTime)
{
    printf("------------------------------------------------------------\n");
    printf("Algorithm: %s\n", algorithmName);
    printf("Open and parse input file: %lf s\n", readFileTime);
    printf("Solving algorithm: %lf s\n", computeTime);
    printf("Process and save the output file: %lf s\n", writeFileTime);
    printf("Number of valid bins: %d\n", outData.validBinCount);
    printf("Total time: %lf s\n", readFileTime + computeTime + writeFileTime);
    printf("------------------------------------------------------------\n");
    printf("\n");
}

void mainAlgorithm(char* inFpath)
{
    printf("------------------------------------------------------------\n");
    printf("File path %s\n", inFpath);

    clock_t startInFileTime = clock();
    Elements elementsData;
    readInputFile(inFpath, &elementsData);
    clock_t stopInFileTime = clock();

    clock_t startSolverTime = clock();
    BinsCollection bins;
    bins.count = 0;
    bins.array = malloc(elementsData.count * sizeof(Bin));  // worst case allocate

#if defined(NAIVE_ALG)
    char* algorithmName = "Naive";
    naiveAlg(&elementsData, &bins);
#elif defined(THRESHOLD_ALG)
    char* algorithmName = "Threshold";
    thresholdAlg(&elementsData, &bins);
#else
    printf("Please define an algorithm in user defines.");
    exit(EXIT_FAILURE)
#endif  // *_ALG

    clock_t stopSolverTime = clock();
    double computeTime = (double) (stopSolverTime - startSolverTime) / CLOCKS_PER_SEC;

    clock_t startOutFileTime = clock();
    OutputData outData;
    processOutput(&elementsData, &bins, &outData);

#ifdef SAVE_OUTPUT_FILE
    saveOutputFile(inFpath, &outData);
#endif  // SAVE_OUTPUT_FILE

    clock_t stopOutFileTime = clock();

    double readFileTime = (double) (stopInFileTime - startInFileTime) / CLOCKS_PER_SEC;
    double writeFileTime = (double) (stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC;
    printResults(algorithmName, outData, computeTime, readFileTime, writeFileTime);

    // Free /////////////////////////////////////////////////////////////////////////////////////////////////
    free(outData.sortedElementIdxs);
    free(outData.binOffsets);
    free(bins.array);
    free(elementsData.binIdxs);
    free(elementsData.array);
}

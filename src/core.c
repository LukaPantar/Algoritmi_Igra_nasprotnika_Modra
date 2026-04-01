
// SYSTEM LIBS //////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// IMPORTED LIBS //////////////////////////////////////////////////////////////////////////
#include "core.h"

Elements* readInputFile(char* inFpath)
{
    FILE* fPtr;
    fPtr = fopen(inFpath, "r");
    if (fPtr == NULL)
    {
        printf("The input file is not exist!");
        exit(EXIT_FAILURE);
    }

    Elements* elements = malloc(sizeof(Elements));

    fscanf(fPtr, "%d", &(elements->len));
    elements->el = malloc(elements->len * sizeof(double));
    for (int i = 0; i < elements->len; i++)
    {
        fscanf(fPtr, "%lf", &(elements->el[i]));
    }
    fclose(fPtr);

    return elements;
}

void saveOutputFile(char* inFpath, Bins* bins)
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

    int filteredLen = 0;
    for (int i = 0; i < bins->len; i++)
        if (bins->b[i].size >= 1)
            filteredLen++;
    fprintf(fPtr, "%d\n", filteredLen);
    
    for (int i = 0; i < bins->len; i++) 
    {
        if (bins->b[i].size >= 1)
        {
            for (int j = 0; j < bins->b[i].lenEl; j++)
            {
                fprintf(fPtr, "%d ", (bins->b[i].elIdx[j] + 1));
            }
            fprintf(fPtr, "\n");
        }
    }
    fclose(fPtr);
}

/**
 * @brief Naive implementaiton of sort. If the previous bin is not filled, the next element is added.
 * 
 */
Bins* naiveSort(Elements* elements)
{
    Bins* bins = malloc(sizeof(Bins));
    bins->b = malloc(sizeof(Bin) * elements->len); // TODO currently the allocated size is maximum possible
    bins->len = 0;

    for (int i = 0; i < elements->len; i++)
    {
        if (bins->len == 0 || bins->b[bins->len - 1].size >= 1)
        {
            bins->b[bins->len].size = 0;
            bins->b[bins->len].lenEl = 0;
            bins->b[bins->len].elIdx = malloc(sizeof(int) * elements->len); // TODO currently the allocated size is maximum possible
            bins->len++;
        }

        bins->b[bins->len - 1].size += elements->el[i];
        bins->b[bins->len - 1].elIdx[bins->b[bins->len - 1].lenEl] = i;
        bins->b[bins->len - 1].lenEl++;
    }

    return bins;
}

void mainAlgorithm(char* inFpath)
{
    clock_t startInFileTime = clock();
    Elements* elements = readInputFile(inFpath);
    clock_t stopInFileTime = clock();
    printf("Open and parse input file: %lf s\n", (double)(stopInFileTime - startInFileTime) / CLOCKS_PER_SEC);

    clock_t startNaiveSortTime = clock();
    Bins* bins = naiveSort(elements);
    clock_t stopNaiveSortTime = clock();
    printf("Naive sorting: %lf s\n", (double)(stopNaiveSortTime - startNaiveSortTime) / CLOCKS_PER_SEC);

#ifdef SAVE_OUTPUT_FILE
    clock_t startOutFileTime = clock();
    saveOutputFile(inFpath, bins);
    clock_t stopOutFileTime = clock();
    printf("Save the output file: %lf s\n", (double)(stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC);
#endif  // SAVE_OUTPUT_FILE

    // Print results ////////////////////////////////////////////////////////////////////////////////////////

    int filteredLen = 0;
    double totalOverhead = 0;
    double maxOverhead = 0;
    double totalUnused = 0;
    double maxUnused = 0;
    for (int i = 0; i < bins->len; i++)
    {
        double binSize = bins->b[i].size;
        if (binSize >= 1)
        {
            filteredLen++;
            totalOverhead += binSize - 1;
            if (binSize - 1 > maxOverhead)
                maxOverhead = binSize - 1;
        }
        else
        {
            totalUnused += binSize;
            if (binSize > maxUnused)
                maxUnused = binSize;
        }
    }

    printf("\n");
    printf("Number of bins: %d [Full %d] [Not full %d]\n", bins->len, filteredLen, bins->len - filteredLen);
    printf("Overhead: %lf [Average %lf] [Max %lf]\n", totalOverhead, totalOverhead / filteredLen, maxOverhead);
    if (bins->len - filteredLen)
        printf("Unused: %lf [Average %lf] [Max %lf]\n", totalUnused, totalUnused / (bins->len - filteredLen), maxUnused);
    printf("Total time: %lf s\n", (double) (stopInFileTime - startInFileTime) / CLOCKS_PER_SEC + 
                                  (double) (stopNaiveSortTime - startNaiveSortTime) / CLOCKS_PER_SEC + 
                                  (double) (stopOutFileTime - startOutFileTime) / CLOCKS_PER_SEC);

    // Free /////////////////////////////////////////////////////////////////////////////////////////////////
    free(elements->el);
    for (int i = 0; i < bins->len; i++)
    {
        free(bins->b[i].elIdx);
    }
    free(elements);
    free(bins->b);
    free(bins);
}

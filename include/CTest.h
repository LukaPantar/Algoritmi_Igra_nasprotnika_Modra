#ifndef CTEST_H
#define CTEST_H

// SYSTEM LIBS //////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// MACROS ////////////////////////////////////////////////////////////////////////////////
#define OUT_FEXT ".out"
#define TXT_FEXT ".txt"
#define TESTS_FPATH "../tests/"

#define MAX_FPATH_LEN 100

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////
/**
 * @brief enum structure that holds the test state
 * 
 */
typedef enum {
    IDLE,       // 0 status code
    RUNNING,    // 1 status code
    PASSED,     // 2 status code
    FAILED      // 3 status code
} Status;

/**
 * @brief Structure for a Test object
 * 
 */
typedef struct {
    char* id;               // ID for test
    char* description;      // User provided description
    Status testStatus;      // Status for pass/fail 0 - 3
    char* statusMessage;    // Message to display on error
    double timeTaken;       // Runtime of test
    clock_t start;          // Start of runtime
    clock_t end;            // End of runtime
    bool skipped;           // Check if test is to be skipped at runtime
} Test;

/**
 * @brief Structure for CTest object
 * 
 */
typedef struct {
    Test tests[50];         // Limit of 50 tests per parent, might change later
    int elements;           // Counter of elements
    double timeTaken;       // Runtime of test
    clock_t start;          // Start of runtime
    clock_t end;            // End of runtime
} CTest;

/**
 * @brief Create a CTest object.
 * 
 * @return CTest*  
 */
CTest* createCTest() {
    CTest *test = malloc(sizeof(CTest));
    test->elements = 0;
    test->start = clock();
    test->timeTaken = 0.0;
    return test;
}

/**
 * @brief Create a Test object.
 * 
 * @param description   A description of the test object.
 * @param skip          Boolean to check if the test is skipped. 
 * @return Test* 
 */
Test* createTest(const char* description, bool skip) {

    Test* test = malloc(sizeof(Test));          
    if (!test) {                       
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    test->id = strdup("CTEST_UNIT");            // Hardcoded ID for tests - may be useful later
    test->testStatus = IDLE;                    

    test->description = strdup(description);     
    test->statusMessage = strdup("");           
    test->timeTaken = 0.0;                      
    test->skipped = skip;                       

    return test;
}

/**
 * @brief Prints green text to the terminal, reserved for pass
 * 
 * @param text  Test description text
 */
void printGreen(const char* text) {
    printf("\033[0;32m%s\033[0m", text);
}

/**
 * @brief Prints red text to the terminal, reserved for errors
 * 
 * @param text  Text description text
 */
void printRed(const char* text) {
    printf("\033[0;31m%s\033[0m", text);
}

/**
 * @brief Print yellow text to the terminal, reserved for skipping
 * 
 * @param text  Text description text
 */
void printYellow(const char* text) {
    printf("\033[0;33m%s\033[0m", text);
}

/**
 * @brief Function that starts a test, begins the runtime clock and returns it
 * 
 * @param test          Global CTest object
 * @param description   Description of the test
 * @param skip          Bool for test skip
 * @return Test* 
 */
Test* startTest(CTest* test, const char* description, bool skip) {

    if (test->elements >= 50) return NULL; 
    Test *newTest = createTest(description, skip);
    newTest->start = clock();    
    return newTest;
}

/**
 * @brief Function that ends the test, calculates the runtime and adds the test to 
 *        the global CTest object
 * 
 * @param test      The global CTest object
 * @param curTest   The current test being executed
 */
void endTest(CTest* test, Test* curTest) {
    
    curTest->end = clock();
    curTest->timeTaken = ((double) (curTest->end - curTest->start)) / CLOCKS_PER_SEC;
    test->tests[test->elements] = (*curTest);
    test->elements++;
}

/**
 * @brief Helper function to check the status of the test
 * 
 * @param test 
 */
void checkStatus(Test* test) {
    
    if (test->skipped == true) printYellow("SKIP");
    else {
        if (test->testStatus == 2) printGreen("PASS");
        else if (test->testStatus == 3) printRed("FAIL");
        else printf("IDLE"); 
    }
}

/**
 * @brief Function that prints the current test result
 * 
 * @param test  Current test object
 */
void testResult(Test* test) {
    
    printf("%-40s %-40s\t", test->description, test->statusMessage);
    checkStatus(test);
}

/**
 * @brief Function to display all tests that have been run within global CTest object, with
 *        a status of FAILED or PASSED depending on the check for the test status, if skip
 *        then the test will not effect the final score
 * 
 * @param test  Global CTest object
 */
void displayTests(CTest* test) {
    
    int failures = 0;
    double totalTime;
    for (int i = 0; i < test->elements; i++) totalTime += test->tests[i].timeTaken;

    printf("\n"); 

    for (int i = 0; i < test->elements; i++) {

        testResult(&test->tests[i]);
        printf("\n");

        if (test->tests[i].skipped == true) continue;
        else if (test->tests[i].testStatus == 3) failures++;
    }
    
    printf("\n--------------------------------------------------------------------------------------------\n");
    printf("Ran %d tests in %fs\n", test->elements, test->timeTaken);
    
    // Check failures to either pass or fail parent test
    if (failures > 0) printRed("\nTEST FAILED\n\n");
    else  printGreen("\nTEST PASSED\n\n");
}

/**
 * @brief Helper function to free the memory for the Test objects
 * 
 * @param test  Global CTest object
 */
void freeTest(CTest* test) {
    for (int i = 0; i < test->elements; i++) {
        free(test->tests[i].description);
    }

    free(test);
}

/**
 * @brief Function to 'run' the Test, records the runtime, displays tests and frees memory
 * 
 * @param test  Global CTest object 
 */
void run(CTest* test) {
    test->end = clock();
    test->timeTaken = ((double) (test->end - test->start)) / CLOCKS_PER_SEC;
    displayTests(test);
    freeTest(test);
}

/**
 * @brief Assert bins, checks if they are at least of size 1
 * 
 * @param test          Global CTest object
 * @param numElements   Number of input elements
 * @param elements      Array of input elements
 * @param numBins       Number of bins
 * @param numBinsEl     Number of elemensts in bins
 * @param bins          Array of bins
 * @param description   Test description 
 * @param skip          Bool for test skip
 */
void assertBinSizes(CTest* test, int numElements, double elements[], int numBins, int numBinsEl[], int** bins, const char* description, bool skip)
{
    Test *newTest = startTest(test, description, skip);
    if (!newTest) return;

    // Check if all bins are full
    newTest->testStatus = PASSED;
    for (int binIdx = 0; binIdx < numBins; binIdx++)
    {
        double size = 0;
        for (int elIdx = 0; elIdx < numBinsEl[binIdx]; elIdx++)
        {
            size += elements[bins[binIdx][elIdx] - 1];
        }

        if (size < 1)
        {
            newTest->testStatus = FAILED;
            asprintf(&(newTest->statusMessage), "AssertionError: bin %d is not full: %f", binIdx, size);
        }
    }
    endTest(test, newTest);
}

/**
 * @brief Assert if there are not duplicated eleements or incorrect with wrong index
 * 
 * @param test          Global CTest object
 * @param numElements   Number of input elements
 * @param numBins       Number of bins
 * @param numBinsEl     Number of elemensts in bins
 * @param bins          Array of bins
 * @param description   Test description 
 * @param skip          Bool for test skip
 */
void assertBinElements(CTest* test, int numElements, int numBins, int numBinsEl[], int** bins, const char* description, bool skip)
{
    Test *newTest = startTest(test, description, skip);
    if (!newTest) return;

    newTest->testStatus = PASSED;

    int seenN[numElements];
    for (int i = 0; i < numElements; i++)
        seenN[i] = 0;

    for (int j = 0; j < numBins; j++)
    {
        for (int k = 0; k < numBinsEl[j]; k++)
        {
            if (bins[j][k] > numElements || bins[j][k] < 1)
            {
                newTest->testStatus = FAILED;
                asprintf(&(newTest->statusMessage), "AssertionError: incorrect element index %d", bins[j][k]);
                endTest(test, newTest);
                return;
            }

            seenN[bins[j][k] - 1]++;
        }
    }

    for (int i = 0; i < numElements; i++)
    {
        if (seenN[i] > 1)
        {
            newTest->testStatus = FAILED;
            asprintf(&(newTest->statusMessage), "AssertionError: index %d is seen %d times", (i + 1), seenN[i]);
            break;
        }
    }

    endTest(test, newTest);
}

#endif

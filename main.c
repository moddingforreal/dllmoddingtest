#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "prototypes.h"
#include "exit_codes.h"

// Set default values here
// Max line length
#define MAX_LINE_LEN 255

// Function to look for in the libs
#define FUNCTION_NAME "addnums"

// Change to TRUE to run in rounds mode
#define TEST_RUN_USE_ROUNDS FALSE

// If defined in rounds mode, how many rounds to run
#define TEST_RUN_ROUNDS 15

// If not defined in rounds mode, how many tests to run
#define TEST_RUN_TESTS 250

#ifdef MODDING_NO_PRINTF
#define printf no_printlog
#endif

#ifdef MODDING_NO_GETCHAR
#define getchar no_getkeypress
#endif

int no_printlog(const char *restrict _Format, ...) {
    // Empty function: no printf
}

int no_getkeypress() {
    // Empty function: no getchar
}

int main(int argc, char ** argv) {
    // Proof that the program has started working
    printf("Hello, World! I'm ready for work!\n");

    // Accurate console logs :D
    #ifdef WINDOWS
    const char * lName = "DLL";
    #else
    const char * lName = "SO";
    #endif

    // Proof that its starting to work
    printf("Reading ./libs.txt ...\n");

    FILE * fp = fopen("./libs.txt", "r");

    // Checks for existance of libs.txt
    if (fp == NULL) {
        printf("Did not find a libs.txt file in the current directory.\nExiting... (Press any key to continue)");
        getchar();
        return ERR_NO_FILE_FOUND;
    }

    printf("Am counting the amount of libaries to load...\n");

    int newLines = 0;
    // Extract characters from file and store in character c
    for (char c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n') {
            // Increment count if this character is newline
            newLines++;
        }
    }

    printf("Will attempt to read %d %ss into the program from the file, press any key to continue...\n", newLines, lName);
    getchar();

    // Create LibHandle array
    LIBHANDLE * libHandles = malloc(newLines * sizeof(LIBHANDLE));
    int * exists = malloc(newLines * sizeof(int)); // If the handle is NULL, this is TRUE

    // Set all exists members to FALSE
    for (int i = 0; i < (newLines * sizeof(int)); i++) {
        exists[i] = FALSE;
    }

    int totalExists = 0; // Cnt of existing libs

    // Not needed
    /*int * containsFunc = calloc(newLines, sizeof(int)); // This will tell if the lib contains the func addnums
    int totalContainFunc = 0; // Will be kept after freeing the libs taken for check if contains func*/
    
    // If any xalloc calls failed (eg not enough memory) the program will terminate instead of SEGFAULT-ing
    if ((libHandles == NULL) || (exists == NULL) /*|| (containsFunc = NULL)*/) {
        printf("Cannot allocate enough memory to proceed. [libHandles = %d B, exists = %d B, containsFunc = %d B] Press any key to terminate the program...\n", 
        (int)(newLines * sizeof(LIBHANDLE)), (int)(newLines, sizeof(int)), (int)(newLines, sizeof(int)));
        getchar();
        return ERR_NOT_ENOUGH_MEMORY;
    }

    int currProcLine = 0;
    int currLinePos = 0;
    char * currLine = malloc(MAX_LINE_LEN * sizeof(char)); // path to lib (incl. filename) cannot be >254

    // If any xalloc calls failed (eg not enough memory) the program will terminate instead of SEGFAULT-ing
    if (currLine == NULL) {
        printf("Cannot allocate enough memory to proceed. [currLine = %d B] Press any key to terminate the program...\n", (int)(MAX_LINE_LEN * sizeof(char)));
        getchar();
        return ERR_NOT_ENOUGH_MEMORY;
    }

    printf("Loading handles from files...\n");

    // Extract characters from file and store in character c
    fseek(fp, 0, 0);
    for (char c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n') {
            // Increment count if this character is newline
            currLine[currLinePos] = (char)0x00; // NULL-terminated string
            printf("[DBG] Loading: \"%s\"\n");
            libHandles[currProcLine] = loadHandle(currLine, FUNCTION_NAME);
            if (libHandles[currProcLine] != NULL) {exists[currProcLine] = TRUE; totalExists++;} 
            else { printf("Cannot load a handle for the %s on line %d of the libs file.\n", lName, currProcLine); }
            currLinePos = 0;
            currProcLine++;
            continue; // Skip to next line
        }
        currLine[currLinePos] = c; // set the character
        currLinePos++; // Prepare for the next round
    }
    free(currLine);
    printf("Loaded %d/%d %ss successfully!\n", totalExists, newLines, lName);

    printf("Attempting to find a function \"%s\" in every %s\n", FUNCTION_NAME, lName);

    // Create function array
    AddFunc * funcArr = malloc(totalExists * sizeof(AddFunc));
    //int len = 0; // Replaced by totalContainsFunc

    if (totalExists == 0) { printf("None of the %s handles exist. \nPress any key to exit the program.\n", lName); getchar(); return ERR_NO_HANDLES; }
    int existingEncountered = 0;
    for (int i = 0; i < newLines; i++) {
        if (libHandles[i] != NULL) {
            funcArr[existingEncountered] = loadFunc(libHandles[i], FUNCTION_NAME);
            existingEncountered++;
        } else {
            printf("%s on line %d of the libs file does not contain the function %s.\n", lName, i, FUNCTION_NAME);
        }
    }
    printf("[DBG] existingEncountered = %d\n", existingEncountered);

    printf("Successfully loaded function pointers to %d of the %d %ss successfully loaded from %d filenames in the libs file! Cleaning up...\n", 
        existingEncountered, totalExists, lName, newLines);

    // Free up unused space
    if (realloc(funcArr, (existingEncountered * sizeof(AddFunc))) == NULL) { printf("Unable to realloc the function pointer array. [funcArr = %d B]\n Press any key to exit.\n", (int)(totalExists * sizeof(AddFunc))); 
        getchar(); return ERR_NOT_ENOUGH_MEMORY; }

    printf("Finished cleaning up! Entering test phase...\n");

    // Tests
    if (TEST_RUN_USE_ROUNDS) {
        int currRes = 0;
        for (int i = 0; i < (existingEncountered * TEST_RUN_ROUNDS); i++) {
            currRes = (*funcArr[(i % existingEncountered)])(i, i+1);
            printf("Result for %dth successfully loaded function: %d\n", (i % existingEncountered), currRes);
        }
    } else {
            int currRes = 0;
        for (int i = 0; i < TEST_RUN_TESTS; i++) {
            currRes = (funcArr[(i % existingEncountered)])(i, i+1);
            printf("Result for %dth successfully loaded function: %d\n", (i % existingEncountered), currRes);
        }
    }

    printf("\n\n\t\tTesting complete. Freeing all handles...\n");

    int notFreed = 0;
    for (int i = 0; i < newLines; i++) {
        if (exists[i]) {
            notFreed += freeLib(libHandles[i]) / 5;
        }
    }
    free(exists);
    free(libHandles);
    free(funcArr);

    if (notFreed != 0) printf("Was unable to unload/free %d %ss.\n", notFreed, lName);

    printf("\n\n\t\tTesting complete. Press any key to close the program...\n");

    getchar();
    return NOERROR;
}

#if defined(WINDOWS)
LIBHANDLE loadHandle(const char * dllname, const char * funcname) {
    HINSTANCE hDLL = LoadLibrary(dllname);
    if (hDLL == NULL) {
        printf("Failed to load DLL %s\n", dllname);
        return NULL;
    }
    return hDLL;
}

AddFunc loadFunc(LIBHANDLE handle, const char * funcname) {
    AddFunc addNumbers = (AddFunc)GetProcAddress(handle, funcname);
    if (addNumbers == NULL) {
        printf("Failed to find function \"%s\" in a lib.\n", funcname);
        return NULL;
    }
}

int freeLib(LIBHANDLE handle) {
    if (FreeLibrary(handle)) return NOERROR; else return ERR_UNABLE_TO_FREE_LIB;
}
#else
LIBHANDLE loadHandle(const char * libname, const char * funcname) {
    void* handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
        printf("Failed to load shared object: %s\n", dlerror());
        return NULL;
    }
    return handle;
}

AddFunc loadFunc(LIBHANDLE handle, const char * funcname) {
    AddFunc addNumbers = (AddFunc)dlsym(handle, funcname);
    if (!addNumbers) {
        printf("Failed to find function: %s\n", dlerror());
        dlclose(handle);
        return NULL;
    }

    return addNumbers;
}

int freeLib(LIBHANDLE handle) {
    if (dlclose(handle)) return NOERROR; else return ERR_UNABLE_TO_FREE_LIB;
}
#endif

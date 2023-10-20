#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "prototypes_windows.h"



int main(int argc, char ** argv) {
    printf("Hello, World!\n");

    AddFunc * funcArr = malloc(3 * sizeof(AddFunc));
    int len = 0;

    funcArr[len] = loadFunc("./libs/dll_one.dll", "addnums");
    if (funcArr[len] != NULL) {len++;}
    funcArr[len] = loadFunc("./libs/dll_two.dll", "addnums");
    if (funcArr[len] != NULL) {len++;}
    funcArr[len] = loadFunc("./libs/dll_three.dll", "addnums");
    if (funcArr[len] != NULL) {len++;}

    printf("Loaded %d/3 functions successfully for testing!\nTesting functions...\n", len);

    int currRes = 0;
    int currFunc = 0;
    for (int i = 0; i < 100; i++) {
        currRes = (*funcArr[currFunc])(i, i+1);
        printf("Result for dll-%d's function: %d\n", currFunc, currRes);
        if (currFunc == (len-1)) {currFunc = 0;} else {currFunc++;}
    }

    printf("\n\nTesting complete\n");

    return 0;
}

AddFunc loadFunc(const char * dllname, const char * funcname) {
    HINSTANCE hDLL = LoadLibrary(dllname);
    if (hDLL == NULL) {
        printf("Failed to load DLL %s\n", dllname);
        return NULL;
    }

    AddFunc addNumbers = (AddFunc)GetProcAddress(hDLL, funcname);
    if (addNumbers == NULL) {
        printf("Failed to find function in %s\n", dllname);
        return NULL;
    }
}

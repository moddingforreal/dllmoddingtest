#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "prototypes_linux.h"

int main(int argc, char ** argv) {
    printf("Hello, World!\n");

    AddFunc * funcArr = malloc(3 * sizeof(AddFunc));
    int len = 0;

    funcArr[len] = loadFunc("./so_one.so", "addnums");
    if (funcArr[len] != NULL) {len++;}
    funcArr[len] = loadFunc("./so_two.so", "addnums");
    if (funcArr[len] != NULL) {len++;}
    funcArr[len] = loadFunc("./so_three.so", "addnums");
    if (funcArr[len] != NULL) {len++;}

    printf("Loaded %d/3 functions successfully for testing!\nTesting functions...\n", len);

    int currRes = 0;
    int currFunc = 0;
    for (int i = 0; i < 100; i++) {
        currRes = (*funcArr[currFunc])(i, i+1);
        printf("Result for so-%d's function: %d\n", currFunc, currRes);
        if (currFunc == (len-1)) {currFunc = 0;} else {currFunc++;}
    }

    printf("\n\nTesting complete\n");

    return 0;
}

AddFunc loadFunc(const char * libname, const char * funcname) {
    void* handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
        printf("Failed to load shared object: %s\n", dlerror());
        return NULL;
    }

    AddFunc addNumbers = (AddFunc)dlsym(handle, funcname);
    if (!addNumbers) {
        printf("Failed to find function: %s\n", dlerror());
        dlclose(handle);
        return NULL;
    }

    return addNumbers;
}

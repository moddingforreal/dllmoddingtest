#ifndef PROTOTYPES_H
#define PROTOTYPES_H

typedef int (*AddFunc)(int, int);
typedef struct SizedArray {
    size_t len;
    void * arr;
} SizedArray;

AddFunc loadFunc(const char * dllname, const char * funcname);

#endif
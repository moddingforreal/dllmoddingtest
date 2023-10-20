#ifndef PROTOTYPES_H
#define PROTOTYPES_H

// In this program it's assumed that every system other than windows has the dlfcn.h header
#define WINDOWS (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))

typedef int (*AddFunc)(int, int);
typedef struct SizedArray {
    size_t len;
    void * arr;
} SizedArray;

#ifdef WINDOWS
#include <windows.h>
typedef HINSTANCE LIBHANDLE;
LIBHANDLE loadHandle(const char * dllname, const char * funcname);
#else
#include <dlfcn.h>
typedef void* LIBHANDLE;
LIBHANDLE loadHandle(const char * libname, const char * funcname);
#endif

AddFunc loadFunc(LIBHANDLE handle, const char * funcname);
int freeLib(LIBHANDLE handle);
#endif
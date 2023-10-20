#include <stdio.h>
__declspec(dllexport) int addnums(int a, int b) {
    printf("Called a func in dll three!\n");
    return a + b;
}
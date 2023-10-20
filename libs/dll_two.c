#include <stdio.h>
__declspec(dllexport) int addnums(int a, int b) {
    printf("Called a func in dll two!\n");
    return a + b;
}
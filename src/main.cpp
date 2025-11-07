#include <stdio.h>
#include "include/global.h"

int main() {
    printf("Testing global variables\n");
    printf("Global cpufile: %p\n", cpufile);
    printf("Global MANDIR: %p\n", MANDIR);
    printf("Global tri: %p\n", tri);
    printf("Global pt: %p\n", pt);
    printf("Global nd: %p\n", nd);
    printf("Global reg: %p\n", reg);
    printf("Global edg: %p\n", edg);

    return 0;
}

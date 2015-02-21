#include <sys/types.h>
#include <stdlib.h>

/*
 * Implement C++ new/delete operators using the heap
 */

void *operator new(size_t size) {
    return malloc(size);
}

void *operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void *p) {
    free(p);
}

void operator delete[](void *p) {
    free(p);
}
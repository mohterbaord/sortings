#ifndef COLLECTIONS_INT_ARRAY_DATA_H
#define COLLECTIONS_INT_ARRAY_DATA_H

#include <stddef.h>

typedef struct IntArray {

    int* _elems;

    size_t _size;

    size_t _capacity;

} IntArray;

#endif

#ifndef COLLECTIONS_INT_ARRAY_SORTINGS_H
#define COLLECTIONS_INT_ARRAY_SORTINGS_H

#include "collections/int_array/api.h"

typedef struct IntArraySortings {

    const SortIntArray BUBBLE_SORT;

    const SortIntArray SHAKER_SORT;

    const SortIntArray COMB_SORT;

    const SortIntArray INSERTION_SORT;

    const SortIntArray SHELL_SORT;

} IntArraySortings;

IntArraySortings* int_array_sortings();

#endif

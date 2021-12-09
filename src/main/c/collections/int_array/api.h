#ifndef COLLECTIONS_INT_ARRAY_API_H
#define COLLECTIONS_INT_ARRAY_API_H

#include <stdbool.h>
#include <stddef.h>

#include "collections/int_array/data.h"

typedef char* CharSequence;

typedef int (*CompareInts)(int left, int right);
typedef void (*SortIntArray)(IntArray* self);
typedef int (*MapInt)(int value);
typedef bool (*FilterInt)(int value);
typedef int (*FoldInts)(int left, int right);

typedef struct IntArrayApi {

    bool _is_initialized;

    CompareInts _compare_ints;

    bool _compare_reversed;

    SortIntArray _sort_int_array;

    IntArray* (*init)();

    void (*del)(IntArray* self);

    CharSequence (*str)(IntArray* self);

    void (*append)(IntArray* self, int value);

    void (*append_all)(IntArray* self, int values[], size_t n);

    void (*append_all_from)(IntArray* self, IntArray* other);

    void (*sort)(IntArray* self);

    int (*fold)(IntArray* self, int start_value, FoldInts fold);

    IntArray* (*copied)(IntArray* self);

    IntArray* (*reversed)(IntArray* self);

    IntArray* (*merged)(IntArray* self, IntArray* other);

    IntArray* (*sorted)(IntArray* self);

    IntArray* (*mapped)(IntArray* self, MapInt map);

    IntArray* (*filtered)(IntArray* self, FilterInt filter);

    void (*_swap)(IntArray* self, int i, int j);

} IntArrayApi;

IntArrayApi* int_array_api();

#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "collections/int_array/sortings/sortings.h"
#include "macro/qualifiers.h"

#include "collections/int_array/__str_impl.h"

#define DEFAULT_SIZE 0
#define MIN_CAPACITY 64
#define DEFAULT_CAPACITY MIN_CAPACITY
#define CAPACITY_INCREASE_FACTOR 1.5

__pure__ static int fold(IntArray* self, int start_value, FoldInts fold);
static void append(IntArray* self, int value);
static void append_all(IntArray* self, int values[], size_t n);

__pure__ static IntArray __construct(int* elems, size_t size, size_t capacity);
__pure__ static IntArray __prepare_new_data(IntArray* self, size_t new_capacity);
__pure__ static size_t __limit_capacity(size_t capacity);
__pure__ static size_t __limit_size_with_capacity(size_t size, size_t capacity);
__pure__ static int __compare_ints(int left, int right);
__pure__ static bool __should_swap(IntArray* self, int left_index, int right_index);
__pure__ static int __compare(int left, int right);
static void __set_fields_from(IntArray* self, IntArray data);
static void __realloc_elems(IntArray* self, size_t new_capacity);
static void __swap(IntArray* self, int i, int j);

//

__pure__ static IntArray* init() {
    IntArray* int_array = malloc(sizeof(IntArray));
    __set_fields_from(int_array, __construct(calloc(MIN_CAPACITY, sizeof(int)), DEFAULT_SIZE, DEFAULT_CAPACITY));
    return int_array;
}

__pure__ static IntArray* init_from(int values[], size_t n) {
    IntArray* int_array = malloc(sizeof(IntArray));
    size_t capacity = __limit_capacity(n);
    __set_fields_from(int_array, __construct(calloc(capacity, sizeof(int)), DEFAULT_SIZE, capacity));
    append_all(int_array, values, n);
    return int_array;
}

static void del(IntArray* self) {
    free(self->_elems);
    free(self);
}

static void append(IntArray* self, int value) {
    if (self->_size == self->_capacity) {
        __realloc_elems(self, (size_t) (CAPACITY_INCREASE_FACTOR * self->_capacity));
    }
    self->_elems[self->_size++] = value;
}

static void append_all(IntArray* self, int values[], size_t n) {
    size_t new_size = self->_size + n;
    if (new_size >= self->_capacity) {
        __realloc_elems(self, (size_t) (CAPACITY_INCREASE_FACTOR * new_size));
    }
    memcpy(self->_elems + self->_size, values, sizeof(int) * n);
    self->_size = new_size;
}

static void append_all_from(IntArray* self, IntArray* other) {
    append_all(self, other->_elems, other->_size);
}

static void sort(IntArray* self) {
    int_array_api()->_sort_int_array(self);
}

__pure__ static int fold(IntArray* self, int start_value, FoldInts fold) {
    int accumulator = start_value;
    for (int i = 0; i < self->_size; ++i) {
        accumulator = fold(accumulator, self->_elems[i]);
    }
    return accumulator;
}

__pure__ static IntArray* copied(IntArray* self) {
    IntArray* copy = init();
    append_all_from(copy, self);
    return copy;
}

__pure__ static IntArray* reversed(IntArray* self) {
    IntArray* result = copied(self);
    int half_size = result->_size / 2;
    int last_elem_index = result->_size - 1;
    for (int i = 0; i < half_size; ++i) {
        __swap(result, i, last_elem_index - i);
    }
    return result;
}

__pure__ static IntArray* merged(IntArray* self, IntArray* other) {
    IntArray* result = copied(self);
    append_all_from(result, other);
    return result;
}

__pure__ static IntArray* mapped(IntArray* self, MapInt map) {
    IntArray* result = copied(self);
    for (int i = 0; i < result->_size; ++i) {
        result->_elems[i] = map(result->_elems[i]);
    }
    return result;
}

__pure__ static IntArray* sorted(IntArray* self) {
    IntArray* result = copied(self);
    sort(result);
    return result;
}

__pure__ static IntArray* filtered(IntArray* self, FilterInt filter) {
    IntArray* result = init();
    for (int i = 0; i < self->_size; ++i) {
        if (filter(self->_elems[i])) {
            append(result, self->_elems[i]);
        }
    }
    return result;
}

static void __set_fields_from(IntArray* self, IntArray data) {
    self->_elems = data._elems;
    self->_size = data._size;
    self->_capacity = data._capacity;
}

static void __realloc_elems(IntArray* self, size_t new_capacity) {
    IntArray new_data = __prepare_new_data(self, new_capacity);
    memcpy(new_data._elems, self->_elems, sizeof(int) * new_data._size);
    free(self->_elems);
    __set_fields_from(self, new_data);
}

__pure__ static IntArray __prepare_new_data(IntArray* self, size_t new_capacity) {
    size_t new_capacity_limited = __limit_capacity(new_capacity);
    return __construct(
        calloc(new_capacity_limited, sizeof(int)),
        __limit_size_with_capacity(self->_size, new_capacity_limited),
        new_capacity_limited
    );
}

__pure__ static size_t __limit_capacity(size_t capacity) {
    return (capacity < MIN_CAPACITY) ? MIN_CAPACITY : capacity;
}

__pure__ static size_t __limit_size_with_capacity(size_t size, size_t capacity) {
    return (capacity < size) ? capacity : size;
}

__pure__ static IntArray __construct(int* elems, size_t size, size_t capacity) {
    IntArray int_array = {
        ._elems=elems,
        ._size=size,
        ._capacity=capacity,
    };
    return int_array;
}

__pure__ static int __compare_ints(int left, int right) {
    return (left < right) ? -1 : ((left > right) ? 1 : 0);
}

static bool __decide_swap(IntArray* self, int left_index, int right_index) {
    return __should_swap(self, left_index, right_index) && (__swap(self, left_index, right_index), true);
}

__pure__ static bool __should_swap(IntArray* self, int left_index, int right_index) {
    return __compare(self->_elems[left_index], self->_elems[right_index]) > 0;
}

__pure__ static int __compare(int left, int right) {
    int result = __compare_ints(left, right);
    return !int_array_api()->_compare_reversed ? result : -result;
}

static void __swap(IntArray* self, int i, int j) {
    int tmp = self->_elems[i];
    self->_elems[i] = self->_elems[j];
    self->_elems[j] = tmp;
}

IntArrayApi* int_array_api() {
    static IntArrayApi instance = { ._is_initialized=false };
    if (!instance._is_initialized) {
        IntArrayApi new_instance = {
            __export__(init),
            __export__(init_from),
            __export__(del),
            __export__(str),
            __export__(append),
            __export__(append_all),
            __export__(append_all_from),
            __export__(sort),
            __export__(fold),
            __export__(copied),
            __export__(reversed),
            __export__(merged),
            __export__(mapped),
            __export__(sorted),
            __export__(filtered),
            __export__(__decide_swap),
            __export__(__compare),
            ._compare_ints=__compare_ints,
            ._compare_reversed=false,
            ._sort_int_array=int_array_sortings()->SHELL_SORT,
            ._is_initialized=true,
        };
        instance = new_instance;
    }
    return &instance;
}

#undef DEFAULT_SIZE
#undef MIN_CAPACITY
#undef DEFAULT_CAPACITY
#undef CAPACITY_INCREASE_FACTOR

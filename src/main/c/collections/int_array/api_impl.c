#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "collections/int_array/sortings/sortings.h"
#include "macro/qualifiers.h"

#define DEFAULT_SIZE 0
#define MIN_CAPACITY 64
#define DEFAULT_CAPACITY MIN_CAPACITY
#define CAPACITY_INCREASE_FACTOR 1.5

struct __CharSequenceAndLength {
    const CharSequence char_sequence;
    const int length;
};

struct __IntArrayStrFormat {
    const struct __CharSequenceAndLength typename;
    const struct __CharSequenceAndLength separator_1;
    const struct __CharSequenceAndLength separator_2;
    const struct __CharSequenceAndLength separator_3;
    IntArray* _int_array;
    int _size_length;
    IntArray* _lengths;
    int _result_str_length;
};

__pure__ static int fold(IntArray* self, int start_value, FoldInts fold);
static void append(IntArray* self, int value);

__pure__ static IntArray _construct(int* elems, size_t size, size_t capacity);
__pure__ static IntArray _prepare_new_data(IntArray* self, size_t new_capacity);
__pure__ static size_t _limit_capacity(size_t capacity);
__pure__ static size_t _limit_size_with_capacity(size_t size, size_t capacity);
__pure__ static int _compare(int left, int right);
__pure__ static int _compare_ints(int left, int right);
static void _set_fields_from(IntArray* self, IntArray data);
static void _realloc_elems(IntArray* self, size_t new_capacity);
static void _swap(IntArray* self, int i, int j);

__pure__ static IntArray* init() {
    IntArray* int_array = malloc(sizeof(IntArray));
    _set_fields_from(int_array, _construct(calloc(MIN_CAPACITY, sizeof(int)), DEFAULT_SIZE, DEFAULT_CAPACITY));
    return int_array;
}

static void del(IntArray* self) {
    free(self->_elems);
    free(self);
}

__pure__ static int _sum(int i1, int i2) {
    return i1 + i2;
}

static void _strcat_ulong(CharSequence dst, unsigned long value, int value_lenght) {
    CharSequence tmp = calloc(value_lenght + 1, sizeof(char));
    snprintf(tmp, value_lenght + 1, "%zu", value);
    strncat(dst, tmp, value_lenght + 1);
    free(tmp);
}

static void _strcat_int(CharSequence dst, int value, int value_length, struct __CharSequenceAndLength postfix) {
    int postfixed_value_length = value_length + postfix.length;
    CharSequence tmp = calloc(postfixed_value_length + 1, sizeof(char));
    snprintf(tmp, postfixed_value_length + 1, "%d%s", value, postfix.char_sequence);
    strncat(dst, tmp, postfixed_value_length + 1);
    free(tmp);
}

__pure__ static int _count_result_str_length(struct __IntArrayStrFormat format) {
    return format.typename.length +
        format._size_length +
        format.separator_1.length +
        fold(format._lengths, 0, &_sum) +
        (format.separator_2.length * (format._int_array->_size - 1)) +
        format.separator_3.length;
}

__pure__ static struct __CharSequenceAndLength _init_char_sequence_and_length(const CharSequence char_sequence) {
    struct __CharSequenceAndLength char_sequence_and_length = {
        .char_sequence=char_sequence,
        .length=strlen(char_sequence),
    };
    return char_sequence_and_length;
}

static void _setup_format(struct __IntArrayStrFormat* format, IntArray* int_array) {
    format->_int_array = int_array;
    format->_size_length = snprintf(NULL, 0, "%zu", format->_int_array->_size);
    IntArray* lengths = init();
    for (int i = 0; i < format->_int_array->_size; ++i) {
        append(lengths, snprintf(NULL, 0, "%d", format->_int_array->_elems[i]));
    }
    format->_lengths = lengths;
    format->_result_str_length = _count_result_str_length(*format);
}

__pure__ static struct __IntArrayStrFormat _prepare_format(IntArray* int_array) {
    struct __IntArrayStrFormat format = {
        .typename=_init_char_sequence_and_length("IntArray["),
        .separator_1=_init_char_sequence_and_length("]{ "),
        .separator_2=_init_char_sequence_and_length(", "),
        .separator_3=_init_char_sequence_and_length(" }"),
    };
    _setup_format(&format, int_array);
    return format;
}

static void _write_elems_in_result_str(CharSequence result_str, struct __IntArrayStrFormat format) {
    int i = 0;
    for (; i < (format._int_array->_size - 1); ++i) {
        _strcat_int(result_str, format._int_array->_elems[i], format._lengths->_elems[i], format.separator_2);
    }
    _strcat_int(result_str, format._int_array->_elems[i], format._lengths->_elems[i], format.separator_3);
}

__pure__ static CharSequence _concat_result_str_and_free_format(struct __IntArrayStrFormat format) {
    CharSequence result = calloc(format._result_str_length + 1, sizeof(char));
    strncat(result, format.typename.char_sequence, format.typename.length + 1);
    _strcat_ulong(result, format._int_array->_size, format._size_length);
    strncat(result, format.separator_1.char_sequence, format.separator_1.length + 1);
    _write_elems_in_result_str(result, format);
    free(format._lengths);
    return result;
}

__pure__ static CharSequence str(IntArray* self) {
    return _concat_result_str_and_free_format(_prepare_format(self));
}

static void append(IntArray* self, int value) {
    if (self->_size == self->_capacity) {
        _realloc_elems(self, (size_t) (CAPACITY_INCREASE_FACTOR * self->_capacity));
    }
    self->_elems[self->_size++] = value;
}

static void append_all(IntArray* self, int values[], size_t n) {
    size_t new_size = self->_size + n;
    if (new_size >= self->_capacity) {
        _realloc_elems(self, (size_t) (CAPACITY_INCREASE_FACTOR * new_size));
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
        _swap(result, i, last_elem_index - i);
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

static void _swap(IntArray* self, int i, int j) {
    int tmp = self->_elems[i];
    self->_elems[i] = self->_elems[j];
    self->_elems[j] = tmp;
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

static void _set_fields_from(IntArray* self, IntArray data) {
    self->_elems = data._elems;
    self->_size = data._size;
    self->_capacity = data._capacity;
}

static void _realloc_elems(IntArray* self, size_t new_capacity) {
    IntArray new_data = _prepare_new_data(self, new_capacity);
    memcpy(new_data._elems, self->_elems, sizeof(int) * new_data._size);
    free(self->_elems);
    _set_fields_from(self, new_data);
}

__pure__ static IntArray _prepare_new_data(IntArray* self, size_t new_capacity) {
    size_t new_capacity_limited = _limit_capacity(new_capacity);
    return _construct(
        calloc(new_capacity_limited, sizeof(int)),
        _limit_size_with_capacity(self->_size, new_capacity_limited),
        new_capacity_limited
    );
}

__pure__ static size_t _limit_capacity(size_t capacity) {
    return (capacity < MIN_CAPACITY) ? MIN_CAPACITY : capacity;
}

__pure__ static size_t _limit_size_with_capacity(size_t size, size_t capacity) {
    return (capacity < size) ? capacity : size;
}

__pure__ static IntArray _construct(int* elems, size_t size, size_t capacity) {
    IntArray int_array = {
        ._elems=elems,
        ._size=size,
        ._capacity=capacity,
    };
    return int_array;
}

__pure__ static bool _should_swap(IntArray* self, int left_index, int right_index) {
    return _compare(self->_elems[left_index], self->_elems[right_index]) > 0;
}

__pure__ static int _compare(int left, int right) {
    int result = _compare_ints(left, right);
    return !int_array_api()->_compare_reversed ? result : -result; 
}

__pure__ static int _compare_ints(int left, int right) {
    return (left < right) ? -1 : ((left > right) ? 1 : 0);
}

IntArrayApi* int_array_api() {
    static IntArrayApi instance = { ._is_initialized=false };
    if (!instance._is_initialized) {
        instance.init = init;
        instance.del = del;
        instance.str = str;
        instance.append = append;
        instance.append_all = append_all;
        instance.append_all_from = append_all_from;
        instance.sort = sort;
        instance.fold = fold;
        instance.copied = copied;
        instance.reversed = reversed;
        instance.merged = merged;
        instance.mapped = mapped;
        instance.sorted = sorted;
        instance.filtered = filtered;
        instance._compare_ints = _compare_ints;
        instance._compare_reversed = false;
        instance._sort_int_array = int_array_sortings()->SHAKER_SORT;
        instance._swap = _swap;
        instance._should_swap = _should_swap;
        instance._is_initialized = true;
    }
    return &instance;
}

#undef DEFAULT_SIZE
#undef MIN_CAPACITY
#undef DEFAULT_CAPACITY
#undef CAPACITY_INCREASE_FACTOR

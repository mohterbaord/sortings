#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "macro/qualifiers.h"

typedef struct CharSequenceAndLength {
    const CharSequence char_sequence;
    const int length;
} CharSequenceAndLength;

typedef struct IntArrayStrFormat {
    const CharSequenceAndLength typename;
    const CharSequenceAndLength separator_1;
    const CharSequenceAndLength separator_2;
    const CharSequenceAndLength separator_3;
    IntArray* _int_array;
    int _size_length;
    IntArray* _lengths;
    int _result_str_length;
} IntArrayStrFormat;

__pure__ static IntArrayStrFormat __prepare_format(IntArray* int_array);
__pure__ static CharSequenceAndLength __init_char_sequence_and_length(const CharSequence char_sequence);
static void __setup_format(IntArrayStrFormat* format, IntArray* int_array);
__pure__ static int __count_result_str_length(IntArrayStrFormat format);
__pure__ static int __sum(int i1, int i2);
static CharSequence __concat_result_str_and_free_format(IntArrayStrFormat format);
static void __strcat_ulong(CharSequence dst, unsigned long value, int value_lenght);
static void __write_elems_in_result_str(CharSequence result_str, IntArrayStrFormat format);
static void __strcat_int(CharSequence dst, int value, int value_length, CharSequenceAndLength postfix);

//

__pure__ static CharSequence str(IntArray* self) {
    return __concat_result_str_and_free_format(__prepare_format(self));
}

__pure__ static IntArrayStrFormat __prepare_format(IntArray* int_array) {
    IntArrayStrFormat format = {
        .typename=__init_char_sequence_and_length("IntArray["),
        .separator_1=__init_char_sequence_and_length("]{ "),
        .separator_2=__init_char_sequence_and_length(", "),
        .separator_3=__init_char_sequence_and_length(" }"),
    };
    __setup_format(&format, int_array);
    return format;
}

__pure__ static CharSequenceAndLength __init_char_sequence_and_length(const CharSequence char_sequence) {
    CharSequenceAndLength char_sequence_and_length = {
        .char_sequence=char_sequence,
        .length=strlen(char_sequence),
    };
    return char_sequence_and_length;
}

static void __setup_format(IntArrayStrFormat* format, IntArray* int_array) {
    IntArrayApi* api = int_array_api();
    format->_int_array = int_array;
    format->_size_length = snprintf(NULL, 0, "%zu", format->_int_array->_size);
    IntArray* lengths = api->init();
    for (int i = 0; i < format->_int_array->_size; ++i) {
        api->append(lengths, snprintf(NULL, 0, "%d", format->_int_array->_elems[i]));
    }
    format->_lengths = lengths;
    format->_result_str_length = __count_result_str_length(*format);
}

__pure__ static int __count_result_str_length(IntArrayStrFormat format) {
    return format.typename.length +
        format._size_length +
        format.separator_1.length +
        int_array_api()->fold(format._lengths, 0, &__sum) +
        (format.separator_2.length * (format._int_array->_size - 1)) +
        format.separator_3.length;
}

__pure__ static int __sum(int i1, int i2) {
    return i1 + i2;
}

static CharSequence __concat_result_str_and_free_format(IntArrayStrFormat format) {
    CharSequence result = calloc(format._result_str_length + 1, sizeof(char));
    strncat(result, format.typename.char_sequence, format.typename.length + 1);
    __strcat_ulong(result, format._int_array->_size, format._size_length);
    strncat(result, format.separator_1.char_sequence, format.separator_1.length + 1);
    __write_elems_in_result_str(result, format);
    free(format._lengths);
    return result;
}

static void __strcat_ulong(CharSequence dst, unsigned long value, int value_lenght) {
    CharSequence tmp = calloc(value_lenght + 1, sizeof(char));
    snprintf(tmp, value_lenght + 1, "%zu", value);
    strncat(dst, tmp, value_lenght + 1);
    free(tmp);
}

static void __write_elems_in_result_str(CharSequence result_str, IntArrayStrFormat format) {
    int last_elem_index = format._int_array->_size - 1;
    for (int i = 0; i < last_elem_index; ++i) {
        __strcat_int(result_str, format._int_array->_elems[i], format._lengths->_elems[i], format.separator_2);
    }
    __strcat_int(
        result_str,
        format._int_array->_elems[last_elem_index],
        format._lengths->_elems[last_elem_index],
        format.separator_3
    );
}

static void __strcat_int(CharSequence dst, int value, int value_length, CharSequenceAndLength postfix) {
    int postfixed_value_length = value_length + postfix.length;
    CharSequence tmp = calloc(postfixed_value_length + 1, sizeof(char));
    snprintf(tmp, postfixed_value_length + 1, "%d%s", value, postfix.char_sequence);
    strncat(dst, tmp, postfixed_value_length + 1);
    free(tmp);
}

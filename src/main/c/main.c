#include <stdio.h>
#include <stdlib.h>

#include "collections/int_array/api.h"

static void print_array_formatted(const CharSequence title, IntArray* arr) {
    CharSequence arr_str = int_array_api()->str(arr);
    printf("\n%s\n  %s\n", title, arr_str);
    free(arr_str);
}

int main() {
    int elems[] = { 5, 1, 7, 9, 3, 2, 0, 4, 7, 8 };
    IntArray* arr = int_array_api()->init_from(elems, sizeof elems / sizeof *elems);
    print_array_formatted("Original array:", arr);

    //

    IntArray* arr_sorted = int_array_api()->sorted(arr);
    print_array_formatted("Sorted array:", arr_sorted);

    //

    IntArray* arr_sorted_reversed = int_array_api()->reversed(arr_sorted);
    print_array_formatted("Reversed sorted array:", arr_sorted_reversed);

    //

    int_array_api()->del(arr);
    int_array_api()->del(arr_sorted);
    int_array_api()->del(arr_sorted_reversed);

    return 0;
}

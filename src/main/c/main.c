#include <stdio.h>
#include <stdlib.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"

int main() {
    int elems[] = { 5, 1, 7, 9, 3, 2, 0, 4, 7, 8 };
    IntArray* arr = int_array_api()->init();
    int_array_api()->append_all(arr, elems, 10);

    CharSequence arr_str = int_array_api()->str(arr);
    printf("\nOriginal array:\n  %s\n", arr_str);

    IntArray* arr_sorted = int_array_api()->sorted(arr);
    CharSequence arr_sorted_str = int_array_api()->str(arr_sorted);
    printf("\nSorted array:\n  %s\n", arr_sorted_str);

    int_array_api()->del(arr);
    int_array_api()->del(arr_sorted);

    free(arr_str);
    free(arr_sorted_str);

    return 0;
}

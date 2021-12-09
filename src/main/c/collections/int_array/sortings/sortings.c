#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "collections/int_array/sortings/sortings.h"

#include "collections/int_array/sortings/__bubble_sort_impl.h"

IntArraySortings* int_array_sortings() {
    static IntArraySortings sortings = {
        .BUBBLE_SORT=bubble_sort,
    };
    return &sortings;
}

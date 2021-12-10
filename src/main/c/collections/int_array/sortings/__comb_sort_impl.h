#include "collections/int_array/api.h"
#include "collections/int_array/data.h"

#define COMB_SORT_COEFFICIENT 1.2473309

static void bubble_sort(IntArray* self);

static void comb_sort(IntArray* self) {
    for (int gap = self->_size - 1; gap > 1; gap /= COMB_SORT_COEFFICIENT) {
        for (int i = 0; (i + gap) < self->_size; ++i) {
            int_array_api()->__decide_swap(self, i, i + gap);
        }
    }
    bubble_sort(self);
}

#undef COMB_SORT_COEFFICIENT

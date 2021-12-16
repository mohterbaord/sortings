#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "collections/int_array/sortings/sortings.h"

#define COMB_SORT_COEFFICIENT 1.2473309

static void comb_sort(IntArray* self) {
    for (int gap = self->_size - 1; gap > 1; gap /= COMB_SORT_COEFFICIENT) {
        for (int i = 0; (i + gap) < self->_size; ++i) {
            int_array_api()->__decide_swap(self, i, i + gap);
        }
    }
    int_array_sortings()->BUBBLE_SORT(self);
}

#undef COMB_SORT_COEFFICIENT

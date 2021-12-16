//
//    #=+\/\/\/\/-+=#\ 
//    #  SHAKER SORT ||>
//    #=+-~*'^C\~-+=#/
//        l    /
//

#include <stdbool.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "macro/qualifiers.h"

typedef struct ShakerIterator {
    IntArray* _int_array;
    int _left_elems_in_pair;
    int _right_elems_in_pair;
    bool _swapped;
} ShakerIterator;

__pure__ static ShakerIterator __init_shaker(IntArray* int_array);
__pure__ static bool __cocktail_not_done(ShakerIterator shaker_iterator);
__pure__ static ShakerIterator __shake_further(ShakerIterator shaker_iterator);
static bool __shake_with_swaps_folding(ShakerIterator shaker);
static bool __shake_right_with_swaps_folding(ShakerIterator shaker);
static bool __shake_left_with_swaps_folding(ShakerIterator shaker);
static bool __decide_swap(IntArray* int_array, int left_index, int right_index);

//

static void shaker_sort(IntArray* self) {
    for (
        ShakerIterator shaker = __init_shaker(self);
        __cocktail_not_done(shaker);
        shaker = __shake_further(shaker)
    );
}

__pure__ static ShakerIterator __init_shaker(IntArray* int_array) {
    ShakerIterator shaker = {
        ._int_array=int_array,
        ._left_elems_in_pair=(int_array->_size - 1),
        ._right_elems_in_pair=0,
        ._swapped=true,
    };
    return shaker;
}

__pure__ static bool __cocktail_not_done(ShakerIterator shaker) {
    return shaker._swapped && ((shaker._left_elems_in_pair - shaker._right_elems_in_pair) > 0);
}

__pure__ static ShakerIterator __shake_further(ShakerIterator shaker) {
    ShakerIterator next_shaker = {
        ._int_array=shaker._int_array,
        ._left_elems_in_pair=(shaker._left_elems_in_pair - 1),
        ._right_elems_in_pair=(shaker._right_elems_in_pair + 1),
        ._swapped=__shake_with_swaps_folding(shaker),
    };
    return next_shaker;
}

static bool __shake_with_swaps_folding(ShakerIterator shaker) {
    return __shake_right_with_swaps_folding(shaker) && __shake_left_with_swaps_folding(shaker);
}

static bool __shake_right_with_swaps_folding(ShakerIterator shaker) {
    bool swapped = false;
    for (int i = shaker._right_elems_in_pair; i < shaker._left_elems_in_pair; ++i) {
        swapped |= int_array_api()->__decide_swap(shaker._int_array, i, i + 1);
    }
    return swapped;
}

static bool __shake_left_with_swaps_folding(ShakerIterator shaker) {
    bool swapped = false;
    for (int i = shaker._left_elems_in_pair; i > shaker._right_elems_in_pair; --i) {
        swapped |= int_array_api()->__decide_swap(shaker._int_array, i - 1, i);
    }
    return swapped;
}

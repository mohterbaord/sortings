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

struct __ShakerIterator {
    IntArray* _int_array;
    int _left_elems_in_pair;
    int _right_elems_in_pair;
    bool _swapped;
};

__pure__ static struct __ShakerIterator _init_shaker(IntArray* int_array);
__pure__ static bool _cocktail_not_done(struct __ShakerIterator shaker_iterator);
__pure__ static struct __ShakerIterator _shake_further(struct __ShakerIterator shaker_iterator);
static bool _shake_with_swaps_folding(struct __ShakerIterator shaker);
static bool _shake_right_with_swaps_folding(struct __ShakerIterator shaker);
static bool _shake_left_with_swaps_folding(struct __ShakerIterator shaker);
static bool _decide_swap(IntArray* int_array, int left_index, int right_index);

static void shaker_sort(IntArray* self) {
    typedef struct __ShakerIterator Shaker;
    for (Shaker shaker = _init_shaker(self); _cocktail_not_done(shaker); shaker = _shake_further(shaker));
}

__pure__ static struct __ShakerIterator _init_shaker(IntArray* int_array) {
    struct __ShakerIterator shaker = {
        ._int_array=int_array,
        ._left_elems_in_pair=(int_array->_size - 1),
        ._right_elems_in_pair=0,
        ._swapped=true,
    };
    return shaker;
}

__pure__ static bool _cocktail_not_done(struct __ShakerIterator shaker) {
    return shaker._swapped && ((shaker._left_elems_in_pair - shaker._right_elems_in_pair) > 0);
}

__pure__ static struct __ShakerIterator _shake_further(struct __ShakerIterator shaker) {
    struct __ShakerIterator next_shaker = {
        ._int_array=shaker._int_array,
        ._left_elems_in_pair=(shaker._left_elems_in_pair - 1),
        ._right_elems_in_pair=(shaker._right_elems_in_pair + 1),
        ._swapped=_shake_with_swaps_folding(shaker),
    };
    return next_shaker;
}

static bool _shake_with_swaps_folding(struct __ShakerIterator shaker) {
    return _shake_right_with_swaps_folding(shaker) && _shake_left_with_swaps_folding(shaker);
}

static bool _shake_right_with_swaps_folding(struct __ShakerIterator shaker) {
    bool swapped = false;
    for (int i = shaker._right_elems_in_pair; i < shaker._left_elems_in_pair; ++i) {
        swapped |= int_array_api()->__decide_swap(shaker._int_array, i, i + 1);
    }
    return swapped;
}

static bool _shake_left_with_swaps_folding(struct __ShakerIterator shaker) {
    bool swapped = false;
    for (int i = shaker._left_elems_in_pair; i > shaker._right_elems_in_pair; --i) {
        swapped |= int_array_api()->__decide_swap(shaker._int_array, i - 1, i);
    }
    return swapped;
}

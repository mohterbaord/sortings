#include <stdbool.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "macro/qualifiers.h"

typedef struct InsertionSortRun {
    int _left_elem_index_swapping;
    int _right_elem_index_swapping;
    bool _has_swaps;
    int _swapping_index_offset;
} InsertionSortRun;

static void __run_insertion_sort_for_index(IntArray* self, int index_to_swap_from);
__pure__ static InsertionSortRun __init_insertion_sort_run(int right_elem_index_swapping);
__pure__ static bool __has_further_swapping(InsertionSortRun insertion_sort_run);
__pure__ static int __left_elem_index_swapping_with_offset(InsertionSortRun insertion_sort_run);
__pure__ static int __right_elem_index_swapping_with_offset(InsertionSortRun insertion_sort_run);
__pure__ static InsertionSortRun __next_insertion_sort_run(InsertionSortRun insertion_sort_run);
static void __decide_swap_for_run(InsertionSortRun* insertion_sort_run, IntArray* int_array);

//

static void insertion_sort(IntArray* self) {
    const int FIRST_RIGHT_ELEM_INDEX = 1;
    for (
        int right_elem_index = FIRST_RIGHT_ELEM_INDEX;
        right_elem_index < self->_size;
        ++right_elem_index
    ) {
        __run_insertion_sort_for_index(self, right_elem_index);
    }
}

static void __run_insertion_sort_for_index(IntArray* self, int index_to_swap_from) {
    for (
        InsertionSortRun run = __init_insertion_sort_run(index_to_swap_from);
        __has_further_swapping(run);
        run = __next_insertion_sort_run(run)
    ) {
        __decide_swap_for_run(&run, self);
    }
}

__pure__ static struct InsertionSortRun __init_insertion_sort_run(int right_elem_index_swapping) {
    InsertionSortRun insertion_sort_run = {
        ._left_elem_index_swapping=(right_elem_index_swapping - 1),
        ._right_elem_index_swapping=right_elem_index_swapping,
        ._has_swaps=true,
        ._swapping_index_offset=0,
    };
    return insertion_sort_run;
}

__pure__ static bool __has_further_swapping(InsertionSortRun insertion_sort_run) {
    return insertion_sort_run._has_swaps && (__right_elem_index_swapping_with_offset(insertion_sort_run) > 0);
}

__pure__ static int __left_elem_index_swapping_with_offset(InsertionSortRun insertion_sort_run) {
    return insertion_sort_run._left_elem_index_swapping + insertion_sort_run._swapping_index_offset;
}

__pure__ static int __right_elem_index_swapping_with_offset(InsertionSortRun insertion_sort_run) {
    return insertion_sort_run._right_elem_index_swapping + insertion_sort_run._swapping_index_offset;
}

__pure__ static InsertionSortRun __next_insertion_sort_run(InsertionSortRun insertion_sort_run) {
    InsertionSortRun insertion_sort_run_next = {
        ._left_elem_index_swapping=insertion_sort_run._left_elem_index_swapping,
        ._right_elem_index_swapping=insertion_sort_run._right_elem_index_swapping,
        ._has_swaps=insertion_sort_run._has_swaps,
        ._swapping_index_offset=(insertion_sort_run._swapping_index_offset - 1),
    };
    return insertion_sort_run_next;
}

static void __decide_swap_for_run(InsertionSortRun* insertion_sort_run, IntArray* int_array) {
    insertion_sort_run->_has_swaps = int_array_api()->__decide_swap(
        int_array,
        __left_elem_index_swapping_with_offset(*insertion_sort_run),
        __right_elem_index_swapping_with_offset(*insertion_sort_run)
    );
}

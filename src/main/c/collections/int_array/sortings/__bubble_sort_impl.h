#include "collections/int_array/api.h"
#include "collections/int_array/data.h"
#include "macro/qualifiers.h"

__pure__ static bool _should_swap(IntArray* self, int left_index, int right_index);
__pure__ static int _compare(int left, int right);
static bool _raise_bubble(IntArray* self, int left_elem_in_pair_last_index);

//

static void bubble_sort(IntArray* self) {
    for (
        int left_elem_in_pair_last_index = self->_size - 1;
        (left_elem_in_pair_last_index > 0) && _raise_bubble(self, left_elem_in_pair_last_index--);
    );
}

static bool _raise_bubble(IntArray* self, int left_elem_in_pair_last_index) {
    bool bubble_raised = false;
    for (
        int left_index = 0, right_index = 1;
        left_index < left_elem_in_pair_last_index;
        ++left_index, ++right_index
    ) {
        bubble_raised |= (
            _should_swap(self, left_index, right_index) &&
            (int_array_api()->_swap(self, left_index, right_index), true)
        );
    }
    return bubble_raised;
}

__pure__ static bool _should_swap(IntArray* self, int left_index, int right_index) {
    return _compare(self->_elems[left_index], self->_elems[right_index]) > 0;
}

__pure__ static int _compare(int left, int right) {
    IntArrayApi* api = int_array_api();
    int result = api->_compare_ints(left, right);
    return !api->_compare_reversed ? result : -result; 
}

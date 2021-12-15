/*
 * IntArray[10]{ 5, 1, 7, 9, 3, 2, 0, 4, 7, 8 }
 *
 * gap = 9 (size - 1)
 *   i = 9: { (5)  1   7   9   3   2   0   4   7  <8> }
 *
 * gap = 4 (9 / 2)
 *   i = 4: { (5)  1   7   9  <3>  2   0   4   7   8  }
 *   i = 5: {  3  (1)  7   9   5  <2>  0   4   7   8  }
 *   i = 6: {  3   1  (7)  9   5   2  <0>  4   7   8  }
 *   i = 7: {  3   1   0  (9)  5   2   7  <4>  7   8  }
 *   i = 8: {  3   1   0   4  (5)  2   7   9  <7>  8  }
 *   i = 9: {  3   1   0   4   5  (2)  7   9   7  <8> }
 *
 * gap = 2 (4 / 2)
 *   i = 2: { (3)  1  <0>  4   5   2   7   9   7   8  }
 *   i = 3: {  0  (1)  3  <4>  5   2   7   9   7   8  }
 *   i = 4: { (0)  1  (3)  4  <5>  2   7   9   7   8  }
 *   i = 5: {  0  (1)  3  (4)  5  <2>  7   9   7   8  }
 *   i = 6: { (0)  1  (3)  2  (5)  4  <7>  9   7   8  }
 *   i = 7: {  0  (1)  3  (2)  5  (4)  7  <9>  7   8  }
 *   i = 8: { (0)  1  (3)  2  (5)  4  (7)  9  <7>  8  }
 *   i = 9: {  0  (1)  3  (2)  5  (4)  7  (9)  7  <8> }
 *
 * gap = 1 (2 / 2)
 *   i = 1: { (0) <1>  3   2   5   4   7   8   7   9  }
 *   i = 2: { (0) (1) <3>  2   5   4   7   8   7   9  }
 *   i = 3: { (0) (1) (3) <2>  5   4   7   8   7   9  }
 *   i = 4: { (0) (1) (2) (3) <5>  4   7   8   7   9  }
 *   i = 5: { (0) (1) (2) (3) (5) <4>  7   8   7   9  }
 *   i = 6: { (0) (1) (2) (3) (4) (5) <7>  8   7   9  }
 *   i = 7: { (0) (1) (2) (3) (4) (5) (7) <8>  7   9  }
 *   i = 8: { (0) (1) (2) (3) (4) (5) (7) (8) <7>  9  }
 *   i = 9: { (0) (1) (2) (3) (4) (5) (7) (7) (8) <9> }
 *
 * IntArray[10]{ 0, 1, 2, 3, 4, 5, 7, 7, 8, 9 }
 */

#include <stdbool.h>

#include "collections/int_array/api.h"
#include "collections/int_array/data.h"

static void __run_shell_sort_for_gap(IntArray* self, int gap);
static void __shift_last_elem(IntArray* self, int gap, int last_elem_index);
static bool __should_insert_last_elem_before(int last_elem, int in_front_elem);

static void shell_sort(IntArray* self) {
    for (int gap = self->_size - 1; gap > 0; gap /= 2) {
        __run_shell_sort_for_gap(self, gap);
    }
}

static void __run_shell_sort_for_gap(IntArray* self, int gap) {
    for (int last_elem_index = gap; last_elem_index < self->_size; ++last_elem_index) {
        __shift_last_elem(self, gap, last_elem_index);
    }
}

static void __shift_last_elem(IntArray* self, int gap, int last_elem_index) {
    int last_elem = self->_elems[last_elem_index];
    int i, in_front_elem;
    for (
        i = last_elem_index, in_front_elem = self->_elems[i - gap];
        (i >= gap) && __should_insert_last_elem_before(last_elem, in_front_elem);
        i -= gap, in_front_elem = self->_elems[i - gap]
    ) {
        self->_elems[i] = in_front_elem;
    }
    self->_elems[i] = last_elem;
}

static bool __should_insert_last_elem_before(int last_elem, int in_front_elem) {
    return int_array_api()->__compare(last_elem, in_front_elem) <= 0;
}

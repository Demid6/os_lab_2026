#include "find_max_min.h"

void GetMinMax(const int *array, size_t start, size_t end,
               int *min, int *max) {
    if (array == NULL || min == NULL || max == NULL || start >= end) {
        return;
    }

    int cur_min = array[start];
    int cur_max = array[start];

    for (size_t i = start + 1; i < end; ++i) {
        if (array[i] < cur_min) cur_min = array[i];
        if (array[i] > cur_max) cur_max = array[i];
    }

    *min = cur_min;
    *max = cur_max;
}

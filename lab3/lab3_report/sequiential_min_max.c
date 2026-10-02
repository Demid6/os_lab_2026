#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "find_max_min.h"

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <array_size> <start> <end> [seed]\n", argv[0]);
        return 1;
    }

    size_t n     = (size_t)strtoul(argv[1], NULL, 10);
    size_t start = (size_t)strtoul(argv[2], NULL, 10);
    size_t end   = (size_t)strtoul(argv[3], NULL, 10);
    unsigned seed = (argc > 4) ? (unsigned)strtoul(argv[4], NULL, 10)
                               : (unsigned)time(NULL);

    if (end > n || start >= end) {
        fprintf(stderr, "Invalid range\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); return 1; }

    srand(seed);
    for (size_t i = 0; i < n; ++i) arr[i] = rand() % 1000;

    int mn, mx;
    GetMinMax(arr, start, end, &mn, &mx);

    printf("min = %d, max = %d\n", mn, mx);

    free(arr);
    return 0;
}

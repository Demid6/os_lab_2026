#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "find_max_min.h"

typedef struct { int min; int max; } MinMax;

static MinMax worker(const int *arr, size_t start, size_t end) {
    MinMax r;
    GetMinMax(arr, start, end, &r.min, &r.max);
    return r;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr,
            "Usage: %s <array_size> <start> <end> [seed] [by_files]\n",
            argv[0]);
        return 1;
    }

    size_t n     = (size_t)strtoul(argv[1], NULL, 10);
    size_t start = (size_t)strtoul(argv[2], NULL, 10);
    size_t end   = (size_t)strtoul(argv[3], NULL, 10);
    unsigned seed = (argc > 4) ? (unsigned)strtoul(argv[4], NULL, 10)
                               : (unsigned)time(NULL);
    int by_files = (argc > 5 && strcmp(argv[5], "by_files") == 0);

    if (end > n || start >= end) { fprintf(stderr, "bad range\n"); return 1; }

    int *arr = malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); return 1; }
    srand(seed);
    for (size_t i = 0; i < n; ++i) arr[i] = rand() % 1000;

    size_t mid = start + (end - start) / 2;

    if (by_files) {
        const char *child_file  = "child_result.txt";
        const char *parent_file = "parent_result.txt";

        pid_t pid = fork();
        if (pid < 0) { perror("fork"); free(arr); return 1; }

        if (pid == 0) {
            MinMax r = worker(arr, mid, end);
            FILE *f = fopen(child_file, "w");
            if (!f) { perror("fopen child"); _exit(1); }
            fprintf(f, "%d %d\n", r.min, r.max);
            fclose(f);
            free(arr);
            _exit(0);
        }

        MinMax pr = worker(arr, start, mid);
        FILE *fp = fopen(parent_file, "w");
        if (!fp) { perror("fopen parent"); free(arr); return 1; }
        fprintf(fp, "%d %d\n", pr.min, pr.max);
        fclose(fp);

        int status;
        waitpid(pid, &status, 0);

        int cmin, cmax;
        FILE *fc = fopen(child_file, "r");
        if (!fc || fscanf(fc, "%d %d", &cmin, &cmax) != 2) {
            fprintf(stderr, "cannot read child result\n");
            if (fc) fclose(fc);
            free(arr); return 1;
        }
        fclose(fc);

        int gmin = pr.min < cmin ? pr.min : cmin;
        int gmax = pr.max > cmax ? pr.max : cmax;
        printf("min = %d, max = %d\n", gmin, gmax);

        unlink(child_file);
        unlink(parent_file);
    } else {
        int fd[2];
        if (pipe(fd) < 0) { perror("pipe"); free(arr); return 1; }

        pid_t pid = fork();
        if (pid < 0) { perror("fork"); free(arr); return 1; }

        if (pid == 0) {
            close(fd[0]);
            MinMax r = worker(arr, mid, end);
            if (write(fd[1], &r, sizeof(r)) != sizeof(r)) {
                perror("write");
                close(fd[1]); free(arr); _exit(1);
            }
            close(fd[1]);
            free(arr);
            _exit(0);
        }

        close(fd[1]);
        MinMax pr = worker(arr, start, mid);
        MinMax cr;
        ssize_t got = read(fd[0], &cr, sizeof(cr));
        close(fd[0]);

        int status;
        waitpid(pid, &status, 0);

        if (got != sizeof(cr)) {
            fprintf(stderr, "read failed\n");
            free(arr); return 1;
        }

        int gmin = pr.min < cr.min ? pr.min : cr.min;
        int gmax = pr.max > cr.max ? pr.max : cr.max;
        printf("min = %d, max = %d\n", gmin, gmax);
    }

    free(arr);
    return 0;
}

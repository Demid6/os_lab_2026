CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
COMMON  = find_max_min.c

.PHONY: all clean

all: sequential_min_max parallel_min_max run_sequential

sequential_min_max: sequiential_min_max.c $(COMMON) find_max_min.h
	$(CC) $(CFLAGS) -o $@ $^

parallel_min_max: parallel_min_max.c $(COMMON) find_max_min.h
	$(CC) $(CFLAGS) -o $@ $^

run_sequential: run_sequential.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f sequential_min_max parallel_min_max run_sequential \
	      child_result.txt parent_result.txt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100
#define GROUP_SIZE 2

typedef struct elf_range elf_range_t;
struct elf_range {
	unsigned long start;
	unsigned long end;
};

// returns true if range a is subset of b
bool is_subset(elf_range_t a, elf_range_t b) {
//        |aaaaa|
//       |bbbbbbb|
	return (a.start >= b.start && b.end >= a.end);
}

// returns true if ranges a and b overlap
bool has_overlap(elf_range_t a, elf_range_t b) {
//    |aaaaa|
//       |bbbbbb|
//    or
//       |aaaaa|
//    |bbbbbb|
	return	(b.start <= a.end && a.end <= b.end) ||
			(a.start <= b.end && b.end <= a.end);
}

int main() {
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {'0'};
	elf_range_t elf_group[GROUP_SIZE];
	int subset_cnt = 0;
	int overlap_cnt = 0;

	while (fgets(line, MAX_LINE_LEN, f)) {
		char *end_ptr = NULL;
		elf_group[0].start = strtoul(line, &end_ptr, 10);
		elf_group[0].end = strtoul(end_ptr+1, &end_ptr, 10);

		for (int i = 1; i < GROUP_SIZE; i++) {
			elf_group[i].start = strtoul(end_ptr+1, &end_ptr, 10);
			elf_group[i].end = strtoul(end_ptr+1, &end_ptr, 10);
		}
		/*
		printf("%zu-%zu,%zu-%zu\n",
				elf_group[0].start, elf_group[0].end,
				elf_group[1].start, elf_group[1].end
				);
		*/

		for (int i = 0; i < GROUP_SIZE; i++) { //every elf
			elf_range_t a = elf_group[i];

			for (int j = i+1; j < GROUP_SIZE; j++) { //with every other elf
				elf_range_t b = elf_group[j];

				subset_cnt += (is_subset(a, b) || is_subset(b, a));
				overlap_cnt += (has_overlap(a, b));
			}
		}
	}
	printf("Total full subsets %d\n", subset_cnt);
	printf("Total overlaps %d\n", overlap_cnt);
}

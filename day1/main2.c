#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LEN 100
#define TOP_N 3

unsigned long sum_of_n(unsigned long vals[], unsigned long len) {
	unsigned long sum = 0;
	for (unsigned long i = 0; i < len; i++) {
		sum += vals[i];
	}
	return sum;
}

unsigned long *find_smallest(unsigned long vals[], unsigned long len) {
	if (len == 0) {return NULL;}
	unsigned long *smallest = vals;
	for (unsigned long i = 1; i < len; i++) {
		if (*smallest > vals[i]) {
			smallest = vals + i; //&(vals[i])
		}
	}
	return smallest;
}

int main() {
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {0};
	char **end_ptr = NULL;

	unsigned long cur_num = 0;
	unsigned long cur_total = 0;
	unsigned long highest_totals[TOP_N] = {0};

	while (fgets(line, MAX_LINE_LEN, f)) {
		if (line[0] == '\n') {
			//next elf
			unsigned long *smallest = find_smallest(highest_totals, TOP_N);
			if (cur_total > *smallest) {
				*smallest = cur_total;
			}
			cur_total = 0;
			cur_num = 0;
		} else {
			//same elf
			cur_num = strtoul(line, end_ptr, 10);
			cur_total += cur_num;
		}
	}

	unsigned long top_n_total_sum = sum_of_n(highest_totals, TOP_N);
	printf("Top %d elfs carry %zu callories.\n",
			TOP_N, top_n_total_sum);
}

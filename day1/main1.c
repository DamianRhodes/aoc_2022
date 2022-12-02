#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LEN 100

int main() {
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {'0'};
	char **end_ptr = NULL;

	unsigned long cur_num = 0;
	unsigned long cur_total = 0;
	unsigned long highest_total = 0;

	while (fgets(line, MAX_LINE_LEN, f)) {
		if (line[0] == '\n') {
			//next elf
			if (cur_total > highest_total) {
				highest_total = cur_total;
			}
			cur_total = 0;
			cur_num = 0;
		} else {
			//same elf
			cur_num = strtoul(line, end_ptr, 10);
			cur_total += cur_num;
		}
	}
	printf("Most callories an elf is carrying is %zu.\n", highest_total);
}

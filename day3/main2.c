#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100
#define GROUP_SIZE 3

int get_priority(char c) {
	if ('a' <= c && c <= 'z') { return c - 'a' + 1; }
	else if ('A' <= c && c <= 'Z') { return c - 'A' + 1 + 26; }
	else {
		fprintf(stderr, "%c\n", c);
		//assert(false);
		return 0;
	}
}

int main() {
	FILE *f = stdin;
	char line[GROUP_SIZE][MAX_LINE_LEN] = {{'0'}};
	unsigned long len = 0;
	int cur_total = 0;
	int k = 0;
	char *shared = NULL;
	bool found = false;

	while (fgets(line[k], MAX_LINE_LEN, f)) {
		if (k <= 1) {
			k++;
			continue;
		}
		//printf("%s", line[0]);
		//printf("%zu\n", strlen(line[0]));
		k = 0;
		found = false;
		for (int i = 0; !found && i < strlen(line[0])-1; i++) { // for each item in first rucksack
			char *cur_c = line[0] + i;
			for (int j = 1; j < GROUP_SIZE; j++) { // check next rucksack

				shared = strchr(line[j], *cur_c);
				if (shared == NULL) {
					break;
				} else if (j + 1 == GROUP_SIZE) {
					cur_total += get_priority(*shared);
					found = true;
					break;
				}
			}
		}

	}
	printf("Total priority %d\n", cur_total);
}

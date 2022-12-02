#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100


int get_priority(char c) {
	if ('a' <= c && c <= 'z') { return c - 'a' + 1; }
	else if ('A' <= c && c <= 'Z') { return c - 'A' + 1 + 26; }
	else {
		fprintf(stderr, "%c", c);
		assert(false);
		return 0;
	}
}

int main() {
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {'0'};
	unsigned long len = 0;
	int cur_total = 0;

	while (fgets(line, MAX_LINE_LEN, f)) {
		len = strlen(line) - 1;
		char shared;
		for (int i = 0; i < len/2; i++) { // for each item in one compartment
			for (int j = len/2; j < len; j++) { // check each item in the second compartment
				if (line[i] == line[j]) {
					shared = line[i];
				}
			}
		}

		//printf("%c %d\n", shared, get_priority(shared));
		cur_total += get_priority(shared);
	}
	printf("Total priority %d\n", cur_total);
}

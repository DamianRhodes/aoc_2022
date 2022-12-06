#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100
#define MARKER_LEN 4

typedef struct marker marker_t;
struct marker {
	char str[MARKER_LEN+1];
	size_t start_index;
};


// cycle through marker str while the function f returns true
// return at what marker->str index the cycle ended
size_t marker_while(marker_t *m, char *c, bool (*f)(char *marker_c, char *call_c)) {
	for (size_t i = 0; true; i++) {
		size_t cur_index = (i + m->start_index) % MARKER_LEN;

		//if the called function returns false, exit
		if (!f((m->str) + cur_index, c)) {
			return cur_index;
		}
	}
}

// cycle through marker str once
void marker_foreach(marker_t *m, char *c, bool (*f)(char *marker_c, char *call_c)) {
	for (size_t i = 0; i < MARKER_LEN; i++) {
		size_t cur_index = (i + m->start_index) % MARKER_LEN;
		if (!f((m->str) + cur_index, c)) {
			return;
		}
	}
}

bool append_c(char *marker_c, char *call_c) {
	if (*marker_c == '\0') {
		*marker_c = *call_c;
		return false;
	}
	return true;
}

bool zero_to_c(char *marker_c, char *call_c) {
	if (*marker_c != *call_c) {
		*marker_c = '\0';
		return true;
	}
	*marker_c = '\0';
	return false;
}

void marker_add_char(marker_t *m, char c) {
	marker_while(m, &c, append_c);
}

const char *marker_strchr(const marker_t *m, char c) {
	for (size_t i = 0; i < sizeof(m->str) - 1; i++) {
		size_t cur_index = (i + m->start_index) % MARKER_LEN;

		if (m->str[cur_index] == '\0') {
			return NULL;
		} else if (m->str[cur_index] == c) {
			return m->str + cur_index;
		}
	}
	return NULL;
}

void marker_zero_before(marker_t *m, char c) {
	size_t cycle_end_i = marker_while(m, &c, zero_to_c);
	m->start_index = (cycle_end_i + 1) % MARKER_LEN;
	//printf("new m start: %zu\n", m->start_index);
}

bool printc(char *marker_c, char *idk ) {
	if (*marker_c != '\0') {
		putc(*marker_c, stdout);
		return true;
	}
	return false;
}

void marker_print(marker_t *m) {
	marker_foreach(m, NULL, printc);
}

void marker_dbg_print(marker_t *m) {
	printf("formated:");

	marker_foreach(m, NULL, printc);

	printf("\n");
	printf("raw mark:");
	putc('|', stdout);
	for (int i = 0; i < MARKER_LEN; i++) {
		if (m->str[i] == '\0') {
			putc(' ', stdout);
		} else {
			putc(m->str[i], stdout);
		}
	}
	putc('|', stdout);
	putc('\n', stdout);
	printf("          0123");
	putc('\n', stdout);
}


// mjqjpqmgbljsphdztnvjfqwrcgsmlb
// MJQ
//   QJP
//    JPQM
//
/// A|ABC |  B| ABC|
///  | BCA|   |B  C|
int main() {
	size_t char_index = 0;
	int c;
	marker_t cur_marker;

	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			break;
		}
		//marker_dbg_print(&cur_marker);
		char_index++;

		// if the character already is in the marker
		if (marker_strchr(&cur_marker, c)) {
			//printf("<zero> %c\n", c);

			marker_zero_before(&cur_marker, c);
			marker_add_char(&cur_marker, c);
			continue;

		// if the character is not in the marker
		} else {
			//printf("<add> %c\n", c);

			marker_add_char(&cur_marker, c);

			if (strlen(cur_marker.str) == sizeof(cur_marker.str) - 1) {
				printf("%zu", char_index);
				putc('\n',stdout);
				break;
			}
		}
	}

}

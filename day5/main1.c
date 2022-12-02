#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100
#define FIRST_BOX_DISTANCE 1
#define MID_BOX_DISTANCE 4
#define STACK_SIZE 100
#define STACK_COUNT 9

typedef struct stack stack_t;
struct stack {
	char content[STACK_SIZE];
	size_t height;
};

typedef struct crane_instr crane_instr_t;
struct crane_instr {
	size_t from;
	size_t to;
	size_t count;
};

crane_instr_t instr_normalize(crane_instr_t i) {
	i.from -= 1;
	i.to -= 1;
	return i;
}

bool stack_reterminate(stack_t *s) {
	if (s->height >= STACK_SIZE) {
		s->content[s->height] = '\0';
		return true;
	} else {
		return false;
	}
}

bool stack_push(stack_t *s, char box) {
	if (s->height >= STACK_SIZE) {
		fprintf(stderr, "Stack too small\n");
		exit(1);
		return false;
	} else {
		s->content[s->height] = box;
		s->height++;
		return stack_reterminate(s);
	}
}

bool stack_pop(stack_t *s, char *box) {
	if (s->height == 0) {
		fprintf(stderr, "Trying to pop an empty stack\n");
		exit(1);
		return false;
	} else {
		*box = s->content[s->height - 1];
		s->height--;
		return stack_reterminate(s);
	}
}

size_t stack_height(const stack_t *s) {
	return s->height;
}

void stack_flip(stack_t *s) {
	stack_t temp_stack = {.height = 0 };
	while (stack_height(s)) {
		char box;
		stack_pop(s, &box);
		stack_push(&temp_stack, box);
	}
	memcpy(s, &temp_stack, sizeof(temp_stack));
}

bool stack_move(stack_t *from, stack_t *to, size_t number) {
	bool fail = false;
	for (size_t i = 0; i < number; i++) {
		char box;
		fail |= !stack_pop(from, &box);
		fail |= !stack_push(to, box);
	}
	return fail;
}

bool read_boxes(stack_t stacks[STACK_COUNT], char *line) {
	size_t line_index = FIRST_BOX_DISTANCE;
	size_t cur_stack_index;
	if (line[0] == '\n') {
		return false;
	}

	while (line_index < strlen(line)) {
		cur_stack_index = (line_index - FIRST_BOX_DISTANCE)/MID_BOX_DISTANCE;
		if (line[line_index] == ' ') {
			line_index += MID_BOX_DISTANCE;
			continue;
		} else if (line[line_index] == '\n') {
			assert(false);
			return true;
		} else if ('A' <= line[line_index] && line[line_index] <= 'Z') {
			stack_push(&stacks[cur_stack_index], line[line_index]);
			line_index += MID_BOX_DISTANCE;
		} else if (line[line_index] == '1') {
			return false;
		}
	}
	return true;
}

void print_stack(stack_t s) {
	printf("%s\n", s.content);
}

int main() {
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {'0'};
	crane_instr_t instr;

	stack_t stacks[STACK_COUNT];
	for (size_t i = 0; i < STACK_COUNT; i++) {
		stacks[i].height = 0;
		stack_reterminate(&(stacks[i]));
	}

	bool creating_stacks = true;

	while (fgets(line, MAX_LINE_LEN, f)) {
		if (creating_stacks) {
			if (!read_boxes(stacks, line)) {
				creating_stacks = false;

				for (size_t i = 0; i < STACK_COUNT; i++) {
					stack_flip(&stacks[i]);
				/*
					printf("%zu: ", i+1);
					print_stack(stacks[i]);
				*/
				}
			}
		} else {
			if (sscanf(line, "move %zu from %zu to %zu",
						&(instr.count), &(instr.from), &(instr.to)) == 3) {
				instr = instr_normalize(instr);
				/*
				printf(line, "move %zu from %zu to %zu",
							&(instr.count), &(instr.from), &(instr.to));
				*/

				stack_move(&stacks[instr.from], &stacks[instr.to], instr.count);

				/*
				for (size_t i = 0; i < STACK_COUNT; i++) {
					printf("%zu: ", i+1);
					print_stack(stacks[i]);
				}
			*/
				//printf("---\n");
			}
			//printf(line, "move %zu from %zu to %zu\n", &move_count, &move_from, &move_to);
		}
	}

	//done reading crane instructions
	for (size_t i = 0; i < STACK_COUNT; i++) {
		if (stack_height(stacks + 1) == 0) {
			printf(" ");
		} else {
			char cur_box;
			stack_pop(stacks + i, &cur_box);
			printf("%c", cur_box);
		}
	}
	printf("\n");
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LINE_LEN 100
#define WIN_SCORE 6
#define DRAW_SCORE 3

typedef enum hand {
	H_ROCK = 1,
	H_PAPER = 2,
	H_SCISSORS = 3,
} hand_t;

typedef enum result {
	R_WIN = 1,
	R_DRAW = 2,
	R_LOSE = 3,
} result_t;

hand_t op_hand(char c) {
	switch (c) {
	case 'A': return H_ROCK;
	case 'B': return H_PAPER;
	case 'C': return H_SCISSORS;
	default:
		assert(false);
		return 0;
	}
}

result_t round_end(char c) {
	switch (c) {
	case 'X': return R_LOSE;
	case 'Y': return R_DRAW;
	case 'Z': return R_WIN;
	default:
		assert(false);
		return 0;
	}
}

hand_t my_hand(hand_t op, result_t result) {
	switch (result) {
	case R_WIN:
		return (op+1) == 4? 1 : op + 1;
	case R_DRAW:
		return op;
	case R_LOSE:
		return (op-1) == 0? 3 : op - 1;
	default:
		assert(false);
		return 0;
	}
}


int score_round(char *line) {
	hand_t op = op_hand(line[0]);
	result_t res = round_end(line[2]);
	hand_t me = my_hand(op, res);

	if (me == op) {
		//draw
		return me + DRAW_SCORE;
	} else if ((op + 1) == me || (op + 1 == 4 && me == 1)) { //some module shit
		//i win
		return me + WIN_SCORE;
	} else {
		//i lose
		return me;
	}
}

int main() {
	FILE *f = stdin;
	char line[MAX_LINE_LEN] = {'0'};

	unsigned long cur_total = 0;

	while (fgets(line, MAX_LINE_LEN, f)) {
		cur_total += score_round(line);
	}
	printf("I would score %zu.\n", cur_total);
}

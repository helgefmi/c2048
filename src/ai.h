#ifndef __AI_H
#define __AI_H

#include "board.h"

#define AI_NUM_TRIES 10
#define AI_DEPTH 5

extern long long int ai_nodes_searched;

void ai_loop();
int ai_search(board_t*, int, int, int*);
void ai_find_best_move(board_t*, int*);

#endif

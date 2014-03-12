#ifndef __CACHE_H
#define __CACHE_H

#include "board.h"

void cache_init();

extern int cache_directions[SIZE][4];
extern int cache_directions_last[SIZE][4];
extern int cache_directions_first[SIZE][4];

#endif

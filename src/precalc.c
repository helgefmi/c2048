#include "bitmanip.h"
#include "board.h"
#include "precalc.h"
#include "util.h"

static int move_to_delta[4][2] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0},
};

/* a bitmap used to quickly find all the cells a number can move to */
int precalc_directions[SIZE][4];

/* used to find the index of the most distant cell a number can move to */
int precalc_directions_last[SIZE][4];

/* used to find the index of the nearest cell a number can move to */
int precalc_directions_first[SIZE][4];

void precalc_init() {
    for (int i = 0; i < SIZE; ++i) {
        for (int move = 0; move < 4; ++move) {
            int dx = move_to_delta[move][0];
            int dy = move_to_delta[move][1];

            int x = i % COLSIZE;
            int y = i / COLSIZE;

            precalc_directions[i][move] = 0;
            precalc_directions_last[i][move] = -1;
            precalc_directions_first[i][move] = -1;

            for (x += dx, y += dy; x >= 0 && x < COLSIZE && y >= 0 && y < COLSIZE; x += dx, y += dy) {
                precalc_directions[i][move] |= (1 << (y * COLSIZE + x));
                precalc_directions_last[i][move] = y * COLSIZE + x;
                if (precalc_directions_first[i][move] == -1) {
                    precalc_directions_first[i][move] = y * COLSIZE + x;
                }
            }
        }
    }
}

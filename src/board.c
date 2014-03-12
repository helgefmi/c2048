#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bitmanip.h"
#include "board.h"
#include "ai.h"
#include "test.h"
#include "precalc.h"

static void fill_random_cell(board_t*);
static int get_score(int);

static int score_map[] = {
        2, //     2
        5, //     4
       10, //     8
       25, //    16
       50, //    32
      125, //    64
      250, //   128
      500, //   256
     1000, //   512
     2000, //  1024
     4000, //  2048
     8000, //  4096
    16000, //  8192
    32500, // 16384
};

static int get_score(int number) {
    if (number > 16384) {
        return number * 2;
    }

    /* since `number` is always a power of two, we can use LSB() to get its index into `score_map`. */
    unsigned int score_i = LSB(number);
    return score_map[score_i - 1];
}

static void fill_random_cell(board_t *board) {
    /* we don't want randomness while doing testcases. */
    if (test_status == DOING_TESTS) {
        return;
    }

    /* bitmap of all the unoccupied squares */
    int bitmap_flipped = ~board->occupied_cells & ((1 << SIZE) - 1);
    if (bitmap_flipped) {
        int num_unoccupied = PopCnt(bitmap_flipped);

        /* remove a bit from the bottom of `bitmap_flipped`, `rand() % num_unoccupied` times. */
        for (int r = rand() % num_unoccupied; r; --r) {
            bitmap_flipped &= bitmap_flipped - 1;
        }

        /* Get the index of the lowermost bit, which should now be a random index of the unoccupied squares. */
        int rand_i = LSB(bitmap_flipped);
        int rand_val = (rand() % 9) ? 2 : 4;

        board->cells[rand_i] = rand_val;
        board->occupied_cells |= (1 << rand_i);
        board->score += rand_val;
    }
}

board_t board_init() {
    board_t board;
    memset(&board, 0, sizeof(board_t));
    for (int i = 0; i < 2; ++i)
        fill_random_cell(&board);
    return board;
}

void board_setup_bitfields(board_t *board) {
    board->occupied_cells = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (board->cells[i]) {
            board->occupied_cells |= (1 << i);
        }
    }
}

void board_move(board_t *board, int move) {
    int reversed = move == MOVE_UP || move == MOVE_LEFT;

    /* `bitmap` represents all the numbers that we haven't moved yet */
    for (int bitmap = board->occupied_cells; bitmap;) {
        int i = reversed ? MSB(bitmap) : LSB(bitmap);
        int val = board->cells[i];

        bitmap ^= (1 << i);

        /* did this number collide with another number? */
        int collision_bitmap = precalc_directions[i][move] & board->occupied_cells;

        if (!collision_bitmap) {
            /* if not, put the number on the most distant square (if it's not there already) */
            int last_i = precalc_directions_last[i][move];
            if (last_i >= 0) {
                board->occupied_cells ^= (1 << i) | (1 << last_i);
                board->cells[i] = 0;
                board->cells[last_i] = val;
            }
        }
        else {
            /* we have a collision; find out the index and value */
            int other_i = reversed ? MSB(collision_bitmap) : LSB(collision_bitmap);
            int other_val = board->cells[other_i];

            if (other_val == val) {
                /* score! */
                board->occupied_cells ^= 1 << i;
                board->cells[i] = 0;
                board->cells[other_i] = val * 2;
                board->score += get_score(val);
                /* remove the number from `bitmap` as it's been merged, and so it shouldn't move */
                bitmap &= ~(1 << other_i);
            }
            else {
                /* two different numbers; move to its neighbor square if we're not there already */
                int first_i = precalc_directions_first[other_i][(move + 2) % 4]; // `(move + 2) % 4` flips the direction.
                if (first_i >= 0 && first_i != i) {
                    board->occupied_cells ^= (1 << i) | (1 << first_i);
                    board->cells[i] = 0;
                    board->cells[first_i] = val;
                }
            }
        }
    }

    fill_random_cell(board);
}

void board_print(board_t board) {
    for (int y = 0; y < COLSIZE; ++y) {
        for (int x = 0; x < COLSIZE; ++x) {
            printf("%d ", board.cells[y * COLSIZE + x]);
        }
        printf("\n");
    }
    printf("Score: %d\n", board.score);
    printf("\n");
}

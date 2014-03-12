#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>
#include "test.h"
#include "board.h"
#include "ai.h"

int test_status = NOT_DOING_TESTS;

void rotate_board(board_t *board) {
    board_t rotated;

    for (int y = 0; y < COLSIZE; ++y) {
        for (int x = 0; x < COLSIZE; ++x) {
            rotated.cells[y * COLSIZE + x] = board->cells[(COLSIZE - x - 1) * COLSIZE + y];
        }
    }

    memcpy((void*) board->cells, (void*) rotated.cells, sizeof(board->cells));
}

void test_run_testcases() {
    struct {
        board_t init;
        int move;
        board_t after;
    } testcases[] = {{
    /* 1 */
    {{
        2, 2, 2, 0,
        2, 0, 0, 0,
        2, 2, 4, 2,
        4, 4, 4, 4
    }, 0, 0},
    MOVE_RIGHT,
    {{
        0, 4, 0, 2,
        0, 0, 0, 2,
        0, 4, 4, 2,
        0, 8, 0, 8
     }, 0, 0} },

    /* 2 */
    { {{
           2,    4,      8,    16,
          32,   64,    128,   256,
         512,  1024,  2048,  4096,
        8192, 16384, 32768, 65536,
    }, 0, 0},
    MOVE_LEFT,
    {{
           2,    4,      8,    16,
          32,   64,    128,   256,
         512,  1024,  2048,  4096,
        8192, 16384, 32768, 65536,
        }, 0, 0} },

    /* 3 */
    { {{
        2, 4, 2, 0,
        2, 4, 0, 4,
        2, 8, 4, 2,
        8, 4, 4, 4
    }, 0, 0},
    MOVE_RIGHT,
    {{
        2, 4, 0, 2,
        2, 0, 0, 8,
        2, 8, 4, 2,
        8, 0, 8, 4
    }, 0, 0} },

    };

    int num_tests = sizeof(testcases) / sizeof(testcases[0]);
    for (int test_i = 0; test_i < num_tests; ++test_i) {
        /* Try the test case with every possible rotation */
        for (int dir = 0; dir < 4; ++dir) {
            board_t board;
            memcpy((void*) &board, (void*) &testcases[test_i].init, sizeof(board_t));

            board_t solution;
            memcpy((void*) &solution, (void*) &testcases[test_i].after, sizeof(board_t));

            int move = testcases[test_i].move;

            for (int i = 0; i < dir; ++i) {
                rotate_board(&board);
                rotate_board(&solution);
                move = (move + 1) % 4;
            }

            board_setup_bitfields(&board);
            board_move(&board, move);

            if (0 != memcmp(board.cells, solution.cells, sizeof(board.cells))) {
                printf("Failed test case: %d (%d)\n", test_i, dir);

                puts("Start:");
                board_print(testcases[test_i].init);

                printf("Move: %d\n", move);

                puts("Me:");
                board_print(board);

                puts("Test solution:");
                board_print(solution);

                exit(1);
            }
        }
    }

    printf("%d tests OK!\n", num_tests);

    /* making sure that the AI knows what to do when there's only one move */
    board_t board = {
        {  2,  4,   8,  16,
          32, 64, 128, 256,
           2,  4,   8,  16,
          32, 64, 128, 128 },
        0, 0
    };
    board_setup_bitfields(&board);
    assert(board.occupied_cells == 0xFFFF);

    int best_move = -1;
    ai_search(&board, 10, 0, &best_move);
    assert(best_move >= 0);
    assert(best_move == MOVE_RIGHT);
}

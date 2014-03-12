#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "board.h"
#include "ai.h"
#include "test.h"
#include "interface.h"
#include "precalc.h"

int main(int argc, char **argv) {
    int seed = time(NULL);
    //seed = 1394569404;
    printf("Random seed: %d\n", seed);
    srand(seed);

    precalc_init();

    if (argc == 2) {
        if (0 == strcmp(argv[1], "--human")) {
            board_t board = board_init();

            while (1) {
                board_print(board);

                printf("[hjkl] ");
                char key = getchar();
                getchar(); // eat newline

                if (key == 'k') {
                    board_move(&board, MOVE_UP);
                }
                else if (key == 'l') {
                    board_move(&board, MOVE_RIGHT);
                }
                else if (key == 'j') {
                    board_move(&board, MOVE_DOWN);
                }
                else if (key == 'h') {
                    board_move(&board, MOVE_LEFT);
                }
            }
        }
        else if (0 == strcmp(argv[1], "--test")) {
            test_status = DOING_TESTS; // disable board_fill_random_cell
            test_run_testcases();
            test_status = NOT_DOING_TESTS;
        }
        else if (0 == strcmp(argv[1], "--ai")) {
            ai_loop();
        }
        else {
            interface_main(argv[1]);
        }
    }
    else {
        puts("--human  Play on the console, using hjkl to move around");
        puts("--test   Runs the testcases and does a performance test (nodes/sec)");
        puts("--ai     Runs infinitly many games in a row, while tracking stats (avg, max, nps, etc.)");
        return 1;
    }

    return 0;
}

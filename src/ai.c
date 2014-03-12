#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bitmanip.h"
#include "ai.h"
#include "board.h"
#include "util.h"

long long int ai_nodes_searched = 0;

static inline int eval_board(board_t *board) {
    return board->score - PopCnt(board->occupied_cells) + SIZE;
}

/* depth first search with no pruning */
int ai_search(board_t *board, int depth, int ply, int *best_move) {
    ai_nodes_searched += 1;

    if (depth == 0) {
        return eval_board(board);
    }

    /* in case there's no possible moves, we'll return `up`. let `ai_play` and `interface_main`
     * worry about when the game has ended. */
    int best_ply_move = MOVE_UP;
    int best_ply_score = board->score;

    board_t board_copy;
    memcpy(&board_copy, board, sizeof(board_t));

    int score_before = board->score;

    for (int move = 0; move < 4; ++move) {
        board_move(board, move);

        /* we check to see if we did nothing at all. no point in searching further then, since it'll be
         * the same exact position. */
        if (score_before == board->score) {
            memcpy(board, &board_copy, sizeof(board_t));
            continue;
        }

        int score = ai_search(board, depth - 1, ply + 1, NULL);

        if (score > best_ply_score) {
            best_ply_score = score;
            best_ply_move = move;
        }

        memcpy(board, &board_copy, sizeof(board_t));
    }

    if (ply == 0) {
        *best_move = best_ply_move;
    }

    return best_ply_score;
}

void ai_find_best_move(board_t *board, int *best_move) {
    int move_to_popularity[4] = {0};

    /* since this game is affected by rand(), doing a naive depth first search
     * will not necessarily give a good move, since we could have had really bad luck.
     * so instead of doing it once, we do it `AI_NUM_TRIES` times, and take the move that
     * was returned the most.
     *
     * since we search upwards first in `ai_search()`, this is usually the move that's
     * returned in open positions. */
    for (int try = 0; try < AI_NUM_TRIES; ++try) {
        int move = -1;
        ai_search(board, AI_DEPTH, 0, &move);

        assert(move >= 0 && move < 4);

        move_to_popularity[move] += 1;
    }

    int most_popular = 0;
    for (int move = 0; move < 4; ++move) {
        if (move_to_popularity[move] > most_popular) {
            most_popular = move_to_popularity[move];
            *best_move = move;
        }
    }
}

void ai_loop() {
    board_t board = board_init();

    int max_score = 0;
    long long unsigned int score_sum = 0;
    int tries = 0;

    double start_time = util_get_timestamp();

    printf("avg\tmax\tcur\tsecs\tnps\n");
    while (1) {
        int move = -1;
        ai_find_best_move(&board, &move);

        if (move == -1) {
            break;
        }

        int score_before = board.score;
        board_move(&board, move);

        if (score_before == board.score) {
            if (board.score > max_score) {
                max_score = board.score;
            }
            score_sum += board.score;
            tries += 1;

            double spent_time = util_get_timestamp() - start_time;
            double seconds = spent_time / 1000000.0;
            double mill_nodes = (double)ai_nodes_searched / 1000000.0;

            printf("%llu\t%d\t%d\t%.2f\t%.2f\n", score_sum / tries, max_score, board.score, seconds, mill_nodes / seconds);
            board = board_init();
        }
    }
}

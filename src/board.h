#ifndef __BOARD_H
#define __BOARD_H

#define COLSIZE 4
#define SIZE (COLSIZE * COLSIZE)

#define MOVE_UP 0
#define MOVE_RIGHT 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3

typedef struct board_t {
    int cells[SIZE];
    int score;
    int occupied_cells;
} board_t;

board_t board_init();
void board_print(board_t);
void board_move(board_t*, int);
void board_setup_bitfields(board_t*);

#endif

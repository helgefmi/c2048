#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "interface.h"
#include "board.h"
#include "ai.h"
#include "util.h"

static char move_to_char[4] = {'u', 'r', 'd', 'l', };

static board_t get_board_from_socket(int s, int *should_exit) {
    char buffer[4096];
    if (recv(s, buffer, 4096, 0) <= 0) {
        perror("recv");
        exit(1);
    }

    board_t board;

    if(0 != strstr(buffer, "FIN")) {
        *should_exit = 1;
        return board;
    }

    char *pch = strtok(buffer, " \n");
    for (int i = 0; i < SIZE; ++i) {
        assert(pch != NULL);
        board.cells[i] = atoi(pch);
        pch = strtok(NULL, " \n");
    }

    /* meh */
    board.score = 0;

    board_setup_bitfields(&board);
    return board;
}

static int get_socket(char *path) {
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_un remote;
    memset(&remote, 0, sizeof(remote));

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, path);

    if (connect(s, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
        perror("connect");
        exit(1);
    }
    return s;
}

void interface_main(char *unix_socket_path) {
    int s = get_socket(unix_socket_path);

    char welcome[] = "helge\n";
    if (send(s, welcome, strlen(welcome), 0) < 0) {
        perror("send");
        exit(1);
    }

    while (1) {
        int should_exit = 0;
        board_t board = get_board_from_socket(s, &should_exit);

        if (should_exit)  {
            break;
        }

        int best_move = -1;
        ai_find_best_move(&board, &best_move);

        char move_str[3] = " \n";
        move_str[0] = move_to_char[best_move];

        if (send(s, move_str, 2, 0) < 0) {
            perror("send");
            exit(1);
        }
    }

    close(s);
}

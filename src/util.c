#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "board.h"

void util_print_bitmap(int bitmap) {
    for (int y = 0; y < COLSIZE; ++y) {
        for (int x = 0; x < COLSIZE; ++x) {
            int mask = 1 << (y * COLSIZE + x);
            printf("%c ", (bitmap & mask) ? 'x' : '.');
        }
        printf("\n");
    }
}

double util_get_timestamp() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000.0 + now.tv_usec;
}

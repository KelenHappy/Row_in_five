#include "chessData.h"

void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    while (1) {
        *x = rand() % 20 + 1;
        *y = rand() % 20 + 1;
        if (setXY(chess, *x, *y, player) == 1) {
            return;
        }
    }
}


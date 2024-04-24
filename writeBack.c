#include "chessData.h"
#include <limits.h>
#define BOARD_SIZE 20

int isGameOver(Chess* chess, int player) {
    // 檢查是否有一方連成五子
    for (int i = 0; i < chess->size; i++) {
        int count = 1; // 計數器初始化為1，因為已經有一子在該位置
        int x = chess->cord[i].x;
        int y = chess->cord[i].y;

        // 水平方向
        for (int dx = 1; dx < 5; dx++) {
            if (setXY(chess, x + dx, y, player)) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1; // 如果有五子連成一線，返回遊戲結束

        // 垂直方向
        count = 1; // 重新初始化計數器
        for (int dy = 1; dy < 5; dy++) {
            if (setXY(chess, x, y + dy, player)) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1; // 如果有五子連成一線，返回遊戲結束

        // 左斜方向
        count = 1; // 重新初始化計數器
        for (int d = 1; d < 5; d++) {
            if (setXY(chess, x + d, y + d, player)) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1; // 如果有五子連成一線，返回遊戲結束

        // 右斜方向
        count = 1; // 重新初始化計數器
        for (int d = 1; d < 5; d++) {
            if (setXY(chess, x + d, y - d, player)) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1; // 如果有五子連成一線，返回遊戲結束
    }

    // 如果沒有玩家連成五子，且棋盤已滿，則遊戲平局
    if (chess->size == BOARD_SIZE * BOARD_SIZE) return -1;

    // 遊戲還未結束
    return 0;
}
int evaluateBoard(Chess* chess, int player) {
    int score = 0;
    int opponent = (player == 1) ? 0 : 1; // 對手的玩家

    // 計算每個玩家的連線數量並給予分數
    int playerCount = 0;
    int opponentCount = 0;
    for (int i = 0; i < chess->size; i++) {
        if (chess->cord[i].player == player) {
            playerCount++;
        } else {
            opponentCount++;
        }
    }
    score += (playerCount - opponentCount) * 10;

    // 棋盤的中心控制
    int centerControl = 0;
    for (int i = 0; i < chess->size; i++) {
        if (chess->cord[i].x >= 8 && chess->cord[i].x <= 15 && chess->cord[i].y >= 8 && chess->cord[i].y <= 15) {
            if (chess->cord[i].player == player) {
                centerControl++;
            } else {
                centerControl--;
            }
        }
    }
    score += centerControl * 20;

    // 檢查連線情況
    // 水平方向
    for (int i = 0; i < 20; i++) {
        int count = 0;
        for (int j = 0; j < 20; j++) {
            if (setXY(chess, i, j, player)) {
                count++;
            } else {
                count = 0;
            }
            if (count >= 5) {
                score += 100;
            }
        }
    }
    // 垂直方向
    for (int j = 0; j < 20; j++) {
        int count = 0;
        for (int i = 0; i < 20; i++) {
            if (setXY(chess, i, j, player)) {
                count++;
            } else {
                count = 0;
            }
            if (count >= 5) {
                score += 100;
            }
        }
    }
    // 對角線方向
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int count1 = 0, count2 = 0;
            for (int k = 0; k < 20; k++) {
                if (i + k < 20 && j + k < 20) {
                    if (setXY(chess, i + k, j + k, player)) {
                        count1++;
                    } else {
                        count1 = 0;
                    }
                    if (count1 >= 5) {
                        score += 100;
                    }
                }
                if (i + k < 20 && j - k >= 0) {
                    if (setXY(chess, i + k, j - k, player)) {
                        count2++;
                    } else {
                        count2 = 0;
                    }
                    if (count2 >= 5) {
                        score += 100;
                    }
                }
            }
        }
    }

    return score;
}


int minimax(Chess* chess, int depth, int maximizingPlayer, int player, int* bestX, int* bestY) {
    if (depth == 0 || isGameOver(chess, player)) {
        return evaluateBoard(chess, player);
    }

    int bestScore;
    if (maximizingPlayer) {
        bestScore = INT_MIN;
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                if (setXY(chess, i, j, player)) {
                    int score = minimax(chess, depth - 1, 0, 1 - player, NULL, NULL); // Pass NULL for bestX and bestY
                    if (score > bestScore) {
                        bestScore = score;
                        *bestX = i;
                        *bestY = j;
                    }
                    // Undo the current step
                    chess->size--;
                }
            }
        }
    } else {
        bestScore = INT_MAX;
        for (int i = 1; i <= 20; i++) {
            for (int j = 1; j <= 20; j++) {
                if (setXY(chess, i, j, player)) {
                    int score = minimax(chess, depth - 1, 1, 1 - player, NULL, NULL); // Pass NULL for bestX and bestY
                    if (score < bestScore) {
                        bestScore = score;
                        *bestX = i;
                        *bestY = j;
                    }
                    // Undo the current step
                    chess->size--;
                }
            }
        }
    }

    return bestScore;
}

void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    if (chess->size == 0) {
        // 如果棋盘上没有棋子，则默认下一步棋为正中心位置
        *x = BOARD_SIZE / 2;
        *y = BOARD_SIZE / 2;
    } else {
        // 如果棋盘上有棋子，则使用 minimax 算法确定下一步棋的位置
        int bestX, bestY;
        minimax(chess, 3, 1, player, &bestX, &bestY);
        *x = bestX;
        *y = bestY;
    }

    // 如果下一步棋的位置有效，则在棋盘上落子
    if (setXY(chess, *x, *y, player) == 1) {
        return;
    }
}

/*
void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    int bestX, bestY;
    minimax(chess, 3, 1, player, &bestX, &bestY);
    *x = rand()%20+1;
    *y = rand()%20+1;
	if (setXY(chess, *x, *y, player) == 1) {
            return;
    }
}
*/
// writeBack.c
#include "chessData.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 20

void unsetXY(Chess* chess, int x, int y) {
    for (int i = 0; i < chess->size; i++) {
        if (chess->cord[i].x == x && chess->cord[i].y == y) {
            chess->cord[i] = chess->cord[chess->size - 1];
            chess->size--;
            return;
        }
    }
}

void createBoard(Chess* chess, int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = -1;
        }
    }
    for (int i = 0; i < chess->size; i++) {
        board[chess->cord[i].x][chess->cord[i].y] = chess->cord[i].player;
    }
}

int isGameOver(Chess* chess, int player) {
    int board[BOARD_SIZE][BOARD_SIZE];
    createBoard(chess, board);

    for (int i = 0; i < chess->size; i++) {
        int x = chess->cord[i].x;
        int y = chess->cord[i].y;

        // Horizontal check
        int count = 1;
        for (int dx = 1; dx < 5; dx++) {
            if (x + dx < BOARD_SIZE && board[x + dx][y] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;

        // Vertical check
        count = 1;
        for (int dy = 1; dy < 5; dy++) {
            if (y + dy < BOARD_SIZE && board[x][y + dy] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;

        // Diagonal check (/)
        count = 1;
        for (int d = 1; d < 5; d++) {
            if (x + d < BOARD_SIZE && y + d < BOARD_SIZE && board[x + d][y + d] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;

        // Anti-diagonal check (\)
        count = 1;
        for (int d = 1; d < 5; d++) {
            if (x + d < BOARD_SIZE && y - d >= 0 && board[x + d][y - d] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;
    }

    // Check for a draw
    if (chess->size == BOARD_SIZE * BOARD_SIZE) return -1;

    return 0;
}

int evaluateBoard(Chess* chess, int player) {
    int board[BOARD_SIZE][BOARD_SIZE];
    createBoard(chess, board);

    int score = 0;
    int opponent = 1 - player;

    // Center Control Heuristic
    int centerControl = 0;
    for (int i = 8; i <= 10; i++) {
        for (int j = 8; j <= 10; j++) {
            if (board[i][j] == player) {
                centerControl++;
            } else if (board[i][j] == opponent) {
                centerControl--;
            }
        }
    }
    score += centerControl * 20;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                // Horizontal
                int count = 0;
                int openLineCount = 0;
                for (int dx = 0; dx < 5 && i + dx < BOARD_SIZE; dx++) {
                    if (board[i + dx][j] == player) {
                        count++;
                    } else if (board[i + dx][j] == -1) {
                        openLineCount++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10 + openLineCount;

                // Vertical
                count = 0;
                openLineCount = 0;
                for (int dy = 0; dy < 5 && j + dy < BOARD_SIZE; dy++) {
                    if (board[i][j + dy] == player) {
                        count++;
                    } else if (board[i][j + dy] == -1) {
                        openLineCount++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10 + openLineCount;

                // Diagonal
                count = 0;
                openLineCount = 0;
                for (int d = 0; d < 5 && i + d < BOARD_SIZE && j + d < BOARD_SIZE; d++) {
                    if (board[i + d][j + d] == player) {
                        count++;
                    } else if (board[i + d][j + d] == -1) {
                        openLineCount++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10 + openLineCount;

                // Anti-diagonal
                count = 0;
                openLineCount = 0;
                for (int d = 0; d < 5 && i + d < BOARD_SIZE && j - d >= 0; d++) {
                    if (board[i + d][j - d] == player) {
                        count++;
                    } else if (board[i + d][j - d] == -1) {
                        openLineCount++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10 + openLineCount;
            }
        }
    }

    int playerMobility = 0;
    int opponentMobility = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                // Check surrounding empty cells
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx != 0 || dy != 0) {  
                            int newX = i + dx;
                            int newY = j + dy;
                            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && board[newX][newY] == -1) {
                                playerMobility++;
                            }
                        }
                    }
                }
            } else if (board[i][j] == opponent) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx != 0 || dy != 0) {
                            int newX = i + dx;
                            int newY = j + dy;
                            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && board[newX][newY] == -1) {
                                opponentMobility++;
                            }
                        }
                    }
                }
            }
        }
    }
    score += (playerMobility - opponentMobility) * 5;  // Adjust score based on mobility difference

    return score;
}

int minimax(Chess* chess, int depth, int alpha, int beta, int maximizingPlayer, int player, int* bestX, int* bestY) {
    int gameState = isGameOver(chess, player);
    if (depth == 0 || gameState != 0) {
        return evaluateBoard(chess, player);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                int validMove = setXY(chess, i, j, player);
                if (validMove) {
                    int eval = minimax(chess, depth - 1, alpha, beta, 0, 1 - player, NULL, NULL);
                    unsetXY(chess, i, j);
                    if (eval > maxEval) {
                        maxEval = eval;
                        if (bestX != NULL && bestY != NULL) {
                            *bestX = i;
                            *bestY = j;
                        }
                    }
                    alpha = alpha > eval ? alpha : eval;
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                int validMove = setXY(chess, i, j, player);
                if (validMove) {
                    int eval = minimax(chess, depth - 1, alpha, beta, 1, 1 - player, NULL, NULL);
                    unsetXY(chess, i, j);
                    if (eval < minEval) {
                        minEval = eval;
                        if (bestX != NULL && bestY != NULL) {
                            *bestX = i;
                            *bestY = j;
                        }
                    }
                    beta = beta < eval ? beta : eval;
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return minEval;
    }
}

void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    if (chess->size == 0) {
        *x = BOARD_SIZE / 2;
        *y = BOARD_SIZE / 2;
    } else {
        int bestX = -1, bestY = -1;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (setXY(chess, i, j, player)) {
                    if (isGameOver(chess, player) == 1) {
                        *x = i;
                        *y = j;
                        return;
                    }
                    unsetXY(chess, i, j);
                }
            }
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (setXY(chess, i, j, 1 - player)) {
                    if (isGameOver(chess, 1 - player) == 1) {
                        *x = i;
                        *y = j;
                        unsetXY(chess, i, j);
                        setXY(chess, *x, *y, player);  // Place the blocking piece
                        return;
                    }
                    unsetXY(chess, i, j);
                }
            }
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (setXY(chess, i, j, player)) {
                    int count = 0;
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (dx == 0 && dy == 0) continue;
                            if (i + dx >= 0 && i + dx < BOARD_SIZE && j + dy >= 0 && j + dy < BOARD_SIZE) {
                                if (setXY(chess, i + dx, j + dy, player)) {
                                    if (isGameOver(chess, player) == 1) {
                                        count++;
                                    }
                                    unsetXY(chess, i + dx, j + dy);
                                }
                            }
                        }
                    }
                    if (count >= 2) {
                        *x = i;
                        *y = j;
                        unsetXY(chess, i, j);
                        setXY(chess, *x, *y, player);  // Place the fork piece
                        return;
                    }
                    unsetXY(chess, i, j);
                }
            }
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (setXY(chess, i, j, 1 - player)) {
                    int count = 0;
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (dx == 0 && dy == 0) continue;
                            if (i + dx >= 0 && i + dx < BOARD_SIZE && j + dy >= 0 && j + dy < BOARD_SIZE) {
                                if (setXY(chess, i + dx, j + dy, 1 - player)) {
                                    if (isGameOver(chess, 1 - player) == 1) {
                                        count++;
                                    }
                                    unsetXY(chess, i + dx, j + dy);
                                }
                            }
                        }
                    }
                    if (count >= 2) {
                        *x = i;
                        *y = j;
                        unsetXY(chess, i, j);
                        setXY(chess, *x, *y, player);  // Block the opponent's fork
                        return;
                    }
                    unsetXY(chess, i, j);
                }
            }
        }

        int centerX = BOARD_SIZE / 2;
        int centerY = BOARD_SIZE / 2;
        if (setXY(chess, centerX, centerY, player)) {
            *x = centerX;
            *y = centerY;
            return;
        }

        minimax(chess, 3, INT_MIN, INT_MAX, 1, player, &bestX, &bestY);
        if (bestX >= 0 && bestX < BOARD_SIZE && bestY >= 0 && bestY < BOARD_SIZE) {
            *x = bestX;
            *y = bestY;
        } else {
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (setXY(chess, i, j, player)) {
                        *x = i;
                        *y = j;
                        return;
                    }
                }
            }
        }
    }
    setXY(chess, *x, *y, player);
}

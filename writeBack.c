//writeBack.c
#include "chessData.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 19

// This function should remove a piece at (x, y)
void unsetXY(Chess* chess, int x, int y) {
    for (int i = 0; i < chess->size; i++) {
        if (chess->cord[i].x == x && chess->cord[i].y == y) {
            for (int j = i; j < chess->size - 1; j++) {
                chess->cord[j] = chess->cord[j + 1];
            }
            chess->size--;
            return;
        }
    }
}

// Create a temporary board for internal use in functions
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

// Check if the game is over
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

// Evaluate the board for a given player
int evaluateBoard(Chess* chess, int player) {
    int board[BOARD_SIZE][BOARD_SIZE];
    createBoard(chess, board);

    int score = 0;
    int opponent = 1 - player;

    // Central control
    int centerControl = 0;
    for (int i = 8; i <= 15; i++) {
        for (int j = 8; j <= 15; j++) {
            if (board[i][j] == player) {
                centerControl++;
            } else if (board[i][j] == opponent) {
                centerControl--;
            }
        }
    }
    score += centerControl * 20;

    // Line checking and scoring
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                // Horizontal
                int count = 0;
                for (int dx = 0; dx < 5 && i + dx < BOARD_SIZE; dx++) {
                    if (board[i + dx][j] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10;

                // Vertical
                count = 0;
                for (int dy = 0; dy < 5 && j + dy < BOARD_SIZE; dy++) {
                    if (board[i][j + dy] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10;

                // Diagonal
                count = 0;
                for (int d = 0; d < 5 && i + d < BOARD_SIZE && j + d < BOARD_SIZE; d++) {
                    if (board[i + d][j + d] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10;

                // Anti-diagonal
                count = 0;
                for (int d = 0; d < 5 && i + d < BOARD_SIZE && j - d >= 0; d++) {
                    if (board[i + d][j - d] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
                else score += count * 10;
            }
        }
    }

    return score;
}

// Alpha-beta pruning for minimax algorithm
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

// Write the next move on the chess board
void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    if (chess->size == 0) {
        // Place the first piece in the center
        *x = BOARD_SIZE / 2;
        *y = BOARD_SIZE / 2;
    } else {
        int bestX = -1, bestY = -1;

        // 1. Winning Move
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

        // 2. Block Opponent's Win
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (setXY(chess, i, j, 1 - player)) {
                    if (isGameOver(chess, 1 - player) == 1) {
                        *x = i;
                        *y = j;
                        return;
                    }
                    unsetXY(chess, i, j);
                }
            }
        }

        // 3. Fork Opportunity
        // Your implementation for fork detection

        // 4. Block Opponent's Fork
        // Your implementation for fork blocking

        // 5. Center Control
        int centerX = BOARD_SIZE / 2;
        int centerY = BOARD_SIZE / 2;
        if (setXY(chess, centerX, centerY, player)) {
            *x = centerX;
            *y = centerY;
            return;
        }

        // 6. Corner Control
        int corners[4][2] = {{0, 0}, {0, BOARD_SIZE - 1}, {BOARD_SIZE - 1, 0}, {BOARD_SIZE - 1, BOARD_SIZE - 1}};
        for (int i = 0; i < 4; i++) {
            if (setXY(chess, corners[i][0], corners[i][1], player)) {
                *x = corners[i][0];
                *y = corners[i][1];
                return;
            }
        }

        // If no strategic move found, use minimax
        minimax(chess, 3, INT_MIN, INT_MAX, 1, player, &bestX, &bestY);

        // Ensure the move is within the board bounds
        while (bestX < 0 || bestX >= BOARD_SIZE || bestY < 0 || bestY >= BOARD_SIZE) {
            // Recalculate the move until a valid one is found
            minimax(chess, 3, INT_MIN, INT_MAX, 1, player, &bestX, &bestY);
        }

        *x = bestX;
        *y = bestY;
    }

    // Place the piece on the board
    setXY(chess, *x, *y, player);
}


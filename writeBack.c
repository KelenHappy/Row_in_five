#include "chessData.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 20

// This function should check if placing a piece at (x, y) for player is valid and update the board if valid.
int setXY(Chess* chess, int x, int y, int player) {
    // Ensure coordinates are within bounds and the position is not already occupied
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || chess->board[x][y] != -1) {
        return 0; // Invalid move
    }
    chess->board[x][y] = player; // Place the piece
    chess->cord[chess->size].x = x; // Update the coordinates
    chess->cord[chess->size].y = y;
    chess->cord[chess->size].player = player;
    chess->size++;
    return 1; // Move was valid and successfully placed
}

// This function should remove a piece at (x, y)
void unsetXY(Chess* chess, int x, int y) {
    chess->board[x][y] = -1; // Clear the piece
    chess->size--;
}

// Check if the game is over
int isGameOver(Chess* chess, int player) {
    for (int i = 0; i < chess->size; i++) {
        int x = chess->cord[i].x;
        int y = chess->cord[i].y;

        // Horizontal check
        int count = 1;
        for (int dx = 1; dx < 5; dx++) {
            if (x + dx < BOARD_SIZE && chess->board[x + dx][y] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;

        // Vertical check
        count = 1;
        for (int dy = 1; dy < 5; dy++) {
            if (y + dy < BOARD_SIZE && chess->board[x][y + dy] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;

        // Diagonal check (/)
        count = 1;
        for (int d = 1; d < 5; d++) {
            if (x + d < BOARD_SIZE && y + d < BOARD_SIZE && chess->board[x + d][y + d] == player) {
                count++;
            } else {
                break;
            }
        }
        if (count >= 5) return 1;

        // Anti-diagonal check (\)
        count = 1;
        for (int d = 1; d < 5; d++) {
            if (x + d < BOARD_SIZE && y - d >= 0 && chess->board[x + d][y - d] == player) {
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
    int score = 0;
    int opponent = 1 - player;

    // Central control
    int centerControl = 0;
    for (int i = 8; i <= 15; i++) {
        for (int j = 8; j <= 15; j++) {
            if (chess->board[i][j] == player) {
                centerControl++;
            } else if (chess->board[i][j] == opponent) {
                centerControl--;
            }
        }
    }
    score += centerControl * 20;

    // Line checking and scoring
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (chess->board[i][j] == player) {
                // Horizontal
                int count = 0;
                for (int dx = 0; dx < 5 && i + dx < BOARD_SIZE; dx++) {
                    if (chess->board[i + dx][j] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;

                // Vertical
                count = 0;
                for (int dy = 0; dy < 5 && j + dy < BOARD_SIZE; dy++) {
                    if (chess->board[i][j + dy] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;

                // Diagonal
                count = 0;
                for (int d = 0; d < 5 && i + d < BOARD_SIZE && j + d < BOARD_SIZE; d++) {
                    if (chess->board[i + d][j + d] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;

                // Anti-diagonal
                count = 0;
                for (int d = 0; d < 5 && i + d < BOARD_SIZE && j - d >= 0; d++) {
                    if (chess->board[i + d][j - d] == player) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count >= 5) score += 100;
            }
        }
    }

    return score;
}

// Minimax algorithm
int minimax(Chess* chess, int depth, int maximizingPlayer, int player, int* bestX, int* bestY) {
    int gameState = isGameOver(chess, player);
    if (depth == 0 || gameState != 0) {
        return evaluateBoard(chess, player);
    }

    int bestScore;
    if (maximizingPlayer) {
        bestScore = INT_MIN;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (chess->board[i][j] == -1) {
                    setXY(chess, i, j, player);
                    int score = minimax(chess, depth - 1, 0, 1 - player, NULL, NULL);
                    unsetXY(chess, i, j);
                    if (score > bestScore) {
                        bestScore = score;
                        if (bestX != NULL && bestY != NULL) {
                            *bestX = i;
                            *bestY = j;
                        }
                    }
                }
            }
        }
    } else {
        bestScore = INT_MAX;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (chess->board[i][j] == -1) {
                    setXY(chess, i, j, player);
                    int score = minimax(chess, depth - 1, 1, 1 - player, NULL, NULL);
                    unsetXY(chess, i, j);
                    if (score < bestScore) {
                        bestScore = score;
                        if (bestX != NULL && bestY != NULL) {
                            *bestX = i;
                            *bestY = j;
                        }
                    }
                }
            }
        }
    }

    return bestScore;
}

// Write the next move on the chess board
void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    if (chess->size == 0) {
        // Place the first piece in the center
        *x = BOARD_SIZE / 2;
        *y = BOARD_SIZE / 2;
    } else {
        // Use minimax to determine the best move
        int bestX, bestY;
        minimax(chess, 3, 1, player, &bestX, &bestY);
        *x = bestX;
        *y = bestY;
    }

    // Place the piece on the board
    setXY(chess, *x, *y, player);
}

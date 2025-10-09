#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <vector>


const int ROWS = 6;
const int COLS = 7;
const int FULL = 42;

std::array<char, 42> create_empty_game_state() {
    std::array<char, FULL> board = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    return board;         
}


void print_state(std::array<char, 42>& board) {
    int length = sizeof(board) / sizeof(board[0]);
    for (int i = 0; i < length; i++) {
        if (i % COLS == 0) {
            std::cout << std::endl;
        }
        std::cout << "\'" << board[i] << "\' | ";
    }
    return;
}


std::vector<int> generate_possible_moves(std::array<char, 42>& board) {
    std::vector<int> moves;
    for (int i = 0; i < 7; i++) {
        if (board[i] == ' ') {
            moves.push_back(i);
        }
    }
    return moves;
}


std::array<char, 42> add_piece(const std::array<char, 42>& board, int column, char piece) {
    std::array<char, 42> new_state = board;
    for (int i = 5; i >= 0; i--) {
        if (new_state[column + i*7] == ' ') {
            new_state[column + i*7] = piece;
            break;
        }
    }
    return new_state;
}

int check_game_state(const std::array<char, 42>& board) {
    // Check all cells as potential starting points
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            char piece = board[row * COLS + col];
            if (piece == ' ') continue; // empty cell, skip

            // --- Check horizontally (→)
            if (col + 3 < COLS &&
                piece == board[row * COLS + col + 1] &&
                piece == board[row * COLS + col + 2] &&
                piece == board[row * COLS + col + 3]) {
                return 1000;
            }

            // --- Check vertically (↓)
            if (row + 3 < ROWS &&
                piece == board[(row + 1) * COLS + col] &&
                piece == board[(row + 2) * COLS + col] &&
                piece == board[(row + 3) * COLS + col]) {
                return 1000;
            }

            // --- Check diagonal down-right (↘)
            if (row + 3 < ROWS && col + 3 < COLS &&
                piece == board[(row + 1) * COLS + col + 1] &&
                piece == board[(row + 2) * COLS + col + 2] &&
                piece == board[(row + 3) * COLS + col + 3]) {
                return 1000;
            }

            // --- Check diagonal down-left (↙)
            if (row + 3 < ROWS && col - 3 >= 0 &&
                piece == board[(row + 1) * COLS + col - 1] &&
                piece == board[(row + 2) * COLS + col - 2] &&
                piece == board[(row + 3) * COLS + col - 3]) {
                return 1000;
            }
        }
    }

    // --- Check for draw (no empty spaces)
    bool full = true;
    for (char c : board) {
        if (c == ' ') {
            full = false;
            break;
        }
    }

    if (full)
        return 10; // Draw

    return 0; // Game still ongoing
}


int main() {
    std::array<char, 42> empty_board = create_empty_game_state();
    //print_state(empty_board);
    //std::vector<int> moves = generate_possible_moves(empty_board);
    //std::array<char, 42> new_state = add_piece(empty_board, 6, 'R');
    //print_state(new_state);
    int result = check_game_state(empty_board);
    std::cout << "Result: " << result;
    return 0;
}


/*
std::cout << "Moves: ";
for (auto move : moves) {
    std::cout << move << ", ";
}

{' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                     ' ', ' ', ' ', ' ', ' ', ' ', ' '};

'B', 'B', 'B', 'B', 'B', 'B', 'B', 
                                    'B', 'B', 'B', 'B', 'B', 'B', 'B', 
                                    'B', 'B', 'B', 'B', 'B', 'B', 'B', 
                                    'B', 'B', 'B', 'B', 'B', 'B', 'B', 
                                    'B', 'B', 'B', 'B', 'B', 'B', 'B', 
                                    'B', 'B', 'B', 'B', 'B', 'B', 'B
*/
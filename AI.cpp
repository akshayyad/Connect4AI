#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include <cstdint>
#include <bitset>
#include <algorithm>
#include <bit>


const int ROWS = 6;
const int COLS = 7;
const int FULL = 42;
const uint64_t DRAW_MASK = 279258638311359;
const uint64_t TOP_MASK = 141845657554976;
const uint64_t COL_MASKS[7] = {63, 8064, 1032192, 132120576, 
                               16911433728, 2164663517184, 277076930199552};
constexpr uint64_t bottom_mask(int col) {
    return 1ULL << (col * 7);
}
constexpr uint64_t column_mask(int col) {
    return 0x3FULL << (col * 7);
}


struct Board {
    uint64_t player;
    uint64_t entireBoard;
};


std::array<char, 42> create_empty_game_state() {
    std::array<char, FULL> board = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    return board;         
}


std::array<char, 42> create_game_state() {
    std::array<char, FULL> board = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', 'P', ' ', ' ', ' ', 
                                    ' ', ' ', ' ', 'P', ' ', ' ', ' ', 
                                    'P', ' ', ' ', 'P', ' ', ' ', ' ', 
                                    'P', ' ', ' ', 'P', ' ', ' ', 'P', 
                                    'P', 'P', ' ', 'P', ' ', 'P', 'P'};
    return board;         
}


void print_bits_in_uint(uint64_t bitboard) {
    std::cout << "Bitboard (LSB to MSB): ";
    for (int i = 48; i >= 0; --i) {  // only print 49 bits
        uint64_t mask = 1ULL << i;
        std::cout << ((bitboard & mask) ? '1' : '0');
        if (i % 7 == 0) std::cout << " ";
    }
    std::cout << "\n";
}


uint64_t get_draw_num() {
    uint64_t drawNum = 0ULL;
    for (int i = 0; i < 49; i++) {
        if ((i+1) % 7 == 0) continue;
        drawNum |= (1ULL << i);
    }

    std::cout << "Bitboard (LSB to MSB): ";
    for (int i = 48; i >= 0; --i) {  // only print 49 bits
        uint64_t mask = 1ULL << i;
        std::cout << ((drawNum & mask) ? '1' : '0');
        if (i % 7 == 0) std::cout << " ";
    }
    std::cout << "\n";

    return drawNum;
}


uint64_t get_top_mask() {
    uint64_t topMask = 0ULL;
    for (int i = 0; i < 7; i++) {
        uint64_t m = 1ULL << ((7 * i) + 5);
        topMask |= m;
    }
    std::cout << "Top Mask Number: " << topMask << std::endl;
    print_bits_in_uint(topMask);

    return topMask;
}


uint64_t get_column_masks() {
    // uint64_t masks[7];
    // uint64_t colMask = 63;
    
    // for (int i = 0; i < 7; i++) {
    //     uint64_t m = colMask << 7*i;
    //     masks[i] = m;
    // }
    for (auto mask : COL_MASKS) {
        print_bits_in_uint(mask);
        std::cout << "ColMask: " << mask << "\n\n";
    }
    
    return 0;
}


uint64_t create_empty_bitboard() {
    uint64_t empty = 26;
    //std::cout << empty << std::endl;
    const int numBits = std::numeric_limits<uint64_t>::digits;

    std::cout << "Binary representation of " << empty << ": ";
    for (int i = numBits - 1; i >= 0; --i) {
        // Create a mask with a single '1' at the current bit position
        unsigned int mask = (1ULL << i);
        // Check if the bit is set
        if ((empty & mask) != 0) {
            std::cout << "1";
        } else {
            std::cout << "0";
        }
        // Optional: Add a space every 7 bits for readability
        if (i > 0 && i % 7 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    return empty;
}


uint64_t convert_to_bitboard(std::array<char, 42> board, char player) {
    int bitposition = 0;
    std::vector<int> bits;
    for (int i = 0; i < COLS; i++) {
        for (int j = ROWS-1; j >= 0; j--) {
            int position = (7 * j) + i;
            if (board[position] == player) {
                bits.push_back(1);
            } else {
                bits.push_back(0);
            }
        }
        bits.push_back(0);
    }
    //std::reverse(bits.begin(), bits.end());
    
    // std::cout << "Printing the convert_to_bitboard() result: " << std::endl;
    // for (auto bit : bits) {
    //     std::cout << bit;
    // }
    // std::cout << std::endl;

    uint64_t bitboard = 0ULL;
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i]) {
            bitboard |= (1ULL << i);
        }
    }

    // Used for printing the bit values from the uint itself
    // std::cout << "Bitboard (LSB to MSB): ";
    // for (int i = 48; i >= 0; --i) {  // only print 49 bits
    //     uint64_t mask = 1ULL << i;
    //     std::cout << ((bitboard & mask) ? '1' : '0');
    //     if (i % 7 == 0) std::cout << " ";
    // }
    // std::cout << "\n";
    
    return bitboard;
}


void print_bitboard(uint64_t bitboard) {
    char representation[6][7];
    std::vector<int> bits;
    for (int i = 0; i < 49; i++) {
        uint64_t mask = 1ULL << i;
        bits.push_back((bitboard & mask) ? 1 : 0);
    }
    //std::cout << std::endl;
    /*for (auto bit : bits) {
        std::cout << bit;
    }*/
    std::reverse(bits.begin(), bits.end());
    for (int i = 0; i < COLS; i++) {
        for (int j = ROWS-1; j >= 0; j--) {
            representation[j][i] = ((bits.back() == 1) ? 'P' : ' ');
            bits.pop_back();
        }
        bits.pop_back();
    }

    // Print Representation
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << '\'' << representation[i][j] << "\', ";
        }
        std::cout << std::endl;
    }
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


int check_draw(uint64_t bitboard) {
    return (bitboard & DRAW_MASK) == DRAW_MASK;
}


int check_win(uint64_t bitboard) {
    // Check for Horizontal Win
    uint64_t m = bitboard & (bitboard << 7);
    if ( (m & (m << 14)) != 0) return 1;

    // Check for Vertical Win
    m = bitboard & (bitboard << 1);
    if ( (m & (m << 2)) != 0) return 1;

    // Check for Up-Right Win
    m = bitboard & (bitboard << 8);
    if ( (m & (m << 16)) != 0) return 1;

    // Check for Down-Right Win
    m = bitboard & (bitboard << 6);
    if ( (m & (m << 12)) != 0) return 1;

    return 0;
}


std::vector<int> get_available_moves(uint64_t bitboard) {
    std::vector<int> moves;
    moves.reserve(7);
    
    uint64_t open = ~bitboard & TOP_MASK;
    while (open) {
        int bit_index = std::countr_zero(open); // position of least significant 1
        int col = bit_index / 7;                // each column has 7 bits
        moves.push_back(col);
        open &= (open - 1);                     // clear the lowest set bit
    }
    return moves;
}


inline uint64_t add_piece(uint64_t bitboard, int col) {
    return bitboard | ((bitboard & column_mask(col)) + bottom_mask(col));
}


void shift_testing(uint64_t bitboard) {
    // Left Shift 7 is to the right
    // Left Shift 1 is up
    // Left Shift 8 is Up-Right
    // Left Shift 6 is Down-Right
    print_bits_in_uint(bitboard);
    print_bitboard(bitboard);
    uint64_t firstShift = bitboard >> 1;
    print_bits_in_uint(firstShift);
    print_bitboard(firstShift);

    // uint64_t result = firstShift & (firstShift << 2);
    // print_bits_in_uint(result);
    // print_bitboard(result);
}


int main() {
    std::array<char, 42> board = create_game_state();
    //print_state(empty_board);
    //std::vector<int> moves = generate_possible_moves(empty_board);
    //std::array<char, 42> new_state = add_piece(empty_board, 6, 'R');
    //print_state(new_state);
    //int result = check_game_state(empty_board);
    //std::cout << "Result: " << result;

    //uint64_t emp = create_empty_bitboard();
    //print_bitboard(emp);
    uint64_t bitboard = convert_to_bitboard(board, 'P');
    // std::cout << std::endl;

    //shift_testing(bitboard);
    // print_bitboard(bitboard);
    // get_available_moves(bitboard);

    uint64_t edited = add_piece(bitboard, 4);

    //get_column_masks();
    // std::cout << bottom_mask(1) << "\n";
    // std::cout << column_mask(1) << "\n";

    
    // std::cout << "Result: " << check_win(bitboard) << std::endl;
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
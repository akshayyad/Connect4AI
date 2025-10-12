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

constexpr uint64_t bottom_mask(int col) {
    return 1ULL << (col * 7);
}
constexpr uint64_t column_mask(int col) {
    return 0x3FULL << (col * 7);
}


struct Board {
    uint64_t player;
    uint64_t combined;
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

    uint64_t bitboard = 0ULL;
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i]) {
            bitboard |= (1ULL << i);
        }
    }
    
    return bitboard;
}


void print_bitboard(uint64_t bitboard) {
    char representation[6][7];
    std::vector<int> bits;
    for (int i = 0; i < 49; i++) {
        uint64_t mask = 1ULL << i;
        bits.push_back((bitboard & mask) ? 1 : 0);
    }

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


int main() {
    std::array<char, 42> board = create_game_state();

    uint64_t bitboard = convert_to_bitboard(board, 'P');

    uint64_t edited = add_piece(bitboard, 4);

    return 0;
}

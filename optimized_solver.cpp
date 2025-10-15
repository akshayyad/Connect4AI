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
#include <unordered_map>
#include <chrono>
#include <utility>


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
    bool operator==(const Board &other) const noexcept { 
        return player == other.player && combined == other.combined;
    }
};


template<>
struct std::hash<Board>
{
    std::size_t operator()(const Board& board) const noexcept
    {
        std::size_t h1 = std::hash<uint64_t>{}(board.player);
        std::size_t h2 = std::hash<uint64_t>{}(board.combined);
        return h1 ^ h2;
    }
};


std::unordered_map<Board, int> seen_states;


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


Board create_empty_board() {
    Board emp;
    emp.player = 0ULL;
    emp.combined = 0ULL;
    return emp;
}


Board create_board_state() {
    std::array<char, FULL> boardcombined = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                            ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                            ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                            ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                            'P', ' ', ' ', ' ', ' ', ' ', ' ', 
                                            'P', ' ', ' ', ' ', 'P', 'P', ' '};

    std::array<char, FULL> boardplayer = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                          ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                          ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                          ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                          ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
                                          ' ', ' ', ' ', ' ', 'P', 'P', ' '};
    uint64_t combined = convert_to_bitboard(boardcombined, 'P');
    //print_bits_in_uint(combined);
    uint64_t player = convert_to_bitboard(boardplayer, 'P');
    //print_bits_in_uint(player);
    Board sample;
    sample.combined = combined;
    sample.player = player;
    return sample;
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


void print_both_sides(Board &b, char playerChar) {
    char enemyChar = (playerChar == 'R') ? 'Y' : 'R';
    uint64_t enemyPieces = b.player ^ b.combined;

    char display[6][7];
    // Fill all cells as empty
    for (int r = 0; r < 6; ++r)
        for (int c = 0; c < 7; ++c)
            display[r][c] = ' ';

    // Fill player pieces
    for (int c = 0; c < 7; ++c) {
        for (int r = 0; r < 6; ++r) {
            int bitIndex = c * 7 + r;
            if (b.player & (1ULL << bitIndex))
                display[5 - r][c] = playerChar; // invert row to print top first
        }
    }

    // Fill enemy pieces
    for (int c = 0; c < 7; ++c) {
        for (int r = 0; r < 6; ++r) {
            int bitIndex = c * 7 + r;
            if (enemyPieces & (1ULL << bitIndex))
                display[5 - r][c] = enemyChar;
        }
    }

    // Print Representation
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << '\'' << display[i][j] << "\', ";
        }
        std::cout << std::endl;
    }
}


void print_board_indices() {
    int board[6][7]; 
    int iter = 0;
    for (int i = 0; i < COLS; i++) {
        for (int j = ROWS-1; j >= 0; j--) {
            board[j][i] = iter;
            iter++;
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            std::cout << '\'' << board[i][j] << "\', ";
        }
        std::cout << std::endl;
    }
}


inline int check_draw(Board board) {
    return (board.combined & DRAW_MASK) == DRAW_MASK;
}


inline int check_win(uint64_t player) {
    // Check for Horizontal Win
    uint64_t m = player & (player << 7);
    if ( (m & (m << 14)) != 0) return 1;

    // Check for Vertical Win
    m = player & (player << 1);
    if ( (m & (m << 2)) != 0) return 1;

    // Check for Up-Right Win
    m = player & (player << 8);
    if ( (m & (m << 16)) != 0) return 1;

    // Check for Down-Right Win
    m = player & (player << 6);
    if ( (m & (m << 12)) != 0) return 1;

    return 0;
}


inline int get_available_moves(Board bitboard, int *moves) {
    uint64_t open = ~bitboard.combined & TOP_MASK;
    int count = 0;
    while (open) {
        int bit = std::countr_zero(open);
        moves[count++] = bit / 7;
        open &= open - 1;
    }
    return count;
}


inline void add_piece(Board &bitboard, int col) {
    uint64_t move = ((bitboard.combined & column_mask(col)) + bottom_mask(col));
    //std::cout << "Mask: " << mask << std::endl;
    bitboard.combined |= move;
    bitboard.player |= move;
}


inline void undo_move(Board &b, int col) {
    uint64_t colbits = b.combined & column_mask(col);
    if (colbits == 0) return;                          // nothing to undo
    // position of most significant 1 in column bits
    int msb = 63 - __builtin_clzll(colbits);
    uint64_t move_mask = 1ULL << msb;
    b.combined &= ~move_mask;
    // ensure player mask is consistent (clear any piece that was removed)
    b.player &= b.combined;
}


void swap_sides(Board &board) {
    board.player ^= board.combined;
}


void print_board_components(Board board) {
    std::cout << "Player: \n";
    print_bitboard(board.player);
    std::cout << "Combined: \n";
    print_bitboard(board.combined);
    std::cout << std::endl;
}


void print_board(Board board) {
    print_bitboard(board.combined);
}


void print_seen_states() {
    for (auto state: seen_states) {
        std::cout << "Player: " << state.first.player << " " 
                  << "Combined: " << state.first.combined << std::endl;
    }
}

// Focus First on just Printing out each Board State, forget about returning the correct scores
int minimaz(Board &board, int depth, int alpha, int beta, bool is_maximizing) {

    if (check_win(board.player)) return 1000-depth;
    if (check_win(board.player^board.combined)) return -1000+depth;
    if (check_draw(board)) return 5;
    if (depth >= 6) return 0;

    int moves[7];
    int count = get_available_moves(board, moves);
    if (is_maximizing) {
        int best = -10000;
        for (int i = 0; i < count; i++) {
            add_piece(board, moves[i]);
            
        }
    }

    
    //if (depth == 0) std::cout << "Best Move Found: " << best_move << '\n';
    return 0;
}


int minimax(Board &board, int depth, int alpha, int beta, bool is_maximizing_player) {
    if (check_win(board.player)) return 10000000 - depth;
    if (check_win(board.player ^ board.combined)) return -10000000 + depth;
    if (check_draw(board) || depth >= 13) return 0;

    auto it = seen_states.find(board);
    if (it != seen_states.end()) return it->second;

    int best_score = is_maximizing_player ? -1000000 : 1000000;
    int moves[7];
    int count = get_available_moves(board, moves);

    for (int i = 0; i < count; i++) {
        Board newBoard = board;
        add_piece(newBoard, moves[i]);
        newBoard.player ^= newBoard.combined; // switch player

        int score = minimax(newBoard, depth + 1, alpha, beta, !is_maximizing_player);

        if (is_maximizing_player) {
            best_score = std::max(best_score, score);
            alpha = std::max(alpha, score);
        } else {
            best_score = std::min(best_score, score);
            beta = std::min(beta, score);
        }
        if (beta <= alpha) break; // alpha-beta pruning
    }

    seen_states[board] = best_score;
    return best_score;

}


int get_AI_move(Board b) {
    seen_states.clear();
    int best_val = -1000000;
    int best_move = -1;
    int moves[7];
    int count = get_available_moves(b, moves);
    for (int i = 0; i < count; ++i) {
        Board newBoard = b;
        add_piece(newBoard, moves[i]);

        // immediate win -> take it
        if (check_win(newBoard.player)) return moves[i];

        // evaluate position from opponent's perspective
        newBoard.player ^= newBoard.combined;
        int move_val = minimax(newBoard, 0, -1000000, 1000000, false);

        if (move_val > best_val) {
            best_val = move_val;
            best_move = moves[i];
        }
    }
    return best_move;
}


int game_manager_check_win(Board b) {
    return (check_win(b.player) || check_win(b.player ^ b.combined));
}


void game_manager() {
    int playerStartingPlace;
    char playerColor;

    std::cout << "Welcome to Connect 4!\nPlease enter if you'd like to go first(1) or second(2): ";
    std::cin >> playerStartingPlace;
    std::cout << std::endl;
    
    std::cout << "Please Enter the Color You'd like to Play with. Yellow (Y) or Red (R): ";
    std::cin >> playerColor;
    std::cout << std::endl;

    int AIStartingPlace = (playerStartingPlace == 1) ? 2 : 1;
    char AIColor = (playerColor == 'Y') ? 'R' : 'Y';
    //Board board = create_empty_board();
    Board board = create_board_state();

    int playerTurn = (playerStartingPlace == 1) ? true : false;

    while (!game_manager_check_win(board)) {
        if (playerTurn) {
            int playerMove;
            std::cout << "Enter Your Move (1-7): ";
            std::cin >> playerMove;
            if (playerMove == -1) {break;}
            // Add Move to the Board
            board.player ^= board.combined;
            add_piece(board, playerMove-1);
            board.player ^= board.combined;
            print_both_sides(board, AIColor);
            std::cout << std::endl;
        } else {
            std::cout << "\nAI is Searching for a Move...\n";
            auto start = std::chrono::high_resolution_clock::now();
            //int AIMove = get_AI_move(board);
            seen_states.clear();
            int AIMove = minimaz(board, 0, -1000, 1000, true);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "AI Found Move in " 
                      << std::chrono::duration_cast<std::chrono::seconds>(end-start).count()  << " seconds\n";
            //add_piece(board, AIMove);
            add_piece(board, AIMove);
            print_both_sides(board, AIColor);
            //std::cout << "AI Placed a Piece on Column: " << AIMove+1 << "\n";
            std::cout << "AI Placed a Piece on Column: " << AIMove+1 << "\n";
            std::cout << std::endl;
        }
        playerTurn = !playerTurn;
    }
    if (check_win(board.player)) {
        std::cout << "Game Over AI Wins!\n\n";
    } else if (check_win(board.player^=board.combined)) {
        std::cout << "Congratulations for Besting the AI!\n\n";
    } else {
        std::cout << std::endl;
    }
    
}


int main() { 
    //game_manager();
    //Board state = create_empty_board();
    // seen_states.clear();
    // minimaz(state, 0, -1000000, 1000000, true);

    //Board state = create_board_state();
    
    // print_both_sides(state, 'Y');
    // std::cout << '\n';
    // print_bits_in_uint(state.player);
    // swap_sides(state);
    // print_bits_in_uint(state.player);
    // print_both_sides(state, 'Y');
    return 0;
}


// Focus First on just Printing out each Board State, forget about returning the correct scores
// std::pair<int, int> minimaz(Board &board, int depth, int alpha, int beta, bool is_maximizing) {

//     if (check_win(board.player^board.combined)) return {1000-depth, 0};
//     if (check_win(board.player)) return {-1000+depth, 0};
//     if (check_draw(board)) return {5, 0};
//     if (depth >= 6) return {0, 0};

//     int best_score = is_maximizing ? -1000 : 1000;
//     int moves[7];
//     int count = get_available_moves(board, moves);
//     int best_move = moves[0];

//     for (int i = 0; i < count; i++) {
//         add_piece(board, moves[i]);
//         if (check_win(board.player)) { undo_move(board, moves[i]); return {1000-depth, moves[i]}; }
//         // Print out the Move Made
//         //std::cout << "Move at Depth " << depth << ": " << moves[i] << '\n';
//         //print_both_sides(board, 'Y');
//         //std::cout << std::endl;
//         board.player ^= board.combined;
//         std::pair<int, int> results = minimaz(board, depth+1, alpha, beta, !is_maximizing);
//         int score = results.first;
//         //std::cout << "Score in this Position: " << score << "\n\n";
//         board.player ^= board.combined;
//         undo_move(board, moves[i]);

//         if (is_maximizing) {
//             if (best_score < score) {
//                 best_score = score;
//                 best_move = moves[i];
//             }
//             //alpha = std::max(alpha, score);
//         } else {
//             if (best_score > score) {
//                 best_score = score;
//                 best_move = moves[i];
//             }
//             //beta = std::min(beta, score);
//         }
//         //if (beta <= alpha) break; // alpha-beta pruning
//     }
//     //if (depth == 0) std::cout << "Best Move Found: " << best_move << '\n';
//     return {best_score, best_move};
// }

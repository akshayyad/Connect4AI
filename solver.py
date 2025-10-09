import pandas as pd
import math
import time


import numpy as np

ROWS = 6
COLS = 7

def evaluate_window(window, piece, opp_piece):
    """Score a 4-cell window for its value."""
    score = 0
    piece_count = window.count(piece)
    opp_count = window.count(opp_piece)
    empty_count = window.count(" ")

    if piece_count == 4:
        score += 100000
    elif piece_count == 3 and empty_count == 1:
        score += 100
    elif piece_count == 2 and empty_count == 2:
        score += 10

    if opp_count == 3 and empty_count == 1:
        score -= 80
    elif opp_count == 2 and empty_count == 2:
        score -= 5

    return score


def evaluate_board(board, ai_piece="Y", opp_piece="R"):
    """Evaluate the board for heuristic strength."""
    score = 0
    b = np.array(board)

    # Center column preference
    center_col = [b[r][COLS // 2] for r in range(ROWS)]
    score += center_col.count(ai_piece) * 3

    # Horizontal
    for r in range(ROWS):
        row_array = list(b[r, :])
        for c in range(COLS - 3):
            window = row_array[c:c+4]
            score += evaluate_window(window, ai_piece, opp_piece)

    # Vertical
    for c in range(COLS):
        col_array = list(b[:, c])
        for r in range(ROWS - 3):
            window = col_array[r:r+4]
            score += evaluate_window(window, ai_piece, opp_piece)

    # Diagonals
    for r in range(ROWS - 3):
        for c in range(COLS - 3):
            # Positive slope
            window = [b[r+i][c+i] for i in range(4)]
            score += evaluate_window(window, ai_piece, opp_piece)
            # Negative slope
            window = [b[r+3-i][c+i] for i in range(4)]
            score += evaluate_window(window, ai_piece, opp_piece)

    return score


def create_empty_game_state():
    return (
        (" ", " ", " ", " ", " ", " ", " "), 
        (" ", " ", " ", " ", " ", " ", " "),
        (" ", " ", " ", " ", " ", " ", " "),
        (" ", " ", " ", " ", " ", " ", " "),
        (" ", " ", " ", " ", " ", " ", " "),
        (" ", " ", " ", " ", " ", " ", " ")
    )


def check_state_draw(state):
    for r in range(6):
        for c in range(7):
            if state[r][c] == ' ':
                return False
    return True


def check_possible_move(currState, column):
    # Check Valid Column Input
    if column >= len(currState[0]):
        return False
    # Check if column is already full, then add the piece
    row = 0
    if currState[0][column] != " ":
        #print(f"Column Full")
        return False
    return True


def check_state_win(state):
    # Check if there is a 4 in a row in any orientation for Yellow or Red
    ROWS, COLS = 6, 7
    directions = [(0,1), (1,0), (1,1), (1,-1)]  # right, down, down-right, down-left
    
    for r in range(ROWS):
        for c in range(COLS):
            player = state[r][c]
            if player == " ":
                continue

            for dr, dc in directions:
                count = 1
                for k in range(1, 4):
                    nr, nc = r + dr*k, c + dc*k
                    if 0 <= nr < ROWS and 0 <= nc < COLS and state[nr][nc] == player:
                        count += 1
                    else:
                        break
                if count == 4:
                    return 1 if player == 'Y' else 2
    return 0


def add_piece(color, column, currState):
    # Check Valid Column Input
    if column >= len(currState[0]):
        return 0
    # Check if column is already full, then add the piece
    row = 0
    if currState[0][column] != " ":
        #print(f"Column Full")
        return 0
    else:
        # Find row for available spot
        if currState[5][column] == " ":
            row = 5
        else:
            for i in range(1, 6):
                if currState[i][column] != " ":
                    row = i - 1
                    break
    new_row = currState[row][0:column] + (color,) + currState[row][column+1:]
    new_state = currState[0:row] + (new_row,) + currState[row+1:]
    return new_state


def generate_possible_moves(gameState):
    moves = []
    for col in range(7):
        if gameState[0][col] != ' ':
            continue
        else:
            for row in range(5, -1, -1):
                if gameState[row][col] == ' ':
                    moves.append(col)
                    break
    return moves


def count_empty_spots(gameState):
    count = 0
    for col in range(7):
        for row in range(6):
            if gameState[row][col] == ' ':
                count += 1
            else:
                break
    return count


def assign_score(gameState, ):
    pass


def print_state(state):
    for i in range(6):
        print(state[i])

saved_states = {}


# empty = create_empty_game_state()
# generate_possible_moves(empty)


def minimax(state, depth, alpha, beta, is_maximizing_player):
    if check_state_win(state) == 1:
        return 1000000 - depth  # big reward for fast win
    elif check_state_win(state) == 2:
        return -1000000 + depth  # big penalty for losing
    elif check_state_draw(state):
        return 0
    elif state in saved_states:
        return saved_states[state]
    if depth == 8:
        return evaluate_board(state, ai_piece='Y', opp_piece='R')
     
    if is_maximizing_player:
            best_score = -math.inf
            possible_moves = generate_possible_moves(state)
            while len(possible_moves) > 0:
                move = possible_moves.pop(len(possible_moves)//2)
                new_state = add_piece('Y', move, state)
                score = minimax(new_state, depth+1, alpha, beta, False)
                best_score = max(best_score, score)
                alpha = max(alpha, score)
                if beta <= alpha:
                    break
            saved_states[state] = best_score
            return best_score
    else:
            best_score = math.inf
            enemy_possible_moves = generate_possible_moves(state)
            while len(enemy_possible_moves) > 0:
                move = enemy_possible_moves.pop(len(enemy_possible_moves)//2)
                next_state = add_piece('R', move, state)
                score = minimax(next_state, depth+1, alpha, beta, True)
                best_score = min(best_score, score)
                beta = min(beta, score)
                if beta <= alpha:
                    break
            saved_states[state] = best_score
            return best_score


# Assume AI is Yellow and Goes First
def findBestMove(state):
    saved_states.clear()
    best_val = -math.inf
    best_move = -1
    possible_moves = generate_possible_moves(state)
    while len(possible_moves) > 0:
        move = possible_moves.pop(len(possible_moves)//2)
        new_state = add_piece('Y', move, state)
        move_val = minimax(new_state, 0, -math.inf, math.inf, False)

        if move_val > best_val:
            best_val = move_val
            best_move = move
    
    return best_move

    
def main():
    state = create_empty_game_state()
    quit = False
    while not (check_state_win(state) != 0) or (check_state_draw(state)):
        # Computer's Turn
        start_time = time.perf_counter()
        ai_move = findBestMove(state)
        end_time = time.perf_counter()
        elapsed_time = end_time - start_time
        print(f"AI Found Move in {elapsed_time:.6f} seconds")
        state = add_piece('Y', ai_move, state)
        print_state(state)
        print(f"Adding move to column: {ai_move+1}")
        if check_state_win(state) == 1:
            print("AI Wins")
            break
        elif check_state_draw(state):
            print("Game ends in Draw")
            break

        # Human's Turn
        while True:
            try:
                user_move = int(input("Enter Column Value (1-7) To Place Piece Into: "))
                if user_move == 0:
                    print("Thank you for playing\n")
                    quit = True
                    break
                move_result = add_piece('R', user_move-1, state)
                if move_result == 0:
                    print("Invalid move. Try again")
                else:
                    state = move_result
                    break
            except ValueError:
                print("Invalid input. Please enter numbers.")
        if quit:
            break
        print_state(state)
        if check_state_win(state) == 2:
            print("Human Wins")
            break
        elif check_state_draw(state):
            print("Game ends in Draw")
            break


if __name__ == "__main__":
    main()
    #pass

# empty = create_empty_game_state()
# print(count_empty_spots(empty))


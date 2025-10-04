import math

# Represents the Tic-Tac-Toe board
board = [[' ', ' ', ' '],
         [' ', ' ', ' '],
         [' ', ' ', ' ']]

PLAYER_X = 'X'
PLAYER_O = 'O'
EMPTY = ' '

def print_board(board):
    """Prints the current state of the board."""
    for row in board:
        print("|".join(row))
        print("-" * 5)


def check_win(board, player):
    """Checks if the given player has won."""
    # Check rows
    for row in board:
        if all(cell == player for cell in row):
            return True
    # Check columns
    for col in range(3):
        if all(board[row][col] == player for row in range(3)):
            return True
    # Check diagonals
    if (board[0][0] == player and board[1][1] == player and board[2][2] == player) or \
       (board[0][2] == player and board[1][1] == player and board[2][0] == player):
        return True
    return False


def is_draw(board):
    """Checks if the game is a draw."""
    return all(cell != EMPTY for row in board for cell in row) and not (check_win(board, PLAYER_X) or check_win(board, PLAYER_O))


def minimax(board, depth, is_maximizing_player):
    """
    The Minimax algorithm to find the optimal move.
    Returns the score for the current board state.
    """
    if check_win(board, PLAYER_X):
        return 10 - depth  # X wins
    elif check_win(board, PLAYER_O):
        return depth - 10  # O wins
    elif is_draw(board):
        return 0  # Draw

    if is_maximizing_player:  # AI's turn (PLAYER_X)
        best_score = -math.inf
        for r in range(3):
            for c in range(3):
                if board[r][c] == EMPTY:
                    board[r][c] = PLAYER_X
                    score = minimax(board, depth + 1, False)
                    board[r][c] = EMPTY  # Undo the move
                    best_score = max(best_score, score)
        return best_score
    else:  # Opponent's turn (PLAYER_O)
        best_score = math.inf
        for r in range(3):
            for c in range(3):
                if board[r][c] == EMPTY:
                    board[r][c] = PLAYER_O
                    score = minimax(board, depth + 1, True)
                    board[r][c] = EMPTY  # Undo the move
                    best_score = min(best_score, score)
        return best_score


def find_best_move(board):
    """Finds the best move for the AI (PLAYER_X) using Minimax."""
    best_val = -math.inf
    best_move = (-1, -1)

    for r in range(3):
        for c in range(3):
            if board[r][c] == EMPTY:
                board[r][c] = PLAYER_X
                move_val = minimax(board, 0, False)
                board[r][c] = EMPTY  # Undo the move

                if move_val > best_val:
                    best_val = move_val
                    best_move = (r, c)
    return best_move


# Example usage:
if __name__ == "__main__":
    print("Welcome to Tic-Tac-Toe!")
    print_board(board)

    while not (check_win(board, PLAYER_X) or check_win(board, PLAYER_O) or is_draw(board)):
        # Human player's turn (O)
        while True:
            try:
                row = int(input("Enter row (0-2) for O: "))
                col = int(input("Enter column (0-2) for O: "))
                if 0 <= row <= 2 and 0 <= col <= 2 and board[row][col] == EMPTY:
                    board[row][col] = PLAYER_O
                    break
                else:
                    print("Invalid move. Try again.")
            except ValueError:
                print("Invalid input. Please enter numbers.")

        print_board(board)
        if check_win(board, PLAYER_O):
            print("Player O wins!")
            break
        if is_draw(board):
            print("It's a draw!")
            break

        # AI player's turn (X)
        print("AI (X) is making a move...")
        ai_row, ai_col = find_best_move(board)
        board[ai_row][ai_col] = PLAYER_X
        print_board(board)
        if check_win(board, PLAYER_X):
            print("AI (X) wins!")
            break
        if is_draw(board):
            print("It's a draw!")
            break
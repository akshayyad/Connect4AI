import pandas as pd


def create_empty_game_state():
    return (
        ("E", "E", "E", "R", "E", "E", "E"), 
        ("E", "E", "E", "B", "E", "E", "E"), 
        ("E", "E", "E", "R", "E", "E", "E"), 
        ("E", "E", "E", "B", "E", "E", "E"), 
        ("E", "E", "E", "R", "E", "E", "E"), 
        ("E", "E", "E", "B", "E", "E", "E")
    )


def check_state_win(state):
    # Check if there is a 4 in a row in any orientation for Blue or Red
    ROWS, COLS = 6, 7
    directions = [(0,1), (1,0), (1,1), (1,-1)]  # right, down, down-right, down-left
    
    for r in range(ROWS):
        for c in range(COLS):
            player = state[r][c]
            if player == "E":
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
                    return 1 if player == "B" else 2
    return 0


def add_piece(color, column, currState):
    # Check Valid Column Input
    if column >= len(currState[0]):
        return 0
    # Check if column is already full, then add the piece
    row = 0
    if currState[0][column] != "E":
        #print(f"Column Full")
        return 0
    else:
        # Find row for available spot
        if currState[5][column] == "E":
            row = 5
        else:
            for i in range(1, 6):
                if currState[i][column] != "E":
                    row = i - 1
                    break
    new_row = currState[row][0:column] + (color,) + currState[row][column+1:]
    new_state = currState[0:row] + (new_row,) + currState[row+1:]
    return new_state


def generate_possible_moves(gameState):
    moves = []
    for col in range(7):
        if gameState[0][col] != 'E':
            continue
        else:
            for row in range(5, -1, -1):
                if gameState[row][col] == 'E':
                    moves.append(col)
                    break
    print(moves)
    return moves


def print_state(state):
    for i in range(6):
        print(state[i])


empty = create_empty_game_state()
generate_possible_moves(empty)

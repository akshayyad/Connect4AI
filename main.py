import pandas as pd
import solver

def create_empty_game_state():
    return (
        ("E", "E", "E", "E", "E", "E", "E"), 
        ("E", "E", "E", "E", "E", "E", "E"), 
        ("E", "E", "E", "E", "E", "E", "E"), 
        ("E", "E", "E", "E", "E", "E", "E"), 
        ("E", "E", "E", "E", "E", "E", "E"), 
        ("E", "E", "E", "E", "E", "E", "E")
    )


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


def print_state(state):
    for i in range(6):
        print(state[i])


def single_player():
    pass


def two_player(first_color, second_color):
    gameState = create_empty_game_state()
    print_state(gameState)
    print()
    while (1 == 1):
        firstMove = int(input(f"Enter the Column to add a piece for {first_color} (1-7 from Left to Right): "))
        print()
        gameState = add_piece(first_color[0], firstMove-1, gameState)
        print_state(gameState)
        print()
        if check_state_win(gameState) != 0:
            print(f"Congradulations for the Win Team {first_color}!\n")
            break

        secondMove = int(input(f"Enter the Column to add a piece for {second_color} (1-7 from Left to Right): "))
        print()
        gameState = add_piece(second_color[0], secondMove-1, gameState)
        print_state(gameState)
        print()
        if check_state_win(gameState) != 0:
            print(f"Congradulations for the Win Team {second_color}!\n")
            break


def game_controller():
    gamemode = int(input("Input 1 if you want to play against the computer, or 2 for Two-Player Mode: "))
    if gamemode == 1:
        single_player()
    if gamemode == 2:
        choice = input("Input the color that will go first: ")
        secondChoice = ''
        if choice == "B":
            choice = "Blue"
            secondChoice = 'Red'
        elif choice == "R":
            choice = "Red"
            secondChoice = 'Blue'
        two_player(choice, secondChoice)


def main():
    empty = create_empty_game_state()
    # print_state(empty)
    # print()
    # new_state = add_piece("B", 5, empty)
    # print_state(new_state)

    # print(check_state_win(empty))

    print("Welcome to Connect 4")
    game_controller()
    print("Thank you for playing")
    

if __name__ == "__main__":
    main()

# ("E", "E", "E", "E", "E", "E", "E"), 
# ("E", "E", "E", "E", "E", "E", "E"), 
# ("E", "E", "E", "E", "E", "E", "E"), 
# ("E", "E", "E", "E", "E", "E", "E"), 
# ("E", "E", "E", "E", "E", "E", "E"), 
# ("E", "E", "E", "E", "E", "E", "E")
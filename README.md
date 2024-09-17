
# C++ Maze Game

## Overview

This C++ project is a text-based maze game where the player navigates through a randomly generated maze. The game involves avoiding obstacles and an enemy while trying to reach the exit and collect a key. The maze is populated with walls, a key, an exit, and an enemy that moves randomly.

## Features

- **Maze Generation**: Randomly generates walls, a key, an exit, and an enemy.
- **Player Movement**: Move the player using WASD keys.
- **Enemy Movement**: The enemy moves randomly and can catch the player.
- **Game Over Conditions**: The game ends if the player is caught by the enemy or reaches the exit after collecting the key.

## Requirements

- C++ compiler (e.g., g++)

## Compilation

To compile the program, use:

```bash
g++ -o maze maze.cpp
```

## Usage

Run the compiled executable with:

```bash
./maze
```

Follow the on-screen instructions to navigate the maze. Use the WASD keys to move:

- **W** or **w**: Move up
- **A** or **a**: Move left
- **S** or **s**: Move down
- **D** or **d**: Move right

Your goal is to obtain the key (`!`) and reach the exit (`E`) while avoiding the enemy (`M`).

## Code Overview

- **`maze.cpp`**: Contains the main logic for the game including maze generation, player and enemy movement, and input handling.

### Functions

- **`generateCoords`**: Generates random coordinates within the maze, avoiding edges.
- **`aroundClearOf`**: Checks if all adjacent cells (including diagonals) around a given coordinate are clear of a specific object.
- **`generateInnerWalls`**: Randomly generates inner walls while ensuring the player and enemy are not trapped, and the key and exit are accessible.
- **`printMaze`**: Prints the maze to the console.
- **`moveInput`**: Handles player input and updates the maze accordingly.
- **`moveM`**: Moves the enemy randomly and updates the maze; ends the game if the enemy catches the player.

## Gameplay Instructions

1. Start the game by running the executable.
2. The maze will be displayed with:
   - `@` representing the player
   - `M` representing the enemy
   - `!` representing the key
   - `E` representing the exit
3. Use WASD keys to move the player.
4. The game ends if:
   - The player reaches the exit after collecting the key.
   - The player is caught by the enemy.

## Author

Created by Conor McJannett on 7/26/22.

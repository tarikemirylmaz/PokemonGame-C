# Pokemon Game in C

A console-based Pokemon game developed in **C**.

## About the Project

This project is a Pokemon-themed console game developed to practice fundamental C programming concepts.

The game uses external text files to store Pokemon, move, and type information. These files are read and processed by the program during gameplay.

## Technologies Used

- C
- Structures (`struct`)
- File Handling
- Functions
- Pointers
- Text File Processing

## Project Structure

- `main.c` - Entry point of the program
- `game.c` - Contains the main game logic
- `initialize.c` - Handles initialization and data loading
- `structs.h` - Contains structure definitions and shared declarations
- `pokemon.txt` - Pokemon data
- `moves.txt` - Move data
- `types.txt` - Pokemon type data

## Features

- Pokemon-based game system
- Pokemon and move data loaded from external files
- Type information management
- Battle and game logic
- Modular C source code
- Custom structures for organizing game data

## How to Compile

Using GCC:

```bash
gcc main.c game.c initialize.c -o pokemon_game
```

## How to Run

On macOS/Linux:

```bash
./pokemon_game
```

On Windows:

```bash
pokemon_game.exe
```

Make sure `pokemon.txt`, `moves.txt`, and `types.txt` are in the same directory as the program when running the game.

## Author

**Tarık Emir Yılmaz**

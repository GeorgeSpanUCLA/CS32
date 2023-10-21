Kalah Game Project
Welcome to the Kalah Game Project repository! This project implements the classic board game Kalah, where players strategically sow seeds (beans) across pits to capture their opponent's seeds. The project is written in C++ and includes implementations for various player types, including a Smart Player using the Minimax algorithm.

Key Features:
Board Management: The Board class efficiently handles the game board, keeping track of holes, pots, and beans for both players.

Player Types: Supports different player types, including Human Player, Bad Player (AI with basic moves), and Smart Player (AI using Minimax algorithm).

Minimax Algorithm: The Smart Player employs a sophisticated Minimax algorithm with heuristic evaluation to make optimal moves.

Interactive Gameplay: Provides an interactive gameplay experience, allowing users to make moves through the console interface.

Customizable Game Settings: The game allows customization of the number of holes per side and initial beans per hole to adapt to different variations of Kalah.

How to Use:
Clone the repository to your local machine.
Compile the source files using a C++ compiler.
Run the executable to start playing Kalah.
Project Structure:
Board.h and Board.cpp: Define and implement the Board class responsible for managing the game board.

Player.h and Player.cpp: Define the abstract Player class and its subclasses (Human Player, Bad Player, Smart Player).

Game.h and Game.cpp: Define and implement the Game class responsible for managing the gameplay flow.

Side.h: Contains enum Side and constants related to the game's sides and pots.

Main.cpp: Contains the main routine and additional non-member functions.

Coursework from UCLA CS32.

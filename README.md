# Terminal Connect-5 🎮

A terminal-based Connect-5 game written entirely in C. Played on an 8x8 grid, this project features gravity-based piece dropping, dynamic multi-directional win checking, a basic heuristic AI opponent, and a persistent scoreboard.

## ✨ Features

* **Single Player Mode (vs AI):** Play against a computer opponent. The AI evaluates the board state to proactively block your winning moves and secure its own wins when possible.
* **Local Two-Player Mode:** Play against a friend on the same terminal.
* **Persistent Scoreboard:** Uses File I/O (`scores.txt`) to save, track, and rank player wins across different gaming sessions.
* **Dynamic Board:** 8x8 grid with a simulated "gravity" effect—pieces automatically fall to the lowest available row in the selected column.
* **Multi-Directional Win Detection:** Efficiently checks for 5-in-a-row across horizontal, vertical, and both diagonal axes.

## 🛠️ Tech Stack

* **Language:** C
* **Core Concepts:** 2D Arrays, File I/O, Matrix Traversal, Basic Heuristic Algorithms

## 🚀 Getting Started

### Prerequisites
You need a C compiler (like `gcc`) installed on your system to run this game.

### Installation & Compilation
1. Clone the repository:
   ```bash
   git clone [https://github.com/Codered1729/Connect-5-Game.git](https://github.com/Codered1729/Connect-5-Game.git)

#include "board.h"
#include <iostream>
#include <unistd.h> // for reading STDIN

void Game_Board::PrintBoard() {
  std::cout << std::endl << ' ';
  for (int i = 0; i < cols * 2 + 1; i++) {
    // dash
    std::cout << "-";
  }
  std::cout << "  " << std::endl;
  for (int i = 0; i < rows * cols; i++) {
    if (i % cols == 0) {
      // vertical line
      std::cout << "| ";
    }
    if (i == cursor_location) {
      // highlight cursor
      std::cout << "\033[48;2;100;100;100m";
    }
    if (board[i].state == -1 && dead) {
      // reveal all mines if game over
      std::cout << "\033[2;31m"
                << "\u2620";
    }
    else if (board[i].flag && !board[i].revealed) {
      // flag character
      std::cout << "\u2691";
    } else if (!board[i].revealed) {
      std::cout << '*';
    } else if (board[i].state == -1 && !board[i].flag) {
      // print red skull
      std::cout << "\033[2;31m"
                << "\u2620";
    } else if (board[i].state == 0) {
      // blank character (spaces don't highlight on Replit)
      std::cout << ' ';
    } else {
      PrintNum(board[i].state);
    }
    // space and reset character
    std::cout << "\033[0m" << ' ';
    
    /*
    if the remainder is 0, then print an endline
    we're at the end of the line
    */
    if ((i + 1) % cols == 0) {
      // vertical line
      std::cout << "|" << std::endl;
    }
  }
  std::cout << ' ';
  for (int i = 0; i < cols * 2 + 1; i++) {
    // dash
    std::cout << "-";
  }
  std::cout << ' ' << std::endl;
}

void Game_Board::PrintNum(int num) {
  // blue
  if (num == 1) {
    std::cout << "\033[34m" << num;
  }
  // green
  if (num == 2) {
    std::cout << "\033[32m" << num;
  }
  // red
  if (num == 3) {
    std::cout << "\033[31m" << num;
  }
  // magenta
  if (num == 4) {
    std::cout << "\033[35m" << num;
  }
  // orange
  if (num == 5) {
    std::cout << "\033[38;5;208m" << num;
  }
  // cyan
  if (num == 6) {
    std::cout << "\033[36m" << num;
  }
  // yellow
  if (num == 7) {
    std::cout << "\033[33m" << num;
  }
  // gray
  if (num == 8) {
    std::cout << "\033[37m" << num;
  }
 }

void Game_Board::SetDimensions(int user_row, int user_col) {
  rows = user_row;
  cols = user_col;

  // default cursor location is middle of board
  cursor_location = (rows / 2) * cols + cols / 2;
  // dynamically create array of tiles
  board = new Tile[rows * cols];
  tiles_remaining = rows * cols;
  mines_left = tiles_remaining / 10;
}

void Game_Board::PlaceMines() {
  int user_row = cursor_location / cols;
  int user_col = cursor_location % cols;

  // amount of mines is 10% of the area
  int max_mines = rows * cols / 10;
  // array of mine locations and count of mines
  int mine_locations[max_mines];
  int placed_mines = 0;

  while (placed_mines < max_mines) {
    int random_location = rand() % (rows * cols);
    // prevent placing mine in user's first selected location or adjacent tiles
    if ((random_location / cols >= user_row - 1 &&
         random_location / cols <= user_row + 1) &&
        (random_location % cols >= user_col - 1 &&
         random_location % cols <= user_col + 1)) {
      continue;
    }

    bool unique = true;
    for (int mine_index = 0; mine_index < placed_mines; mine_index++) {
      if (mine_locations[mine_index] == random_location) {
        unique = false;
        break;
      }
    }
    // only place mine and increment if location is unique
    if (unique) {
      board[random_location].state = -1;
      mine_locations[placed_mines] = random_location;
      placed_mines++;
    }
  }
}

void Game_Board::GetDistance() {
  for (int location = 0; location < rows * cols; location++) {
    if (board[location].state != -1) {
      int count = 0;
      // check to the right unless we're on the right edge of the board
      if (board[location + 1].state == -1 
        && (location % cols) != cols - 1)
        count++;
      // check to the left unless we're on the left edge of the board
      if (board[location - 1].state == -1 
        && location % cols != 0)
        count++;
      // check above unless we're on the top of the board
      if (board[location - cols].state == -1 
        && location - cols >= 0)
        count++;
      // check below unless we're on the bottom of the board
      // the last location is always the rows * cols, so we can't exceed that
      if (board[location + cols].state == -1 
        && (location + cols) < (rows * cols))
        count++;
      // check above and to the right
      if (board[location - cols + 1].state == -1 
        && (location % cols) != cols - 1 
        && (location - cols + 1) >= 0)
        count++;
      // check above and to the left
      if (board[location - cols - 1].state == -1 
        && (location % cols) != 0 
        && (location - cols - 1) >= 0)
        count++;
      // check below and to the right
      if (board[location + cols + 1].state == -1 &&
          (location % cols) != cols - 1 
        && (location + cols + 1) < (rows * cols))
        count++;
      // check below and to the left
      if (board[location + cols - 1].state == -1 
        && location % cols != 0 
        && (location + cols - 1) >= 0)
        count++;

      board[location].state = count;
    }
  }
}

void Game_Board::Reveal(int user_row, int user_col) {

  board[user_row * cols + user_col].revealed = true;
  tiles_remaining -= 1;
  if (board[user_row * cols + user_col].state == -1 &&
      !board[user_row * cols + user_col].flag) {
    dead = true;
  }

  if (board[user_row * cols + user_col].state == 0) {
    // reveal to the left unless on edge and cell is not a bomb
    if (user_col > 0 && !board[user_row * cols + user_col - 1].revealed &&
        board[user_row * cols + user_col - 1].state != -1) {
      Reveal(user_row, user_col - 1);
    }
    // reveal to the right unless on edge and cell is not a bomb
    if (user_col < cols - 1 &&
        !board[user_row * cols + user_col + 1].revealed &&
        board[user_row * cols + user_col + 1].state != -1) {
      Reveal(user_row, user_col + 1);
    }
    // reveal above unless on edge and cell is not a bomb
    if (user_row > 0 && !board[(user_row - 1) * cols + user_col].revealed &&
        board[(user_row - 1) * cols + user_col].state != -1) {
      Reveal(user_row - 1, user_col);
    }
    // reveal below unless on edge and cell is not a bomb
    if (user_row < rows - 1 &&
        !board[(user_row + 1) * cols + user_col].revealed &&
        board[(user_row + 1) * cols + user_col].state != -1) {
      Reveal(user_row + 1, user_col);
    }
    // reveal up and to the left unless on edge and cell is not a bomb
    if (user_row > 0 && user_col > 0 &&
        !board[(user_row - 1) * cols + user_col - 1].revealed &&
        board[(user_row - 1) * cols + user_col - 1].state != -1) {
      Reveal(user_row - 1, user_col - 1);
    }
    // reveal up and to the right unless on edge and cell is not a bomb
    if (user_row > 0 && user_col < cols - 1 &&
        !board[(user_row - 1) * cols + user_col + 1].revealed &&
        board[(user_row - 1) * cols + user_col + 1].state != -1) {
      Reveal(user_row - 1, user_col + 1);
    }
    // reveal down and to the left unless on edge and cell is not a bomb
    if (user_row < rows - 1 && user_col > 0 &&
        !board[(user_row + 1) * cols + user_col - 1].revealed &&
        board[(user_row + 1) * cols + user_col - 1].state != -1) {
      Reveal(user_row + 1, user_col - 1);
    }
    // reveal down and to the right unless on edge
    if (user_row < rows - 1 && user_col < cols - 1 &&
        !board[(user_row + 1) * cols + user_col + 1].revealed &&
        board[(user_row + 1) * cols + user_col + 1].state != -1) {
      Reveal(user_row + 1, user_col + 1);
    }
  }
}

// default constructor
Game_Board::Game_Board() {}

// constructor to create game board faster
Game_Board::Game_Board(int user_row, int user_col) {
  SetDimensions(user_row, user_col);
}

// destructor to delete dynamically created array
Game_Board::~Game_Board() { delete[] board; }

void Game_Board::GetInput() {

  bool move = false;
  char input[3];
  read(STDIN_FILENO, input, 3);
    if (input[0] == '\033') { // Escape character
      if (input[1] == '[') {  // next key for arrows
        // up
        if (input[2] == 'A')
          cursor_location -= cols;

        // down
        else if (input[2] == 'B')
          cursor_location += cols;

        // right
        else if (input[2] == 'C')
          cursor_location++;

        // left
        else if (input[2] == 'D')
          cursor_location--;
        move = true;
      }
    }
    // wrapping
    if (cursor_location < 0)
      cursor_location += rows * cols;
    if (cursor_location >= rows * cols)
      cursor_location -= rows * cols;
    if (cursor_location % cols < 0)
      cursor_location += cols;
    if (cursor_location % cols >= cols)
      cursor_location -= cols;

    // enter
    else if (input[0] == 10) {
      if (!board[cursor_location].flag) {
        if (first_move) {
          PlaceMines();
          GetDistance();
          first_move = false;
        }

       // only reveal if we haven't revealed already
     if (!board[cursor_location].revealed){
        int user_row = cursor_location / cols;
        int user_col = cursor_location % cols;
        Reveal(user_row, user_col);
        move = true;
	}
      } else {
        std::cout << "\033[H\033[J";
      }
    }
    // capital or lowercase F
    else if (input[0] == 102 || input[0] == 70) {
      if (!board[cursor_location].flag 
        && !board[cursor_location].revealed) {
        board[cursor_location].flag = true;
        mines_left--;
      } else {
        board[cursor_location].flag = false;
        if (!board[cursor_location].revealed)
          mines_left++;
      }
      move = true;
    } else {
      std::cout << "\033[H\033[J";
    }
    if (move) {
      std::cout << "\033[H\033[J";
    }
  }


void Game_Board::PrintLocation() {
  int user_row = cursor_location / cols;
  int user_col = cursor_location % cols;
  std::cout << '(' << user_row << ", " << user_col << ')' << std::endl;
}

bool Game_Board::IsDead() { return dead; }

bool Game_Board::IsWon(){ return tiles_remaining == rows * cols / 10; }

void Game_Board::PrintFlags() {
  std::cout << "\u2691" << ' ' << mines_left << std::endl;
}

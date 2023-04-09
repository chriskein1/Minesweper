/*
  MINESWEEPER
  Chris Keiningham
  Made in C++ without the use of Curses
*/

#include <cstdlib>
#include <ctime>
#include "board.h"
#include "setup.h"

int main() {
  srand(time(NULL));
  unsigned int start_time = time(NULL);
  std::cout << "\033[H\033[J";
  PrintMenu();

  int user_row, user_col;
  GetDimensions(user_row, user_col);

  bool first_move = true;
  
  // initialize board
  Game_Board board(user_row, user_col);

  // disable terminal echoing 
  DisableEcho();
  std::cout << "\033[H\033[J";
  while (true) {
    
    PrintMenu();
    board.PrintBoard();
    board.PrintLocation();
    board.PrintFlags();
    // display elapsed time
    std::cout << "\U000029D7" << ' ' << time(NULL) - start_time << std::endl;
    
    if (board.IsDead()){
      std::cout << "BOOM! You selected a mine\n" << "Thanks for playing!\n";
      break;
    }
    if (board.IsWon()){
      std::cout << "You Win!\n" << "Thanks for playing!\n";
      break;
    }
    board.GetInput();
  }

  
  EnableEcho();
}
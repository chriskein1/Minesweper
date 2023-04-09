#include "setup.h"

void PrintMenu() {
  // print text art
std::cout << 
"████╗  ███╗██╗███╗   ██╗███████╗███████╗██╗    ██╗███████╗███████╗██████╗ ███████╗██████╗\n"
"████╗ ████║██║████╗  ██║██╔════╝██╔════╝██║    ██║██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗\n"
"██╔████╔██║██║██╔██╗ ██║█████╗  ███████╗██║ █╗ ██║█████╗  █████╗  ██████╔╝█████╗  ██████╔╝\n"
"██║╚██╔╝██║██║██║╚██╗██║██╔══╝  ╚════██║██║███╗██║██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██╔══██╗\n"
"██║ ╚═╝ ██║██║██║ ╚████║███████╗███████║╚███╔███╔╝███████╗███████╗██║     ███████╗██║  ██║\n"
"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝ ╚══╝╚══╝ ╚══════╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝\n"
"\nBy Chris Keiningham\n\n";
}


// prompt user for custom dimensions
void GetDimensions(int &user_row, int &user_col) {
  std::cout << "Enter dimensions for board:\n";
  std::cout << "Dimensions can be between 5 and 30\n";
  std::cout << "Rows: ";
  std::cin >> user_row;
  std::cout << "Cols: ";
  std::cin >> user_col;
  while (user_row > 30 || user_col > 30 || user_row < 5 || user_col < 5) {
    std::cout << "\nOutside range!\n";
    std::cout << "Enter dimensions for board:\n";
    std::cout << "Dimensions can be between 5 and 30\n";
    std::cout << "Rows: ";
    std::cin >> user_row;
    std::cout << "Cols: ";
    std::cin >> user_col;
  }
}

void DisableEcho() {
  // make a termios variable
  termios term;
  // get info from terminal to edit
  tcgetattr(0, &term);
  // change flags for canon and echoing (buffer/echo)
  term.c_lflag &= ~(ICANON | ECHO);
  // set terminal based on changes made
  // (TCSANOW meaning set changes immediately)
  tcsetattr(0, TCSANOW, &term);
}

void EnableEcho() {
  termios term;
  // get info to change again
  tcgetattr(0, &term);
  // re-enable echoing / canon
  term.c_lflag |= (ICANON | ECHO);
  // set changes
  tcsetattr(0, TCSANOW, &term);
}
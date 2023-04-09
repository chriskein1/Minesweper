#ifndef BOARD_H
#define BOARD_H

struct Board {
  int state = 0;
  bool revealed = false;
  bool flag = false;
};

class Game_Board {
private:
  int rows;
  int cols;
  Board *board;
  int cursor_location;
  bool first_move = true;
  bool dead = false;
  int tiles_remaining;
  void SetDimensions(int user_row, int user_col);
  void PrintNum(int num);
  int mines_left;

public:
  bool IsDead();
  void PrintBoard();
  void GetDistance();
  void PlaceMines();
  void Reveal(int user_row, int user_col);
  void GetInput();
  void PrintLocation();
  bool IsWon();
  void PrintFlags();

  ~Game_Board();
  Game_Board();
  Game_Board(int user_row, int user_col);
};

#endif
#ifndef SETUP_H
#define SETUP_H

#include <iostream>
#include <termios.h>

void GetDimensions(int &user_row, int &user_col);
void PrintMenu();
void DisableEcho();
void EnableEcho();

#endif
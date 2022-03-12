#ifndef COMPUTER_CPP
#define COMPUTER_CPP

#include "Computer.h"

namespace Chess
{
   Computer::Computer(Board &board, const Side &side)
       : _board(board), _side(side) {}

   Side Computer::side() const { return _side; }
}

#endif
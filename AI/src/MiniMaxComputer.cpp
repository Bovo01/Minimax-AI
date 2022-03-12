#ifndef MINIMAX_COMPUTER_CPP
#define MINIMAX_COMPUTER_CPP

#include "MiniMaxComputer.h"

namespace Chess
{
   MiniMaxComputer::MiniMaxComputer(Board &board, const Side &side, int depth)
       : Computer(board, side), _depth(depth) {}

   void MiniMaxComputer::move(std::ostream &os) const {
      // TODO move
   }
}

#endif

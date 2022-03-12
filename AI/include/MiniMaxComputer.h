#ifndef MINIMAX_COMPUTER_H
#define MINIMAX_COMPUTER_H

#include "Computer.h"

namespace Chess
{
   class MiniMaxComputer : public Computer
   {
   private:
      int _depth;

   public:
      MiniMaxComputer(Board &board, const Side &side, int depth);
      void move(std::ostream &os) const;
   };
}

#endif
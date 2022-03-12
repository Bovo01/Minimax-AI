//@author: Lorenzo Pamio
#ifndef COMPUTER_H
#define COMPUTER_H

#include "Board.h"
#include "Piece.h"

namespace Chess
{
   class Computer
   {
   private:
      Board &_board;
      const Side _side;

   public:
      Computer(Board &board, const Side &side);
      void move(std::ostream &os) const;

   private:
      PieceType get_random_promotion() const;
   };
}

#endif
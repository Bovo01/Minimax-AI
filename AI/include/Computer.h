#ifndef COMPUTER_H
#define COMPUTER_H

#include <ostream>
#include "Board.h"
#include "Piece.h"

namespace Chess
{
   class Computer
   {
   protected:
      Board &_board;
      const Side _side;

   public:
      Computer(Board &board, const Side &side);
      /* Permette di muovere il pezzo e mandare in otput la mossa effettuata */
      virtual void move(std::ostream &os) const = 0;
      Side side() const;
   };
}

#endif
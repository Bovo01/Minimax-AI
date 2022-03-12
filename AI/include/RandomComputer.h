//@author: Lorenzo Pamio
#ifndef RANDOM_COMPUTER_H
#define RANDOM_COMPUTER_H

#include "Computer.h"

namespace Chess
{
   class RandomComputer : public Computer
   {
   public:
      // Eredito il costruttore
      using Computer::Computer;
      // Movimento
      void move(std::ostream &os) const;

   private:
      PieceType get_random_promotion() const;
   };
}

#endif
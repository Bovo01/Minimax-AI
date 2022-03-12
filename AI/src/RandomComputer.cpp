//@Author: Lorenzo Pamio
#ifndef RANDOM_COMPUTER_CPP
#define RANDOM_COMPUTER_CPP

#include <vector>
#include "RandomComputer.h"

namespace Chess
{
   void RandomComputer::move(std::ostream &os) const
   {
      std::vector<Piece> pieces;
      // Prendo pezzi dello schieramento attuale del bot
      _board.get_pieces(_side, pieces);
      // Scelgo un pezzo casuale del quale effettuare una mossa
      while (_side == _board.turn())
      {
         int piece = rand() % pieces.size();
         // Controllo se il pezzo può muoversi
         std::vector<Position> moves;
         pieces[piece].get_moves(moves);
         if (moves.size() != 0)
         {
            // Estraggo una posizione random di quelle "legali" nella quale spostarmi
            int position = rand() % moves.size();
            try
            {
               bool promotion = pieces[piece].type() == PAWN && moves[position].y == (pieces[piece].side() == WHITE ? 7 : 0);
               // Tento lo spostamento. Se la mossa non è legale ricomincio il ciclo ed estraggo un altro pezzo
               // Passo ad ogni mossa una eventuale promozione
               PieceType promot_type = get_random_promotion();
               _board.move(pieces[piece].position(), moves[position], promot_type);
               os << pieces[piece].position().to_string() + " " + moves[position].to_string();
               if (promotion)
                  os << ' ' << ((char) (promot_type));
               os << std::endl;
            }
            catch (Board::IllegalMoveException e)
            {
            }
         }
      }
   }

   PieceType RandomComputer::get_random_promotion() const
   {
      switch (rand() % 4)
      {
      case 0:
         return QUEEN;
      case 1:
         return BISHOP;
      case 2:
         return ROOK;
      case 3:
         return KNIGHT;
      default: // Impossibile
         return KING;
      }
   }

}

#endif
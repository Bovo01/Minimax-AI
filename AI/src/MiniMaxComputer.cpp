#ifndef MINIMAX_COMPUTER_CPP
#define MINIMAX_COMPUTER_CPP

#include "MiniMaxComputer.h"

namespace Chess
{
   MiniMaxComputer::MiniMaxComputer(Board &board, const Side &side, int depth)
       : Computer(board, side), _depth(depth), _positions(Tree<Board>(board)) {}

   void MiniMaxComputer::move(std::ostream &os) const {
      std::vector<Piece> pieces;
      _board.get_pieces(_side, pieces);

      for (const Piece &piece : pieces) {
         std::vector<Position> moves;
         const Position &from = piece.position();
         piece.get_moves(moves);
         for (const Position &to : moves) {

         }
      }
   }
}

#endif

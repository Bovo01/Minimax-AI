#ifndef MINIMAX_COMPUTER_H
#define MINIMAX_COMPUTER_H

#include "Computer.h"

namespace Chess
{
   struct Evaluation {
      int score;
      Position from;
      Position to;

      Evaluation(int score) : score(score), from(0, 0), to(0, 0) {}
      Evaluation(int score, Position from, Position to) : score(score), from(from), to(to) {}
   };

   class MiniMaxComputer : public Computer
   {
   private:
      int _depth;

   private:
      Evaluation minimax(const Board &b, const int depth, const Side &turn) const;
      // Dà una valutazione alla posizione che sarà positiva se questo computer sta vincendo, negativa altrimenti
      int evaluate_position(const Board &b) const;
      // Dà una valutazione ad un singolo pezzo, ignorando il suo schieramento
      static int get_value(const Piece &p);

   public:
      MiniMaxComputer(Board &board, const Side &side, int depth);
      void move(std::ostream &os) const;
   };
}

#endif
#ifndef MINIMAX_COMPUTER_CPP
#define MINIMAX_COMPUTER_CPP

#include "MiniMaxComputer.h"

namespace Chess
{
   MiniMaxComputer::MiniMaxComputer(Board &board, const Side &side, int depth)
       : Computer(board, side), _depth(depth) {}

   void MiniMaxComputer::move(std::ostream &os) const
   {
      Evaluation bestMove = minimax(_board, 0, _side);
      _board.move(bestMove.from, bestMove.to);
   }

   Evaluation MiniMaxComputer::minimax(const Board &b, const int depth, const Side &turn) const
   {
      // Caso base, ritorno la valutazione della posizione attuale
      if (depth >= _depth)
         return evaluate_position(b);

      constexpr int UNREACHABLE_SCORE = 1000000000;
      Evaluation bestEvaluation{(turn == _side) ? -UNREACHABLE_SCORE : UNREACHABLE_SCORE};

      std::vector<Piece> pieces;
      b.get_pieces(turn, pieces);

      for (const Piece &piece : pieces)
      {
         std::vector<Position> moves;
         piece.get_moves(moves);

         const Position &from = piece.position();

         for (const Position &to : moves)
         {
            Board tmp_b{b};
            try
            {
               // TODO Considerare la promozione
               tmp_b.move(from, to, QUEEN);
               int score;
               if (const Ending game_over = tmp_b.is_game_over())
                  // Controllo se la partita è finita
                  score = evaluate_game_over(game_over, depth);
               else
                  // Chiamata ricorsiva
                  score = minimax(tmp_b, /*turn == _side ? depth : */ depth + 1, !turn).score;
               // minimax (scelgo il "figlio" dell'albero ricorsivo che ha la mossa migliore)
               if (turn == _side)
               {
                  // Massimizzo il punteggio
                  if (bestEvaluation.score < score)
                     bestEvaluation = Evaluation{score, from, to};
               }
               else
               {
                  // Minimizzo il punteggio (prendo il migliore per l'avversario)
                  if (bestEvaluation.score > score)
                     bestEvaluation = Evaluation{score, from, to};
               }
            }
            catch (...)
            { /* Se entra qua la mossa non è valida e viene ignorata */
            }
         }
      }

      return bestEvaluation;
   }

   int MiniMaxComputer::evaluate_game_over(const Ending &finale, const int depth) const {
      constexpr int VICTORY_POINTS = 104;
      const int POINTS_ON_VICTORY = VICTORY_POINTS * (_depth - depth + 1);

      switch (finale)
      {
      case STALEMATE:
      case _50_MOVE_RULE:
      case INSUFFICIENT_MATERIAL:
      case REPETITION:
         return 0;
      case WHITE_CHECKMATE:
         return _side == WHITE ? POINTS_ON_VICTORY : -POINTS_ON_VICTORY;
      case BLACK_CHECKMATE:
         return _side == BLACK ? POINTS_ON_VICTORY : -POINTS_ON_VICTORY;
      default:
         throw "Finale non valido";
      }
   }

   int MiniMaxComputer::evaluate_position(const Board &b) const
   {
      // Valuto la posizione in base al materiale
      int evaluation = 0;
      std::vector<Piece> allies;
      b.get_pieces(_side, allies);

      for (const Piece &p : allies)
      {
         evaluation += get_value(p);
      }

      std::vector<Piece> enemies;
      b.get_pieces(!_side, enemies);

      for (const Piece &p : enemies)
      {
         evaluation -= get_value(p);
      }

      return evaluation;
   }

   int MiniMaxComputer::get_value(const Piece &p)
   {
      switch (p.type())
      {
      case PieceType::PAWN:
         return 1;
      case PieceType::KNIGHT:
      case PieceType::BISHOP:
         return 3;
      case PieceType::ROOK:
         return 5;
      case PieceType::QUEEN:
         return 9;
      case PieceType::KING:
         return 0;
      }
      throw "Pezzo non valido";
   }
}

#endif

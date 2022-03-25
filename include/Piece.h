//@author Lorenzo Pamio
#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "Position.h"

namespace Chess
{
   // Enum che rappresenta lo schieramento di un pezzo
   enum Side
   {
      WHITE = 0,
      BLACK = 1
   };
   // Enum che rappresenta il pezzo stesso
   enum PieceType
   {
      PAWN = 'P',
      KNIGHT = 'C',
      BISHOP = 'A',
      ROOK = 'T',
      QUEEN = 'D',
      KING = 'R'
   };
   std::string get_pgn_char(PieceType type);
   // Classe Piece che rappresenta un pezzo in tutte le sue caratteristiche
   class Piece
   {
   private:
      Position _position;
      Side _side;
      PieceType _type;

   private:
      void get_king_moves(std::vector<Position> &v) const;
      void get_queen_moves(std::vector<Position> &v) const;
      void get_rook_moves(std::vector<Position> &v) const;
      void get_bishop_moves(std::vector<Position> &v) const;
      void get_knight_moves(std::vector<Position> &v) const;
      void get_pawn_moves(std::vector<Position> &v) const;

   public:
      Piece(const Position position, const Side side, const PieceType type);

      // Getter di _position
      Position position(void) const;
      // Getter di _side
      Side side(void) const;
      // Getter di _type
      PieceType type(void) const;

      // Sposta il pezzo dalla posizione corrente a quella passata per parametro, senza eseguire controlli
      void move(const Position position);

      // Ritorna tutte le posizioni possibili in cui il pezzo corrente si potrebbe muovere,
      //    senza considerare gli altri pezzi nella scacchiera
      void get_moves(std::vector<Position> &v) const;

      bool operator==(const Piece& piece) const;
      bool operator!=(const Piece& piece) const;
   };

   Side operator!(const Side &side);
}

#endif
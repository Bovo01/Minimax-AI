#ifndef POSITION_CPP
#define POSITION_CPP

#include <ostream>
#include "Position.h"

namespace Chess
{

   bool Position::is_valid(void) const
   {
      if (!((x < 0) || (x > 7)) && !((y < 0) || ((y > 7))))
         return true;

      return false;
   }

   void Position::decode_arithmetic_notation(const std::string &arithmetic_notation)
   {
      if (arithmetic_notation.size() != 2)
         throw InvalidPositionException();

      x = std::tolower(arithmetic_notation[0]) - 97;
      y = arithmetic_notation[1] - 49;

      if (!(is_valid()))
         throw InvalidPositionException();
   }

   Position::Position(const short x, const short y) : x(x), y(y)
   {
      if (!(is_valid()))
         throw InvalidPositionException();
   }

   Position::Position(const std::string arithmetic_notation)
   {
      decode_arithmetic_notation(arithmetic_notation);
   }

   Position::Position(const char *arithmetic_notation)
       : Position(std::string(arithmetic_notation))
   {
   }

   Position Position::move(const short x_increment, const short y_increment) const
   {
      short newX = x + x_increment;
      short newY = y + y_increment;

      return Position(newX, newY);
   }

   bool Position::operator==(const Position &other) const
   {
      if ((x == other.x) && (y == other.y))
         return true;
      return false;
   }

   bool Position::operator!=(const Position &other) const
   {
      return !(*this == other);
   }

   std::string Position::to_string() const {
      std::string s;
      s.push_back(((char) ('A' + x)));
      s.push_back((char) ('1' + y));
      return s;
   }


   std::ostream& operator<<(std::ostream &os, const Position &pos) {
      return os << pos.to_string();
   }
}

#endif
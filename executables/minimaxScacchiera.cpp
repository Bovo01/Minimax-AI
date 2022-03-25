#include <iostream>
#include <fstream>
#include "Board.h"
#include "MiniMaxComputer.h"
// Macro per utilizzare il codice su windows
#ifdef _WIN32
#include <Windows.h>
#define clear std::system("cls")
#define wait Sleep(1000)
#endif
// Macro per utilizzare il codice su linux
#ifdef linux
#include <unistd.h>
#define clear std::system("clear")
#define wait sleep(1)
#endif

constexpr int DEPTH = 3;

int main() {
   Chess::Board b;
   Chess::MiniMaxComputer ai_1(b, Chess::Side::WHITE, DEPTH);
   Chess::MiniMaxComputer ai_2(b, Chess::Side::BLACK, DEPTH);

   Chess::Ending game_over;
   int moves = 0;
   std::ofstream out("minimax.txt");
   try {
   do {
      clear;
      std::cout << b << std::endl;
      out << b.turn() << std::endl << b << std::endl;
      wait;
      if (ai_1.side() == b.turn())
         ai_1.move(std::cout);
      else
         ai_2.move(std::cout);
   } while (!(game_over = b.is_game_over()) && ++moves < 60);

   clear;
   std::cout << b << std::endl << std::endl;

   std::cout << Chess::ending(game_over);
   } catch (...) {}
   std::ofstream pgn("pgn.txt");
   pgn << b.get_pgn();
   pgn.close();


   return 0;
}
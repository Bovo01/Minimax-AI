// @author: Pietro Bovolenta
#include <iostream>
#include <fstream>
#include <time.h>
#include "Board.h"
#include "Computer.h"
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

Chess::Ending player_computer(Chess::Board &b, std::ostream &os);
Chess::Ending computer_computer(Chess::Board &b, std::ostream &os);

int main(int argc, char *argv[])
{
   if (argc < 2 || (std::string(argv[1]) != "pc" && std::string(argv[1]) != "cc"))
   {
      std::cout << "Devi passare come parametro 'pc' o 'cc' per giocare tra giocatore e computer o tra computer e computer\n";
      return -1;
   }

   srand(time(NULL));

   Chess::Board board;

   Chess::Ending game_over;

   std::cout << "Inserisci il nome da dare a questa partita: ";
   std::string match_name;
   std::getline(std::cin, match_name);

   std::ofstream ofs(match_name + ".txt");

   clear;
   if (std::string(argv[1]) == "pc")
      game_over = player_computer(board, ofs);
   else
      game_over = computer_computer(board, ofs);

   ofs.close();

   std::cout << board << "\n\n";

   std::cout << Chess::ending(game_over) << std::endl;

   return 0;
}

Chess::Ending player_computer(Chess::Board &board, std::ostream &os)
{
   Chess::Side player_side = Chess::Side(rand() % 2);
   Chess::Computer computer{board, !player_side};

   Chess::Ending game_over;

   std::cout << "Giochi con il " << (player_side == Chess::WHITE ? "bianco" : "nero") << "\n\n";

   do
   {
      if (board.turn() == player_side)
      {
         std::cout << board << std::endl;

         std::string move;
         std::getline(std::cin, move);

         if (move.size() != 5 && move.size() != 7)
         {
            clear;
            std::cout << "La mossa inserita non e' valida. Formato accettato: 'xx xx', in caso di promozione 'xx xx p'\n\n";
            continue;
         }

         try
         {
            const Chess::Position from{move.substr(0, 2)};
            const Chess::Position to{move.substr(3, 2)};
            bool promotion = board.find_piece(from).type() == Chess::PAWN && to.y == (player_side == Chess::WHITE ? 7 : 0);
            if (move.size() == 5)
               board.move(from, to);
            else
               board.move(from, to, Chess::PieceType(std::toupper(move[6])));
            os << from << ' ' << to;
            // Caso promozione, mando in output il pezzo a cui ho promosso
            if (promotion)
               os << ' ' << (char)(board.find_piece(to).type());
            os << std::endl;
         }
         catch (Chess::Board::IllegalMoveException e)
         {
            clear;
            std::cout << "Mossa non valida\n\n";
            continue;
         }
         catch (Chess::Board::PieceNotFoundException e)
         {
            clear;
            std::cout << "Non c'e' un pezzo alla posizione iniziale\n\n";
            continue;
         }
         catch (Chess::Position::InvalidPositionException e)
         {
            clear;
            std::cout << "La posizione inserita non e' valida. Controlla che la lettera sia tra 'A' e 'H' e il numero tra 1 e 8\n\n";
            continue;
         }

         clear;
      }
      else
      {
         computer.move(os);
      }
   } while (!(game_over = board.is_game_over()));
   clear;
   return game_over;
}

Chess::Ending computer_computer(Chess::Board &board, std::ostream &os)
{
   constexpr int MAX_MOVES = 120;

   Chess::Computer computer1{board, Chess::WHITE};
   Chess::Computer computer2{board, Chess::BLACK};

   Chess::Ending game_over;
   int moves = 1;
   do
   {
      std::cout << board << std::endl;

      wait;

      if (board.turn() == Chess::WHITE)
         computer1.move(os);
      else
      {
         computer2.move(os);
         moves++;
      }

      clear;
   } while (!(game_over = board.is_game_over()) && moves <= MAX_MOVES);
   return game_over;
}
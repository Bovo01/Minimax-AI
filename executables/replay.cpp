//@author: Lorenzo Pamio
#include <iostream>
#include <fstream>
#include "Board.h"

#ifdef _WIN32
#include <Windows.h>
#define clear std::system("cls")
#define wait Sleep(1000)
#endif

#ifdef linux
#include <unistd.h>
#define clear std::system("clear")
#define wait sleep(1)
#endif

const char *ending(Chess::Ending e);
const bool is_read_legal(std::string line);

int main(int argc, char *argv[])
{
   //Inizio verificando gli argomenti passati al replay
   if (argc <= 1 || argc > 4)
   {
      std::cout << "Inserisci un numero valido di argomenti";
      return -1;
   }
   //Controllo che il primo argomento inserito sia uguale a v o f (Per avviare una delle funzioni previste dal programma replay)
   if (!(*argv[1] == 'v' || *argv[1] == 'f'))
   {
      std::cout << "Inserisci degli argomenti validi";
      return -1;
   }
   if (*argv[1] == 'v')
   {
      //Creo la board e la mostro per la prima volta
      Chess::Board b;
      std::cout << b << std::endl;
      std::ifstream file{argv[2]};
      int line_counter = 0;
      //Leggo finche il file non è terminato
      while (!file.eof())
      {
         std::string line;
         std::getline(file, line);
         line_counter++;
         if (line.size() == 0)
            continue;
         if (!is_read_legal(line))
         {
            std::cout << "Trovata stringa non valida: '" << line << "' alla riga numero: " << line_counter << std::endl;
            std::cout << "Continuo saltando la riga corrente?: S/N" << std::endl
                      << "!Potrebbe generare problemi!:";
            std::cin >> line;
            if (std::toupper(line[0]) == 'S')
               continue;
            break;
         }
         wait;
         clear;
         try
         {
            char promotionType = 'K';
            if (line.size() == 7)
               promotionType = line[6];
            b.move(line.substr(0, 2), line.substr(3, 2), Chess::PieceType(promotionType));
            std::cout << b << std::endl;
         }
         catch (Chess::Board::IllegalMoveException e)
         {
            std::cout << "Errore irreversibile board, rilevata mossa non valida:" << line;
         }
      }
      std::cout << Chess::ending(b.is_game_over());
   }
   else
   {
      Chess::Board b;
      std::ifstream fileIn{argv[2]};
      std::ofstream fileOut{argv[3]};
      fileOut << b << std::endl;
      int line_counter = 0;
      while (!fileIn.eof())
      {
         std::string line;
         std::getline(fileIn, line);

         line_counter++;
         fileOut << "-------------" << std::endl;
         if (line.size() == 0)
            continue;
         if (!is_read_legal(line))
         {
            std::cout << "Trovata stringa non valida: '" << line << "' alla riga numero: " << line_counter << std::endl;
            std::cout << "Continuo saltando la riga corrente?:S/N" << std::endl
                      << "NOTA(Potrebbe generare problemi!)";
            std::cin >> line;
            if (std::toupper(line[0]) == 'S')
               continue;
            break;
         }
         try
         {
            char promotionType = 'K';
            if (line.size() == 7)
               promotionType = line[6];
            b.move(line.substr(0, 2), line.substr(3, 2), Chess::PieceType(promotionType));
            fileOut << b << std::endl;
         }
         catch (Chess::Board::IllegalMoveException e)
         {
            std::cout << "Errore irreversibile board, rilevata mossa non valida:" << line << std::endl;
         }
      }

      fileOut << Chess::ending(b.is_game_over());
      fileOut.close();
   }
   return 0;
}

const bool is_read_legal(std::string line)
{
   int size = line.size();
   //verifico innanzitutto se lo standard è rispettato e rispetta la nomenclatura "LN LN" || "LN LN L" (L=lettera N=numero)
   //controllo se la stringa ha 5 o 7 caratteri
   if (size == 5 || size == 7)
   {
      if (std::isalpha(line[0]) && std::isdigit(line[1]) && std::isalpha(line[3]) && std::isdigit(line[4]))
      {
         //se sono nel caso 5 caratteri, ho finito, la nomenclatura è legale
         if (size == 5)
            return true;
         //verifico ultimo carattere, se è alfabetico sono apposto
         return std::isalpha(line[6]);
      }
   }
   return false;
}
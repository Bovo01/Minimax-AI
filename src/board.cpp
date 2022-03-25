// @author: Pietro Bovolenta
#ifndef BOARD_CPP
#define BOARD_CPP

#include <algorithm>
#include <iostream>
#include "Board.h"

namespace Chess
{
   const char *ending(Ending e)
   {
      switch (e)
      {
      case STALEMATE:
         return "Patta per stallo";
      case _50_MOVE_RULE:
         return "Patta per regola delle 50 mosse";
      case INSUFFICIENT_MATERIAL:
         return "Patta per materiale insufficiente";
      case REPETITION:
         return "Patta per ripetizione";
      case WHITE_CHECKMATE:
         return "SCACCOMATTO! Vince il bianco";
      case BLACK_CHECKMATE:
         return "SCACCOMATTO! Vince il nero";
      default:
         return "La partita non e' finita";
      }
   }

   /*       COSTRUTTORI       */

   Board::Board()
       : _pieces(std::vector<Piece>())
   {
      _pieces.reserve(32);
      initialize();
   }

   Board::Board(const Board &other)
       : _pieces(other._pieces), _turn(other._turn),
         _can_white_castle_left(other._can_white_castle_left), _can_white_castle_right(other._can_white_castle_right),
         _can_black_castle_left(other._can_black_castle_left), _can_black_castle_right(other._can_black_castle_right),
         _last_pawn_move(other._last_pawn_move), _50_move_count(other._50_move_count),
         _50_move_start(other._50_move_start), _positions(other._positions)
   {
   }

   /*       METODI PRIVATI       */

   void Board::initialize(void)
   {
      // Pezzi del bianco
      _pieces.push_back(Piece{"A1", WHITE, ROOK});
      _pieces.push_back(Piece{"B1", WHITE, KNIGHT});
      _pieces.push_back(Piece{"C1", WHITE, BISHOP});
      _pieces.push_back(Piece{"D1", WHITE, QUEEN});
      _pieces.push_back(Piece{"E1", WHITE, KING});
      _pieces.push_back(Piece{"F1", WHITE, BISHOP});
      _pieces.push_back(Piece{"G1", WHITE, KNIGHT});
      _pieces.push_back(Piece{"H1", WHITE, ROOK});
      // Pedoni
      for (short i = 0; i < 8; i++)
      {
         _pieces.push_back(Piece{{i, 1}, WHITE, PAWN});
         _pieces.push_back(Piece{{i, 6}, BLACK, PAWN});
      }
      // Pezzi del nero
      _pieces.push_back(Piece{"A8", BLACK, ROOK});
      _pieces.push_back(Piece{"B8", BLACK, KNIGHT});
      _pieces.push_back(Piece{"C8", BLACK, BISHOP});
      _pieces.push_back(Piece{"D8", BLACK, QUEEN});
      _pieces.push_back(Piece{"E8", BLACK, KING});
      _pieces.push_back(Piece{"F8", BLACK, BISHOP});
      _pieces.push_back(Piece{"G8", BLACK, KNIGHT});
      _pieces.push_back(Piece{"H8", BLACK, ROOK});

      _positions.push_back(_pieces);
   }

   void Board::toggle_turn(void)
   {
      _turn = !_turn;
   }

   bool Board::is_valid_promotion_type(const PieceType &type)
   {
      switch (type)
      {
      case KING:
      case PAWN:
         return false;
      case KNIGHT:
      case BISHOP:
      case ROOK:
      case QUEEN:
         return true;
      default:
         return false;
      }
   }

   bool Board::is_check(const Side &side, const std::vector<Piece> &pieces) const
   {
      // Prendo il re dalla scacchiera (non serve controllare che ci sia il re perché ci deve essere, altrimenti la posizione sulla scacchiera non avrebbe senso)
      const Piece king = *std::find_if(pieces.begin(), pieces.end(),
                                       [&side](const Piece &p)
                                       {
                                          return p.type() == KING && p.side() == side;
                                       });
      for (const Piece &p : pieces)
      {
         // Ignoro i pezzi dello stesso schieramento del re
         if (p.side() == side)
            continue;
         // Controllo se la posizione del re è compresa tra quelle possibili del pezzo
         std::vector<Position> moves;
         p.get_moves(moves);

         auto it = std::find_if(moves.begin(), moves.end(),
                                [&king](const Position &pos)
                                {
                                   return king.position() == pos;
                                });

         if (it == moves.end())
            continue;
         // Se ho un pedone che è sulla stessa colonna del re, di sicuro non può scaccarlo
         if (p.type() == PAWN && p.position().x == king.position().x)
            continue;
         // Se la visione tra il pezzo e il re non è ostruita da altri pezzi (di qualsiasi schieramento), il re è sotto scacco
         if (!is_obstructed(p, king.position(), pieces))
            return true;
      }
      return false;
   }

   bool Board::is_obstructed(const Piece &p, const Position &to, const std::vector<Piece> &pieces) const
   {
      // Il cavallo non può avere il percorso ostruito
      if (p.type() == KNIGHT)
         return false;
      // Controllo se c'è qualche pezzo tra 'p.position()' e 'to' partendo dalla posizione precedente a 'to' e arrivando alla successiva di 'p.position()'
      short directionX = p.position().x > to.x ? 1 : (p.position().x < to.x ? -1 : 0),
            directionY = p.position().y > to.y ? 1 : (p.position().y < to.y ? -1 : 0);
      Position curr = to.move(directionX, directionY);

      while (p.position() != curr)
      {
         auto it = std::find_if(pieces.begin(), pieces.end(),
                                [&curr](const Piece &piece)
                                {
                                   return piece.position() == curr;
                                });
         // Se trovo un pezzo tra 'p.position()' e 'to' ritorno true (c'è qualcuno che blocca la vista di 'p' verso 'to')
         if (it != pieces.end())
            return true;
         // Incremento la posizione attuale
         curr = curr.move(directionX, directionY);
      }
      return false;
   }

   void Board::kill_piece(const Position &position)
   {
      try
      {
         find_piece(position);
         _pieces.erase(std::find_if(_pieces.begin(), _pieces.end(), [&position](const Piece &p)
                                    { return p.position() == position; }));
      }
      catch (PieceNotFoundException e)
      {
      }
   }

   bool Board::can_move(const Piece &p_from, const Position &to) const
   {
      const Position &from = p_from.position();
      // Controlla se è il turno del pezzo selezionato
      if (p_from.side() != _turn)
         return false;
      // Controlla se la posizione finale è tra quelle in cui si può muovere il pezzo selezionato
      std::vector<Position> moves;
      p_from.get_moves(moves);

      auto it =
          std::find_if(moves.begin(),
                       moves.end(),
                       [&to](const Position pos)
                       {
                          return pos == to;
                       });
      if (it == moves.end())
         return false;
      // Controlla se la casa di arrivo contiene un pezzo dello stesso schieramento di quello selezionato
      try
      {
         Piece p_to = find_piece(to);
         if (p_to.side() == p_from.side())
            return false;
         // Se è un pedone e si sta muovendo in avanti non può muoversi se c'è un pezzo in to
         if (p_from.type() == PAWN && from.x == to.x)
            return false;
         // Se è il re e sta cercando di arroccare non può se c'è un pezzo nella posizione finale
         if (p_from.type() == KING && abs(from.x - to.x) == 2)
            return false;
      }
      catch (PieceNotFoundException e)
      {
         // Si arriva qua se ci si sta muovendo in una casa vuota
         // Non si fa niente perché significa che il pezzo si potrebbe muovere in questa posizione
      }

      if (is_obstructed(p_from, to, _pieces))
         return false;

      // Simulo il movimento del pezzo e controllo se è una mossa legale (cioè se mette in scacco il mio stesso re)
      std::vector<Piece> simulated_pieces;
      simulated_pieces.reserve(_pieces.size());
      for (const Piece &p : _pieces)
      {
         if (p.position() != from && p.position() != to)
            simulated_pieces.push_back(p);
      }
      simulated_pieces.push_back(Piece{to, p_from.side(), p_from.type()});

      if (is_check(p_from.side(), simulated_pieces))
         return false;

      // Movimento pedone
      if (p_from.type() == PAWN)
      {
         if (abs(from.y - to.y) == 2) // Sto cercando di avanzare un pedone di 2
         {
            // Non puoi muovere il pedone di 2 se non è nella posizione iniziale
            if ((p_from.side() == WHITE && from.y != 1) || (p_from.side() == BLACK && from.y != 6))
               return false;
            // Controllo che la casa subito davanti al pedone sia libera (la casa di arrivo è già stata controllata)
            try
            {
               find_piece(from.move(0, p_from.side() == WHITE ? 1 : -1));
               // Ho trovato un pezzo davanti al pedone quindi non si può muovere
               return false;
            }
            catch (PieceNotFoundException e)
            {
               // La casa davanti al pedone è libera. OK
            }
         }
         else if (from.x != to.x) // Sto mangiando in obliquo
         {
            try
            {
               find_piece(to);
               // Il caso in cui il pezzo di arrivo è dello stesso schieramento è già stato gestito
            }
            catch (PieceNotFoundException e)
            {
               // Per muoversi in diagonale il pedone deve mangiare (se non è un caso di en passant non è valido)
               if (!(_last_pawn_move == to.x && from.y == (p_from.side() == WHITE ? 4 : 3)))
                  return false;
            }
         }
         // La spinta di 1 del pedone è già gestita
      }
      else if (p_from.type() == KING) /* Arrocco re */
      {
         if (abs(from.x - to.x) == 2)
         {
            short castle_direction = from.x < to.x ? 1 : -1;
            // Controllo che il re non abbia ancora perso il diritto ad arroccare
            if (p_from.side() == WHITE)
            {
               if (castle_direction > 0 && !_can_white_castle_right)
                  return false;
               if (castle_direction < 0 && !_can_white_castle_left)
                  return false;
            }
            else
            {
               if (castle_direction > 0 && !_can_black_castle_right)
                  return false;
               if (castle_direction < 0 && !_can_black_castle_left)
                  return false;
            }
            // Se al momento il re è sotto scacco non può arroccare
            if (is_check(p_from.side(), _pieces))
               return false;
            // Controllo se nella posizione intermedia (tra quella di arrocco e quella attuale) c'è un pezzo (quella finale è già controllata)
            try
            {
               find_piece({(short)(from.x + castle_direction), from.y});
               // Se c'è un pezzo, questo blocca l'arrocco
               return false;
            }
            catch (PieceNotFoundException e)
            {
               // OK, niente ferma l'arrocco
            }
            // Controllo se la posizione intermedia (tra quella di arrocco e quella attuale) mette sotto scacco il re
            std::vector<Piece> simulated_pieces;
            simulated_pieces.reserve(_pieces.size());
            for (const Piece &p : _pieces)
            {
               if (p.position() != from)
                  simulated_pieces.push_back(p);
            }
            simulated_pieces.push_back(Piece{{(short)(from.x + castle_direction), from.y}, p_from.side(), p_from.type()});

            if (is_check(p_from.side(), simulated_pieces))
               return false;
         }
      }
      return true;
   }

   /*       GETTERS       */

   Side Board::turn(void) const
   {
      return _turn;
   }

   void Board::get_pieces(Side side, std::vector<Piece> &output) const
   {
      // Inizializza vector output con size esatta di pezzi appertenente al side corrispondente
      output = std::vector<Piece>{};
      for (const Piece &p : _pieces)
      {
         if (p.side() == side)
            output.push_back(p);
      }
   }

   std::string Board::get_pgn() const
   {
      Board b;
      std::string pgn;
      int turno = 0;

      for (const Mossa &mossa : _mosse)
      {
         // Aggiungo il numero di mossa
         if (b._turn == WHITE)
            pgn += std::to_string(++turno) + ".";
         const Piece p_from = b.find_piece(mossa.from);
         // Controllo arrocco
         if (p_from.type() == KING && abs(mossa.from.x - mossa.to.x) == 2)
         {
            pgn += "0-0";
            // Arrocco lungo
            if (mossa.to.x == 2)
               pgn += "-0";
         }
         else
         {
            // Aggiungo il pezzo spostato
            pgn += get_pgn_char(p_from.type());
            // Controllo ambiguità
            std::vector<Piece> same_pieces;
            same_pieces.reserve(b._pieces.size());
            std::copy_if(b._pieces.begin(), b._pieces.end(), same_pieces.begin(),
                         [&p_from](const Piece &p)
                         {
                            return p.position() != p_from.position() && p.type() == p_from.type() && p.side() == p_from.side();
                         });
            for (const Piece &p : same_pieces)
            {
               if (b.can_move(p, mossa.to))
               {
                  // C'è un'ambiguità
                  if (p_from.position().x != p.position().x)
                     pgn += p_from.position().to_lower_string()[0];
                  else if (p_from.position().y != p.position().y)
                     pgn += p_from.position().to_lower_string()[1];
                  break;
               }
            }
            // Controllo se sta mangiando
            try
            {
               b.find_piece(mossa.to);
               // Se ha mangiato aggiungo 'x'
               pgn += 'x';
            }
            catch (PieceNotFoundException e)
            {
               // Potenziale en passant (aggiungo 'x' perché sto mangiando)
               if (p_from.type() == PAWN && mossa.from.x != mossa.to.x)
                  pgn += 'x';
            }
            // Scrivo la posizione in cui mi sono mosso
            pgn += mossa.to.to_lower_string();
         }
         // Eseguo la mossa nella scacchiera 'virtuale'
         b.move(mossa.from, mossa.to);
         // Controllo se è matto
         if (Ending end = b.is_checkmate_stalemate(b._turn))
         {
            if (end == WHITE_CHECKMATE)
               pgn += "# 1-0";
            else if (end == BLACK_CHECKMATE)
               pgn += "# 0-1";
            else // patta
               pgn += " 0.5-0.5";
            break;
         }
         // Se non è matto controllo se è scacco
         else if (b.is_check(b._turn, b._pieces))
         {
            pgn += '+';
         }
         pgn += ' ';
      }

      return pgn;
   }

   /* CONTROLLO FINALI */

   bool Board::is_insufficient_material() const
   {
      short white_bishop_color = -1,
            black_bishop_color = -1;
      bool has_white_knight = false,
           has_black_knight = false;

      for (const Piece &p : _pieces)
      {
         switch (p.type())
         {
         case KING:
            break;
         case PAWN:
         case ROOK:
         case QUEEN:
            // C'è ancora abbastanza materiale
            return false;
         case BISHOP:
         {
            short bishop_color = (p.position().x + p.position().y) % 2;
            if (p.side() == WHITE)
            {
               // Alfiere e cavallo bastano per mattare
               if (has_white_knight)
                  return false;

               // Se non è stato impostato, imposto il colore dell'alfiere
               if (white_bishop_color == -1)
                  white_bishop_color = bishop_color;
               else if (white_bishop_color != bishop_color)
                  /* se c'è un alfiere del colore opposto a questo, la partita non è ancora patta */
                  return false;
            }
            else
            {
               // Alfiere e cavallo bastano per mattare
               if (has_black_knight)
                  return false;

               // Se non è stato impostato, imposto il colore dell'alfiere
               if (black_bishop_color == -1)
                  black_bishop_color = bishop_color;
               else if (black_bishop_color != bishop_color)
                  /* se c'è un alfiere del colore opposto a questo, la partita non è ancora patta */
                  return false;
            }
         }
         break;
         case KNIGHT:
            if (p.side() == WHITE)
            {
               // Due cavalli / Un cavallo e un alfiere  bastano per mattare
               if (has_white_knight || white_bishop_color != -1)
                  return false;

               has_white_knight = true;
            }
            else
            {
               // Due cavalli / Un cavallo e un alfiere  bastano per mattare
               if (has_black_knight || black_bishop_color != -1)
                  return false;

               has_black_knight = true;
            }
         }
      }
      return true;
   }

   Ending Board::is_checkmate_stalemate(const Side &side) const
   {
      // Controllo se il re ha mosse legali
      const Piece king = *std::find_if(_pieces.begin(), _pieces.end(), [&side](const Piece &p)
                                       { return p.type() == KING && p.side() == side; });
      std::vector<Position> moves;
      king.get_moves(moves);
      // Controllo le mosse del re all'inizio per una questione di ottimizzazione
      for (const Position &pos : moves)
      {
         // Se il re ha mosse legali la partita non è finita
         if (can_move(king, pos))
            return NONE;
      }
      // Controllo se ci sono mosse legali tra tutti pezzi di side
      for (const Piece &p : _pieces)
      {
         if (p.type() == KING || p.side() != side)
            continue;

         std::vector<Position> moves;
         p.get_moves(moves);
         for (const Position &pos : moves)
         {
            // Se il pezzo ha mosse legali la partita non è finita
            if (can_move(p, pos))
            {
               can_move(p, pos);
               return NONE;
            }
         }
      }

      // Se non ci sono mosse legali è scacco matto o stallo

      if (is_check(side, _pieces)) /* Scacco matto */
      {
         if (side == WHITE)
            return BLACK_CHECKMATE; // Il nero ha mattato il bianco
         return WHITE_CHECKMATE;    // Il bianco ha mattato il nero
      }
      // Altrimenti stallo
      return STALEMATE;
   }

   bool Board::is_repetition(void) const
   {
      for (int i = 0; i < _positions.size(); i++)
      {
         short repetitions = 1;
         const std::vector<Piece> &curr_pos = _positions[i];
         for (int j = i + 1; j < _positions.size(); j++)
         {
            const std::vector<Piece> &next_pos = _positions[j];
            if (curr_pos.size() != next_pos.size())
            {
               continue;
            }
            bool found = true;
            for (const Piece &p : curr_pos)
            {
               if (std::find_if(next_pos.begin(), next_pos.end(), [&p](const Piece &other)
                                { return p == other; }) == next_pos.end())
               {
                  found = false;
                  break;
               }
            }
            if (found)
            {
               repetitions++;
            }
         }
         if (repetitions >= 3)
            return true;
      }
      return false;
   }

   /*       OVERLOAD OPERATORI       */

   std::ostream &operator<<(std::ostream &os, const Board &b)
   {
      for (short i = 0; i < 8; i++)
      {
         os << 8 - i << ' ';
         for (short j = 0; j < 8; j++)
         {
            try
            {
               Piece p = b.find_piece({j, (short)(7 - i)});
               os << (p.side() == BLACK ? (char)p.type() : (char)(p.type() + 32));
            }
            catch (Board::PieceNotFoundException e)
            {
               os << ' ';
            }
         }
         os << std::endl;
      }
      os << std::endl;
      os << "  ";
      for (char c = 'A'; c <= 'H'; c++)
         os << c;
      return os;
   }
   /*       FUNZIONALITA' DI GIOCO       */

   Piece &Board::find_piece(const Position &position)
   {
      auto it = std::find_if(_pieces.begin(),
                             _pieces.end(),
                             [&position](const Piece &p)
                             {
                                return p.position() == position;
                             });
      if (it != _pieces.end())
         return *it;
      throw PieceNotFoundException();
   }

   Piece Board::find_piece(const Position &position) const
   {
      auto it = std::find_if(_pieces.begin(),
                             _pieces.end(),
                             [&position](const Piece &p)
                             {
                                return p.position() == position;
                             });
      if (it != _pieces.end())
         return *it;
      throw PieceNotFoundException();
   }

   void Board::move(const Position from, const Position to, PieceType promotion_type)
   {
      // Lancia una PieceNotFoundException se non viene trovato un pezzo alla posizione from
      Piece *p_from = &find_piece(from);
      if (!can_move(*p_from, to))
         throw IllegalMoveException();
      // Controllo la promozione
      if (p_from->type() == PAWN && to.y == (p_from->side() == WHITE ? 7 : 0))
      {
         if (!is_valid_promotion_type(promotion_type))
         {
            std::cout << "Inserisci il carattere del pezzo a cui vuoi promuovere: ";
            std::string line;
            std::getline(std::cin, line);
            if (line.size() > 0)
               promotion_type = PieceType(std::toupper(line[0]));
         }
         if (!is_valid_promotion_type(promotion_type))
            throw IllegalMoveException();
      }

      // Gestione regola delle 50 mosse
      try
      {
         find_piece(to);
         // Sta avvenendo una cattura => resetto il contatore delle 50 mosse
         _50_move_count = 0;
      }
      catch (PieceNotFoundException e)
      {
         if (p_from->type() == PAWN)
            // Se sto muovendo un pedone azzero il contatore
            _50_move_count = 0;
         else if (p_from->side() == _50_move_start)
            // Se non è stato azzerato il contatore, lo incremento (solo una volta ogni 2, visto che una mossa è data da una del bianco e una del nero)
            _50_move_count++;
      }
      // Se il contatore delle mosse è a 0, segnalo chi l'ha inizializzato
      if (_50_move_count == 0)
         _50_move_start = p_from->side();

      /* MOVIMENTO EFFETTIVO DEL PEZZO */
      // Muovo il pezzo selezionato nella posizione finale
      kill_piece(to);
      // Riassegno a p_from il pezzo alla posizione from (viene sminchiato dal kill_piece)
      p_from = &find_piece(from);
      // Cambio turno
      p_from->move(to);
      // Elimino il pezzo nella posizione finale (che viene mangiato)
      toggle_turn();
      /* FINE MOVIMENTO DEL PEZZO */

      /* CASI SPECIALI */
      // En passant, elimino il pezzo mangiato e valorizzo la variabile _last_pawn_move
      if (p_from->type() == PAWN)
      {
         // Sto mangiando en passant
         if (_last_pawn_move == to.x && from.y == (p_from->side() == WHITE ? 4 : 3))
         {
            // Elimino il pezzo mangiato
            kill_piece(Position{to.x, from.y});
            // Riassegno a p_from il pezzo alla posizione from (viene sminchiato dal kill_piece)
            p_from = &find_piece(to);
         }

         // Il pedone è avanzato di 2
         if (abs(from.y - to.y) == 2)
            _last_pawn_move = to.x;
         else
            _last_pawn_move = -1; // valore invalido, non ho appena avanzato un pedone di 2

         // Promozione
         if (to.y == (p_from->side() == WHITE ? 7 : 0))
         {
            const Side s = p_from->side();
            // Elimino il pedone appena mosso
            kill_piece(to);
            // Sostituisco il pedone con la sua promozione
            _pieces.push_back(Piece{to, s, promotion_type});
            // Riassegno a p_from il pezzo alla posizione from (viene sminchiato dal kill_piece)
            p_from = &find_piece(to);
         }
      }
      else
         _last_pawn_move = -1; // valore invalido
      // Arrocco
      if (p_from->type() == KING)
      {
         // Rimuovo l'arrocco al re che si è appena mosso
         if (p_from->side() == WHITE)
         {
            _can_white_castle_right = false;
            _can_white_castle_left = false;
         }
         else
         {
            _can_black_castle_right = false;
            _can_black_castle_left = false;
         }

         // Se il re sta arroccando muovo la rispettiva torre (il re è già stato spostato)
         if (abs(from.x - to.x) == 2)
         {
            short castle_direction = from.x < to.x ? 1 : -1;
            // Sposto la torre vicino al re
            Piece &rook = find_piece(Position{(short)(castle_direction == 1 ? 7 : 0), from.y});
            rook.move(Position{(short)(from.x + castle_direction), from.y});
         }
      }
      else if (p_from->type() == ROOK) /* Tolgo la possibilità di arroccare al re se sto muovendo una torre */
      {
         if (p_from->side() == WHITE)
         {
            if (from.x == 7)
               _can_white_castle_right = false;
            else if (from.x == 0)
               _can_white_castle_left = false;
         }
         else
         {
            if (from.x == 7)
               _can_black_castle_right = false;
            else if (from.x == 0)
               _can_black_castle_left = false;
         }
      }

      _positions.push_back(_pieces);
      _mosse.push_back({from, to, promotion_type});
   }

   Ending Board::is_game_over(void) const
   {
      if (_50_move_count >= 50)
         return _50_MOVE_RULE;
      if (is_insufficient_material())
         return INSUFFICIENT_MATERIAL;
      if (is_repetition())
         return REPETITION;
      return is_checkmate_stalemate(_turn);
   }
}

#endif
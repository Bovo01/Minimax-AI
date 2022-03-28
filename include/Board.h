// @author: Pietro Bovolenta
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <ostream>
#include "Position.h"
#include "Piece.h"

namespace Chess
{
   enum Ending
   {
      // false se non è finita la partita
      NONE = 0,
      // patte (1 - 5)
      STALEMATE = 1,
      _50_MOVE_RULE,
      INSUFFICIENT_MATERIAL,
      REPETITION,
      // Vittorie (10 - 11)
      // Il bianco ha mattato
      WHITE_CHECKMATE = 10,
      // Il nero ha mattato
      BLACK_CHECKMATE
   };
   // Ritorna in stringa il finale
   const char *ending(Chess::Ending e);

   // Struttura che contiene una mossa
   struct Mossa
   {
      Position from;
      Position to;
      PieceType promotion;
   };

   class Board
   {
   private:
      std::vector<Piece> _pieces;
      Side _turn{WHITE};

      /* VARIABILI UTILI PER MANTENERE LO STATO DELLA PARTITA */
      /**
       * arrocco:
       *  Primi due bit -> arrocco per il bianco
       *  Secondi due bit -> arrocco per il nero
       *  Primo bit di ogni coppia: torre 'a', secondo bit: torre 'h'
       */
      __int8 _castling{0b1111};
      // ultima colonna pedone (solo per en passant)
      short _last_pawn_move{-1};
      // regola delle 50 mosse
      short _50_move_count{0};
      Side _50_move_start;
      // Elenco di tutte le posizione avvenute nella scacchiera (per controllare la ripetizione di mosse)
      std::vector<std::vector<Piece>> _positions;
      // Elenco di tutte le mosse avvenute
      std::vector<Mossa> _mosse;

   private:
      // Prepara la posizione iniziale riempiendo il vector _pieces
      void initialize(void);
      // Cambia il turno
      void toggle_turn(void);
      // Ritorna true se la posizione corrente ha generato uno scacco al re dello schieramento side
      bool is_check(const Side &side, const std::vector<Piece> &pieces) const;
      // Ritorna true se il pezzo di partenza è ostruito da un altro pezzo cercando di arrivare alla posizione to
      bool is_obstructed(const Piece &p, const Position &to, const std::vector<Piece> &pieces) const;
      // Metodo che elimina il pezzo alla posizione indicata dal vettore _pieces
      void kill_piece(const Position &position);
      // Ritorna true il pezzo alla posizione from si può muovere nella posizione to, false altrimenti
      bool can_move(const Piece &p_from, const Position &to) const;
      // Controlla se il pezzo può essere una promozione valida
      bool is_valid_promotion_type(const PieceType &type);

      /* CONTROLLO FINALI */
      // Controlla se la posizione per il lato side è scacco matto o stallo o se è una posizione giocabile
      Ending is_checkmate_stalemate(const Side &side) const;
      // Controlla se la posizione non può essere vinta da nessuno a causa di materiale insufficiente
      bool is_insufficient_material() const;
      // Controlla se la posizione corrente è stata già rivista 3 volte durante la partita
      bool is_repetition() const;
      // Rimuove l'arrocco al re dello schieramento passato per parametro
      // In caso di movimento di una torre viene passata per parametro la sua coordinata x
      void remove_castling(const Side &s, const short rook_pos = -1);
      // Controlla se il re di uno schieramento può arroccare nella direzione passata per parametro
      bool can_castle(const Side &s, const short direction) const;

   public:
      // Costruttore che inizializza una partita
      Board();
      Board(const Board &other);

      // Eccezione per indicare una mossa invalida o illegale
      class IllegalMoveException
      {
      };
      // Eccezione che viene lanciata quando non si è trovato un pezzo
      class PieceNotFoundException
      {
      };

      // Cerca il pezzo ad una certa posizione e lo ritorna
      // Lancia una PieceNotFoundException se alla posizione inserita non c'è alcun pezzo
      Piece find_piece(const Position &position) const;
      Piece &find_piece(const Position &position);


      // Getter per il turno attuale
      Side turn(void) const;

      // Getter per i pezzi di nero e bianco, in base al side passato
      // Copia i pezzi nel vector passato come output
      void get_pieces(Side side, std::vector<Piece> &output) const;

      // Ritorna il pgn della partita giocata
      std::string get_pgn() const;

      // Sposta un pezzo dalla posizione 'from' alla posizione 'to'
      // Lancia una eccezione, se per qualche motivo la mossa non è valida
      void move(const Position from, const Position to, const PieceType promotion_type = PieceType::KING);

      // Sposta il pezzo 'p' alla posizione 'to'
      // ATTENZIONE Non vengono effettuati controlli, AZIONE PERICOLOSA
      void _move(const Piece p, const Position to, const PieceType promotion_type = PieceType::KING);

      // Ottiene tutte le mosse legali del pezzo piece e le mette nel vector out
      void get_moves(const Piece &p, std::vector<Position> &out) const;

      // Ritorna 'Ending::NONE = 0' se la partita non è finita, altrimenti ritorna il modo in cui è finita la partita
      // Controlla solo il giocatore del side = _turn
      Ending is_game_over(void) const;

      // Stampa la scacchiera
      friend std::ostream &operator<<(std::ostream &os, const Board &b);
   };
}

#endif
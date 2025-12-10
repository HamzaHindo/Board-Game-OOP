
#ifndef Ultimate_RESULT_CLASSES_H
#define Ultimate_RESULT_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

class Result_Board : public Board<char> {
private:
  char blank_symbol =
      '.'; ///< Character used to represent an empty cell on the board.

public:
  Result_Board();

  bool update_board(Move<char> *move);

  bool is_win(Player<char> *player);

  bool is_lose(Player<char> *) { return false; };

  bool is_draw(Player<char> *player);

  bool game_is_over(Player<char> *player);
};

class Result_UI : public UI<char> {

public:
  Result_UI();

  ~Result_UI() {};

  Player<char> *create_player(string &name, char symbol, PlayerType type);
  void display_board_matrix(const vector<vector<char>> &matrix) const;

  Player<char> **setup_players();

  Move<char> *get_move(Player<char> *player) { return new Move(0, 0, 'n'); }
};

#endif // Ultimate_RESULT_CLASSES_H

/*
 * Regular 3x3 board but each cell is a 3x3 board -> in total 9x9 board!
 * To win a game you must win 3 (Sub games) in a line or diagonal
 * When a user won we must change the cell to be with his Symbol
 */

#ifndef Ultimate_CLASSES_H
#define Ultimate_CLASSES_H

#include "BoardGame_Classes.h"
#include "IGame.h"
using namespace std;

class Ultimate_Board : public Board<char> {
private:
  char blank_symbol =
      '.'; ///< Character used to represent an empty cell on the board.

public:
  Ultimate_Board();

  bool update_board(Move<char> *move);

  bool is_win(Player<char> *player);

  bool is_lose(Player<char> *) { return false; };

  bool is_draw(Player<char> *player);

  bool game_is_over(Player<char> *player);
};

class Ultimate_UI : public UI<char> {

  vector<vector<char>> innerMatrix;

public:
  Ultimate_UI();

  ~Ultimate_UI() {};

  Player<char> *create_player(string &name, char symbol, PlayerType type);
  void display_board_matrix(const vector<vector<char>> &matrix) const;

  void replace_sub_board_with_symbol(const Player<char> &player) const;

  Player<char> **setup_players();

  virtual Move<char> *get_move(Player<char> *player);
};

class Ultimate : public IGame {

  Ultimate_UI *ui = new Ultimate_UI;

  Ultimate_Board *board = new Ultimate_Board;

public:
  Ultimate();
  ~Ultimate();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif // Ultimate_CLASSES_H

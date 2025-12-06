
#ifndef Pyramid_CLASSES_H
#define Pyramid_CLASSES_H

#include "BoardGame_Classes.h"
#include "IGame.h"
using namespace std;

class Pyramid_Board : public Board<char> {
private:
  char blank_symbol = '.';
  vector<vector<char>> board;

public:
  Pyramid_Board();
  bool update_board(Move<char> *move);
  bool is_win(Player<char> *player);
  bool is_lose(Player<char> *) { return false; };
  bool is_draw(Player<char> *player);
  bool game_is_over(Player<char> *player);
};

class Pyramid_UI : public UI<char> {
public:
  Pyramid_UI();
  ~Pyramid_UI() {};
  Player<char> *create_player(string &name, char symbol, PlayerType type);
  void display_board_matrix(const vector<vector<char>> &matrix) const;

  Player<char> **setup_players();
  virtual Move<char> *get_move(Player<char> *player);
};

class Pyramid : public IGame {

  Pyramid_UI *ui = new Pyramid_UI;

  Pyramid_Board *board = new Pyramid_Board;

public:
  Pyramid();
  ~Pyramid();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif // Pyramid_CLASSES_H

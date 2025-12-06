#ifndef Word_CLASSES_H
#define Word_CLASSES_H

#include "BoardGame_Classes.h"
#include "IGame.h"
using namespace std;
class Word_Board : public Board<char> {
private:
  char blank_symbol = '.';

public:
  Word_Board();

  bool update_board(Move<char> *move);
  bool is_win(Player<char> *player);
  bool is_lose(Player<char> *) { return false; };
  bool is_draw(Player<char> *player);
  bool game_is_over(Player<char> *player);
};
class Word_UI : public UI<char> {
public:
  Word_UI();
  ~Word_UI() {};
  Player<char> *create_player(string &name, char symbol, PlayerType type);
  Player<char> **setup_players();
  virtual Move<char> *get_move(Player<char> *player);
};

class Word : public IGame {

  Word_UI *ui = new Word_UI;

  Word_Board *board = new Word_Board;

public:
  Word();
  ~Word();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif

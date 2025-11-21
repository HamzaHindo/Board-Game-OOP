#ifndef Word_TTT_CLASSES_H
#define Word_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include "IGame.h"
using namespace std;
class Word_TTT_Board : public Board<char> {
private:
  char blank_symbol = '.';

public:
  Word_TTT_Board();

  bool update_board(Move<char> *move);
  bool is_win(Player<char> *player);
  bool is_lose(Player<char> *) { return false; };
  bool is_draw(Player<char> *player);
  bool game_is_over(Player<char> *player);
};
class Word_TTT_UI : public UI<char> {
public:
  Word_TTT_UI();
  ~Word_TTT_UI() {};
  Player<char> *create_player(string &name, char symbol, PlayerType type);
  Player<char> **setup_players();
  virtual Move<char> *get_move(Player<char> *player);
};

class Word_TTT : public IGame {

  Word_TTT_UI *ui = new Word_TTT_UI;

  Word_TTT_Board *board = new Word_TTT_Board;

public:
  Word_TTT();
  ~Word_TTT();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif

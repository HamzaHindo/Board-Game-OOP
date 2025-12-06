#ifndef _MISERE_TIC_TAC_TOE_H
#define _MISERE_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <algorithm>
#include <limits>

using namespace std;

class Misere_Board : public Board<char> {
public:
  Misere_Board();
  bool update_board(Move<char> *move) override;
  bool is_win(Player<char> *player) override;
  bool is_lose(Player<char> *player) override;
  bool is_draw(Player<char> *player) override;
  bool game_is_over(Player<char> *player) override;
  bool has_three_in_row(char symbol);
  void undo_move(int x, int y);
};

class Misere_AI_Player : public Player<char> {
private:
  int evaluate(Misere_Board *board, int depth);
  int minimax(Misere_Board *board, int depth, bool is_maximizing, int alpha,
              int beta);

public:
  Misere_AI_Player(char symbol);
  Move<char> *get_best_move();
};

class Misere_UI : public UI<char> {
public:
  Misere_UI();
  Move<char> *get_move(Player<char> *player) override;
  Player<char> **setup_players() override;
  Player<char> *create_player(string &name, char symbol,
                              PlayerType type) override;
};

class Misere : public IGame {
private:
  Misere_UI *ui;
  Misere_Board *board;

public:
  Misere();
  ~Misere();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};
#endif

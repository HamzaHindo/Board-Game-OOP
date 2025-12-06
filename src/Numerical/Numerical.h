#ifndef NUMERICAL_H
#define NUMERICAL_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <set>
#include <string>
#include <vector>

using namespace std;

class Numerical_Board : public Board<int> {
private:
  int blank_symbol = 0;
  set<int> used_numbers;
  const int WIN_SUM = 15;

public:
  Numerical_Board();
  bool update_board(Move<int> *move);
  bool is_win(Player<int> *player);
  bool is_lose(Player<int> *) { return false; }
  bool is_draw(Player<int> *player);
  bool game_is_over(Player<int> *player);
  bool is_number_used(int number) const;
  void mark_number_used(int number);
  void unmark_number_used(int number);
  vector<int> get_available_numbers(bool is_player1) const;
};

class Numerical_UI : public UI<int> {
public:
  Numerical_UI();
  ~Numerical_UI() {}
  Player<int> *create_player(string &name, int symbol, PlayerType type);
  virtual Move<int> *get_move(Player<int> *player);
  Player<int> **setup_players();

private:
  bool is_valid_number(int number, bool is_player1) const;
};

class Numerical : public IGame {
private:
  Numerical_UI *ui;
  Numerical_Board *board;

public:
  Numerical();
  ~Numerical();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif

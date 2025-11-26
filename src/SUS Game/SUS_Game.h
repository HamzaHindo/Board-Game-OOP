#ifndef _SUS_GAME_H
#define _SUS_GAME_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <cctype>
#include <iostream>
#include <set>
using namespace std;

class SUS_Board : public Board<char> {
private:
  int player1_score = 0;
  int player2_score = 0;
  char player1_symbol;
  char player2_symbol;
  set<string> counted_sus;
  char last_player_symbol;

  int count_new_sus_sequences(char player_symbol);

public:
  SUS_Board();
  void set_player_symbols(char p1, char p2);
  bool update_board(Move<char> *move) override;
  bool is_win(Player<char> *player) override;
  bool is_lose(Player<char> *player) override;
  bool is_draw(Player<char> *player) override;
  bool game_is_over(Player<char> *player) override;
  pair<int, int> get_scores() const;
  void display_scores() const;
};

class SUS_Player : public Player<char> {
public:
  SUS_Player(string name, char symbol, PlayerType type);
};

class SUS_Random_Player : public SUS_Player {
public:
  SUS_Random_Player(string name, char symbol);
};

class SUS_UI : public UI<char> {
private:
  SUS_Board *sus_board_ptr;

public:
  SUS_UI();
  void set_sus_board(SUS_Board *board);
  Move<char> *get_move(Player<char> *player) override;
  Player<char> *create_player(string &name, char symbol,
                              PlayerType type) override;
  Player<char> **setup_players() override;
  void display_final_result(string message, pair<int, int> scores);
};

class SUS : public IGame {
private:
  SUS_UI *ui;
  SUS_Board *board;

public:
  SUS();
  ~SUS();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif

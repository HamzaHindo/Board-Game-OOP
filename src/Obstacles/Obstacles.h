#ifndef OBSTACLES_TICTACTOE_H
#define OBSTACLES_TICTACTOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class WinChecker {
private:
  const vector<vector<char>> *board;
  int rows, columns;
  const int WIN_LENGTH = 4;

  bool check_line(int row, int col, int dr, int dc, char mark) const;

public:
  WinChecker(const vector<vector<char>> *b, int r, int c)
      : board(b), rows(r), columns(c) {}

  bool has_won(int row, int col, char mark) const;
};

class ObstacleManager {
private:
  set<pair<int, int>> obstacles;
  vector<vector<char>> *board;
  int rows, columns;
  int obstacle_count = 0;
  static const int INITIAL_OBSTACLES = 0;
  static const int OBSTACLES_PER_ROUND = 2;

  bool add_single_obstacle();
  bool is_valid_position(int row, int col) const;

public:
  ObstacleManager(vector<vector<char>> *b, int r, int c)
      : board(b), rows(r), columns(c) {}

  void initialize();
  void add_obstacles_after_round();
  bool is_obstacle(int row, int col) const;
  void place_obstacle(int row, int col);
  int get_total_obstacles() const { return obstacle_count; }
};

class Obstacles_Board : public Board<char> {
private:
  WinChecker win_checker;
  ObstacleManager obstacle_mgr;

public:
  Obstacles_Board();
  bool update_board(Move<char> *move) override;
  bool is_win(Player<char> *player) override;
  bool is_lose(Player<char> *) override { return false; }
  bool is_draw(Player<char> *player) override;
  bool game_is_over(Player<char> *player) override;

  ObstacleManager &get_obstacle_manager() { return obstacle_mgr; }
  const ObstacleManager &get_obstacle_manager() const { return obstacle_mgr; }
};

class Obstacles_UI : public UI<char> {
public:
  Obstacles_UI();
  ~Obstacles_UI() {}
  Player<char> *create_player(string &name, char symbol, PlayerType type);
  virtual Move<char> *get_move(Player<char> *player);
  void display_board_matrix(const vector<vector<char>> &matrix) const;

private:
  bool get_valid_input(int &row, int &col, const Obstacles_Board *board);
};

class Obstacles : public IGame {
private:
  Obstacles_UI *ui;
  Obstacles_Board *board;
  int turns_played = 0;

  void play_turn(Player<char> *player);
  void add_obstacles_if_needed();
  bool check_game_end(Player<char> *current_player, bool &game_over);

public:
  Obstacles();
  ~Obstacles();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif

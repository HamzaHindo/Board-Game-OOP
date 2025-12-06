#ifndef _GAME3_5X5_TICTACTOE_H
#define _GAME3_5X5_TICTACTOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

class Game3_Board : public Board<char> {
public:
  Game3_Board() : Board<char>(5, 5) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        this->board[i][j] = 0;
      }
    }
  }

  bool update_board(Move<char> *move) override {
    int x = move->get_x();
    int y = move->get_y();

    if (x < 0 || x >= 5 || y < 0 || y >= 5) {
      return false;
    }

    if (this->board[x][y] != 0) {
      return false;
    }

    if (this->n_moves >= 24) {
      return false;
    }

    this->board[x][y] = toupper(move->get_symbol());
    this->n_moves++;
    return true;
  }

  int count_three_in_a_row(char symbol) {
    int count = 0;

    for (int i = 0; i < 5; i++) {
      for (int j = 0; j <= 2; j++) {
        if (this->board[i][j] == symbol && this->board[i][j + 1] == symbol &&
            this->board[i][j + 2] == symbol) {
          count++;
        }
      }
    }

    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j < 5; j++) {
        if (this->board[i][j] == symbol && this->board[i + 1][j] == symbol &&
            this->board[i + 2][j] == symbol) {
          count++;
        }
      }
    }

    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j <= 2; j++) {
        if (this->board[i][j] == symbol &&
            this->board[i + 1][j + 1] == symbol &&
            this->board[i + 2][j + 2] == symbol) {
          count++;
        }
      }
    }

    for (int i = 0; i <= 2; i++) {
      for (int j = 2; j < 5; j++) {
        if (this->board[i][j] == symbol &&
            this->board[i + 1][j - 1] == symbol &&
            this->board[i + 2][j - 2] == symbol) {
          count++;
        }
      }
    }

    return count;
  }

  bool is_win(Player<char> *player) override {
    if (this->n_moves < 24) {
      return false;
    }

    char player_symbol = toupper(player->get_symbol());
    char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';

    int player_count = count_three_in_a_row(player_symbol);
    int opponent_count = count_three_in_a_row(opponent_symbol);

    return player_count > opponent_count;
  }

  bool is_lose(Player<char> *player) override {
    if (this->n_moves < 24) {
      return false;
    }

    char player_symbol = toupper(player->get_symbol());
    char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';

    int player_count = count_three_in_a_row(player_symbol);
    int opponent_count = count_three_in_a_row(opponent_symbol);

    return player_count < opponent_count;
  }

  bool is_draw(Player<char> *player) override {
    if (this->n_moves < 24) {
      return false;
    }

    char player_symbol = toupper(player->get_symbol());
    char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';

    int player_count = count_three_in_a_row(player_symbol);
    int opponent_count = count_three_in_a_row(opponent_symbol);

    return player_count == opponent_count;
  }

  bool game_is_over(Player<char> *player) override {
    return (this->n_moves >= 24);
  }
};

class Game3_Random_Player : public Player<char> {
public:
  Game3_Random_Player(char symbol)
      : Player<char>("Random Computer", symbol, PlayerType::RANDOM) {
    srand(static_cast<unsigned int>(time(0)));
  }

  Move<char> *get_move() {
    int x, y;
    do {
      x = rand() % 5;
      y = rand() % 5;
    } while (this->boardPtr->get_board_matrix()[x][y] != 0);

    return new Move<char>(x, y, this->symbol);
  }
};

class Game3_UI : public UI<char> {
public:
  Game3_UI()
      : UI<char>(
            "Welcome to 5x5 Tic Tac Toe!\n"
            "Players alternate placing marks on a 5x5 grid.\n"
            "Game ends when only 1 square remains empty (24 moves total).\n"
            "Winner is the player with the most three-in-a-row sequences!\n",
            3) {}

  Move<char> *get_move(Player<char> *player) override {
    if (player->get_type() == PlayerType::RANDOM) {
      Game3_Random_Player *comp_player =
          dynamic_cast<Game3_Random_Player *>(player);
      if (comp_player) {
        this->display_message(player->get_name() + " is making a move...");
        return comp_player->get_move();
      }
    }

    int x, y;
    cout << player->get_name() << " (" << player->get_symbol()
         << "), enter your move (row col): ";
    cin >> x >> y;

    return new Move<char>(x, y, player->get_symbol());
  }

  Player<char> *create_player(string &name, char symbol,
                              PlayerType type) override {
    if (type == PlayerType::COMPUTER || type == PlayerType::RANDOM) {
      return new Game3_Random_Player(symbol);
    }
    return new Player<char>(name, symbol, PlayerType::HUMAN);
  }
};

class Game3 : public IGame {
private:
  Game3_UI *ui;
  Game3_Board *board;

public:
  Game3();
  ~Game3();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

#endif

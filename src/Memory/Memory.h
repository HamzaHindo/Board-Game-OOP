#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "BoardGame_Classes.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

template <typename T> class Memory_Board : public Board<T> {
private:
  vector<vector<bool>> hidden;
  int last_move_x, last_move_y;

public:
  Memory_Board();
  bool update_board(Move<T> *move) override;
  bool is_win(Player<T> *player) override;
  bool is_lose(Player<T> *player) override;
  bool is_draw(Player<T> *player) override;
  bool game_is_over(Player<T> *player) override;
  bool is_hidden(int x, int y) const;
  pair<int, int> get_last_move() const;
  void reveal_board();
};

template <typename T> class Memory_Random_Player : public Player<T> {
public:
  Memory_Random_Player(string name, T symbol);
  Move<T> *get_move();
};

template <typename T> class Memory_UI : public UI<T> {
public:
  Memory_UI();
  void display_board_matrix(const vector<vector<T>> &matrix) const override;
  void display_with_last_move(Memory_Board<T> *board);
  void display_final_board(Memory_Board<T> *board);
  Move<T> *get_move(Player<T> *player) override;
  Player<T> **setup_players() override;
};

template <typename T> class Memory_GameManager : public GameManager<T> {
private:
  Memory_Board<T> *memBoard;
  Memory_UI<T> *memUI;
  Player<T> *players[2];

public:
  Memory_GameManager(Memory_Board<T> *b, Player<T> *p[2], Memory_UI<T> *u);
  void run();
};

template <typename T>
Memory_Board<T>::Memory_Board()
    : Board<T>(3, 3), hidden(3, vector<bool>(3, false)), last_move_x(-1),
      last_move_y(-1) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      this->board[i][j] = static_cast<T>(' ');
    }
  }
}

template <typename T> bool Memory_Board<T>::update_board(Move<T> *move) {
  int x = move->get_x();
  int y = move->get_y();

  if (x < 0 || x >= 3 || y < 0 || y >= 3) {
    cout << "Invalid position! Must be between 0 and 2.\n";
    return false;
  }

  if (this->board[x][y] != static_cast<T>(' ')) {
    cout << "Cell already occupied!\n";
    return false;
  }

  this->board[x][y] = move->get_symbol();
  hidden[x][y] = true;
  last_move_x = x;
  last_move_y = y;
  this->n_moves++;

  return true;
}

template <typename T> bool Memory_Board<T>::is_win(Player<T> *player) {
  T symbol = player->get_symbol();

  for (int i = 0; i < 3; i++) {
    if (this->board[i][0] == symbol && this->board[i][1] == symbol &&
        this->board[i][2] == symbol) {
      return true;
    }
  }

  for (int j = 0; j < 3; j++) {
    if (this->board[0][j] == symbol && this->board[1][j] == symbol &&
        this->board[2][j] == symbol) {
      return true;
    }
  }

  if (this->board[0][0] == symbol && this->board[1][1] == symbol &&
      this->board[2][2] == symbol) {
    return true;
  }

  if (this->board[0][2] == symbol && this->board[1][1] == symbol &&
      this->board[2][0] == symbol) {
    return true;
  }

  return false;
}

template <typename T> bool Memory_Board<T>::is_lose(Player<T> *player) {
  return false;
}

template <typename T> bool Memory_Board<T>::is_draw(Player<T> *player) {
  return (this->n_moves == 9 && !is_win(player));
}

template <typename T> bool Memory_Board<T>::game_is_over(Player<T> *player) {
  return is_win(player) || is_draw(player);
}

template <typename T> bool Memory_Board<T>::is_hidden(int x, int y) const {
  return hidden[x][y];
}

template <typename T> pair<int, int> Memory_Board<T>::get_last_move() const {
  return {last_move_x, last_move_y};
}

template <typename T> void Memory_Board<T>::reveal_board() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      hidden[i][j] = false;
    }
  }
}

template <typename T>
Memory_Random_Player<T>::Memory_Random_Player(string name, T symbol)
    : Player<T>(name, symbol, PlayerType::RANDOM) {}

template <typename T> Move<T> *Memory_Random_Player<T>::get_move() {
  vector<pair<int, int>> available;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (this->boardPtr->get_cell(i, j) == static_cast<T>(' ')) {
        available.push_back({i, j});
      }
    }
  }

  if (available.empty()) {
    return new Move<T>(0, 0, this->symbol);
  }

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, available.size() - 1);

  int idx = dis(gen);
  return new Move<T>(available[idx].first, available[idx].second, this->symbol);
}

template <typename T>
Memory_UI<T>::Memory_UI()
    : UI<T>("\n=== Welcome to Memory Tic-Tac-Toe ===\n"
            "Rules: Marks are hidden after placement.\n"
            "Remember positions to win!\n",
            5) {}

template <typename T>
void Memory_UI<T>::display_board_matrix(const vector<vector<T>> &matrix) const {
  cout << "\n    ";
  for (int j = 0; j < 3; ++j)
    cout << setw(6) << j;
  cout << "\n   " << string(20, '-') << "\n";

  for (int i = 0; i < 3; ++i) {
    cout << setw(2) << i << " |";
    for (int j = 0; j < 3; ++j) {
      cout << setw(5) << "?" << " |";
    }
    cout << "\n   " << string(20, '-') << "\n";
  }
  cout << "\nNote: All marks are hidden. Remember their positions!\n";
  cout << endl;
}

template <typename T>
void Memory_UI<T>::display_with_last_move(Memory_Board<T> *board) {
  auto lastMove = board->get_last_move();
  int lx = lastMove.first;
  int ly = lastMove.second;

  cout << "\n    ";
  for (int j = 0; j < 3; ++j)
    cout << setw(6) << j;
  cout << "\n   " << string(20, '-') << "\n";

  for (int i = 0; i < 3; ++i) {
    cout << setw(2) << i << " |";
    for (int j = 0; j < 3; ++j) {
      if (i == lx && j == ly) {
        cout << setw(5) << board->get_cell(i, j) << " |";
      } else {
        cout << setw(5) << "?" << " |";
      }
    }
    cout << "\n   " << string(20, '-') << "\n";
  }
  cout << "\nLast move shown. It will be hidden next turn!\n";
  cout << endl;
}

template <typename T>
void Memory_UI<T>::display_final_board(Memory_Board<T> *board) {
  board->reveal_board();
  auto matrix = board->get_board_matrix();

  cout << "\n=== FINAL BOARD (All marks revealed) ===\n";
  cout << "    ";
  for (int j = 0; j < 3; ++j)
    cout << setw(6) << j;
  cout << "\n   " << string(20, '-') << "\n";

  for (int i = 0; i < 3; ++i) {
    cout << setw(2) << i << " |";
    for (int j = 0; j < 3; ++j) {
      cout << setw(5) << matrix[i][j] << " |";
    }
    cout << "\n   " << string(20, '-') << "\n";
  }
  cout << endl;
}

template <typename T> Move<T> *Memory_UI<T>::get_move(Player<T> *player) {
  if (player->get_type() == PlayerType::RANDOM) {
    Memory_Random_Player<T> *randPlayer =
        dynamic_cast<Memory_Random_Player<T> *>(player);
    if (randPlayer) {
      auto move = randPlayer->get_move();
      cout << player->get_name() << " (Computer) plays at (" << move->get_x()
           << ", " << move->get_y() << ")\n";
      return move;
    }
  }

  int x, y;
  cout << player->get_name() << " (" << player->get_symbol()
       << "), enter row and column (0-2): ";
  cin >> x >> y;

  return new Move<T>(x, y, player->get_symbol());
}

template <typename T> Player<T> **Memory_UI<T>::setup_players() {
  Player<T> **players = new Player<T> *[2];
  vector<string> type_options = {"Human", "Computer"};

  string nameX = this->get_player_name("Player X");
  PlayerType typeX = this->get_player_type_choice("Player X", type_options);

  if (typeX == PlayerType::COMPUTER) {
    players[0] = new Memory_Random_Player<T>(nameX, static_cast<T>('X'));
  } else {
    players[0] = new Player<T>(nameX, static_cast<T>('X'), typeX);
  }

  string nameO = this->get_player_name("Player O");
  PlayerType typeO = this->get_player_type_choice("Player O", type_options);

  if (typeO == PlayerType::COMPUTER) {
    players[1] = new Memory_Random_Player<T>(nameO, static_cast<T>('O'));
  } else {
    players[1] = new Player<T>(nameO, static_cast<T>('O'), typeO);
  }

  return players;
}

template <typename T>
Memory_GameManager<T>::Memory_GameManager(Memory_Board<T> *b, Player<T> *p[2],
                                          Memory_UI<T> *u)
    : GameManager<T>(b, p, u), memBoard(b), memUI(u) {
  players[0] = p[0];
  players[1] = p[1];
}

template <typename T> void Memory_GameManager<T>::run() {
  memUI->display_board_matrix(memBoard->get_board_matrix());

  while (true) {
    for (int i : {0, 1}) {
      Player<T> *currentPlayer = players[i];
      Move<T> *move = memUI->get_move(currentPlayer);

      while (!memBoard->update_board(move))
        move = memUI->get_move(currentPlayer);

      memUI->display_with_last_move(memBoard);

      if (memBoard->is_win(currentPlayer)) {
        memUI->display_final_board(memBoard);
        memUI->display_message(currentPlayer->get_name() + " wins!");
        return;
      }

      if (memBoard->is_draw(currentPlayer)) {
        memUI->display_final_board(memBoard);
        memUI->display_message("Draw!");
        return;
      }
    }
  }
}

#endif

/*
 * Regular 3x3 board but each cell is a 3x3 board -> in total 9x9 board!
 * To win a game you must win 3 (Sub games) in a line or diagonal
 * When a user won we must change the cell to be with his Symbol
 */

#ifndef Ultimate_CLASSES_H
#define Ultimate_CLASSES_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include "Result.h"
using namespace std;

class Ultimate_Board : public Board<char> {
private:
  char blank_symbol =
      '.'; ///< Character used to represent an empty cell on the board.
  int currentSubGame[2] = {0, 0};
  Result_Board *resultBoardptr = new Result_Board;

public:
  Ultimate_Board();
  ~Ultimate_Board() { delete resultBoardptr; }

  bool update_board(Move<char> *move);

  bool is_win(Player<char> *player);

  bool is_lose(Player<char> *) { return false; };

  bool is_draw(Player<char> *player);

  bool game_is_over(Player<char> *player);
  Result_Board *getResultPtr() { return resultBoardptr; }
};

class Ultimate_UI : public UI<char> {
  Result_UI *resultUIptr = new Result_UI;

public:
  Ultimate_UI();

  ~Ultimate_UI() { delete resultUIptr; }

  Player<char> *create_player(string &name, char symbol, PlayerType type);
  void display_board_matrix(const vector<vector<char>> &matrix) const;

  Player<char> **setup_players();

  virtual Move<char> *get_move(Player<char> *player);

  Result_UI *getResultPtr() { return resultUIptr; }
};

class Ultimate : public IGame {

  Ultimate_UI *ui = new Ultimate_UI;

  Ultimate_Board *board = new Ultimate_Board;

  Result_UI *resultUI = ui->getResultPtr();
  Result_Board *resultBoardptr = board->getResultPtr();

public:
  Ultimate();
  ~Ultimate();
  void runGame() override;
  std::string getName() const override;
  std::string getDesc() const override;
  std::pair<int, int> getBoardSize() const override;
  void descripe() const override;
};

class Ultimate_Game_Manager : public GameManager<char> {
  Ultimate_Board *mainBoardptr; ///< game board
  Result_Board *resultBoardptr; ///< game board
  Player<char> *players[2];     ///< two players
  Ultimate_UI *ui;              ///< user interface
  Result_UI *resultUI;          ///< game Board
public:
  Ultimate_Game_Manager(Ultimate_Board *b, Result_Board *rb, Player<char> *p[2],
                        Ultimate_UI *u, Result_UI *ru)
      : GameManager(b, p, u) {
    players[0] = p[0];
    players[1] = p[1];
    players[0]->set_board_ptr(b);
    players[1]->set_board_ptr(b);
    mainBoardptr = b;
    ui = u;
    resultBoardptr = rb;
    resultUI = ru;
  }

  void run() {
    resultUI->display_board_matrix(resultBoardptr->get_board_matrix());
    ui->display_board_matrix(mainBoardptr->get_board_matrix());
    Player<char> *currentPlayer = players[0];

    while (true) {
      for (int i : {0, 1}) {
        currentPlayer = players[i];
        Move<char> *move = ui->get_move(currentPlayer);

        while (!mainBoardptr->update_board(move))
          move = ui->get_move(currentPlayer);
        mainBoardptr->is_win(currentPlayer);

        resultUI->display_board_matrix(resultBoardptr->get_board_matrix());
        ui->display_board_matrix(mainBoardptr->get_board_matrix());

        if (resultBoardptr->is_win(currentPlayer)) {
          ui->display_message(currentPlayer->get_name() + " wins!");
          return;
        }
        if (resultBoardptr->is_lose(currentPlayer)) {
          ui->display_message(players[1 - i]->get_name() + " wins!");
          return;
        }
        if (resultBoardptr->is_draw(currentPlayer)) {
          ui->display_message("Draw!");
          return;
        }
      }
    }
  }
};

#endif // Ultimate_CLASSES_H

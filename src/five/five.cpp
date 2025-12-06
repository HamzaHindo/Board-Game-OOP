#include "five.h"
#include "BoardGame_Classes.h"

using namespace std;

// ===================== Five =====================

Five::Five() {
  ui = new Five_UI();
  board = new Five_Board();
}

Five::~Five() {
  delete board;
  delete ui;
}

void Five::runGame() {

  descripe();
  Player<char> **players = ui->setup_players();
  GameManager<char> game(board, players, ui);
  game.run();

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Five::getName() const { return "MISERE TIC-TAC-TOE"; }
std::string Five::getDesc() const {
  return "Rules: The player who gets 3 in a row LOSES!"
         "Try to force your opponent to make 3 in a row.";
}
std::pair<int, int> Five::getBoardSize() const { return {3, 3}; }

void Five::descripe() const {}

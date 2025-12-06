#include "BoardGame_Classes.h"
#include "Game3_5x5_TicTacToe.h"
#include <iostream>

using namespace std;

// ===================== Game3 =====================

Game3::Game3() {
  ui = new Game3_UI();
  board = new Game3_Board();
}

Game3::~Game3() {
  delete board;
  delete ui;
}

void Game3::runGame() {

  descripe();
  Player<char> **players = ui->setup_players();
  GameManager<char> game(board, players, ui);
  game.run();

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Game3::getName() const { return "MISERE TIC-TAC-TOE"; }
std::string Game3::getDesc() const {
  return "Rules: The player who gets 3 in a row LOSES!"
         "Try to force your opponent to make 3 in a row.";
}
std::pair<int, int> Game3::getBoardSize() const { return {3, 3}; }

void Game3::descripe() const {}

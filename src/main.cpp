/**
 * @file XO_Demo.cpp
 * @brief Entry point for the FCAI X-O (Tic-Tac-Toe) game.
 *
 * This file initializes the X-O game by creating the user interface, board, and
 * players. It then launches the game loop via the GameManager class. All
 * dynamically allocated objects are properly deleted at the end.
 */

#include "Menu/Menu.h"
#include <limits>
#include <memory> // Required for unique_ptr
#include <vector> // Required for vector

#include "Numerical_TicTacToe/Numerical_TicTacToe.h"
#include "XO Game/XO_Classes.h"
#include "word_TicTacToe/word_tic_tac_toe.h"

int main() {

  cin.exceptions(std::ios::failbit | std::ios::badbit);
  srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

  std::vector<std::shared_ptr<IGame>> games;

  games.push_back(std::make_shared<XO>());
  games.push_back(std::make_shared<NumericalTicTacToe>());
  games.push_back(std::make_shared<Word_TTT>());

  Menu menu(games);

  menu.runMenuLoop();

  return 0; // Exit successfully
}

// =====================================================================

/**
 * @file XO_Demo.cpp
 * @brief Entry point for the FCAI X-O (Tic-Tac-Toe) game.
 *
 * This file initializes the X-O game by creating the user interface, board, and
 * players. It then launches the game loop via the GameManager class. All
 * dynamically allocated objects are properly deleted at the end.
 */

#include "BoardGame_Classes.h"
#include "Menu/Menu.h"
#include <iostream> // Required for input/output operations (cout, cin)
#include <memory>   // Required for unique_ptr
#include <string>   // Required for string
#include <vector>   // Required for vector

#include "XO Game/XO_Classes.h"

int main() {

  srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

  std::vector<std::shared_ptr<IGame>> games;

  games.push_back(std::make_shared<XO>());

  Menu menu(games);

  menu.runMenuLoop();

  return 0; // Exit successfully
}

// =====================================================================

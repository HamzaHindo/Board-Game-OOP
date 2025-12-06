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

#include "Infinity/Infinity.h"
#include "Memory/Memory.h"
#include "Numerical/Numerical.h"
#include "Obstacles/Obstacles.h"
#include "Pyramid/Pyramid.h"
#include "SUS/SUS.h"
#include "XO/XO.h"
#include "a_Diamond/DiamondTicTacToe.h"
#include "a_FourInARow/FourInARow.h"
#include "a_Misere/Misere.h"
#include "five/five.h"
#include "word/word.h"

#include <iostream>
using namespace std;
int main() {

  cin.exceptions(std::ios::failbit | std::ios::badbit);
  srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

  std::vector<std::shared_ptr<IGame>> games;

  games.push_back(std::make_shared<SUS>());        // 1
  games.push_back(std::make_shared<FourInARow>()); // 2
  games.push_back(std::make_shared<Five>());
  games.push_back(std::make_shared<Word>()); // 4
  games.push_back(std::make_shared<Misere>());
  games.push_back(std::make_shared<DiamondTicTacToe>()); // 11
  // games.push_back(std::make_shared<Four>());
  games.push_back(std::make_shared<Pyramid>());

  ////////////////// Group Games \\\\\\\\\\\\\\\\\\\\\\\

  games.push_back(std::make_shared<Numerical>()); // 9
  games.push_back(std::make_shared<Obstacles>()); // 10
  games.push_back(std::make_shared<Infinity>());
  // games.push_back(std::make_shared<Ultimate>());
  // games.push_back(std::make_shared<Memory>());
  games.push_back(std::make_shared<XO>()); // Demo
  Menu menu(games);

  menu.runMenuLoop();

  return 0; // Exit successfully
}

// =====================================================================

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

#include "FourInARow/FourInARow.h"
#include "Numerical_TicTacToe/Numerical_TicTacToe.h"
#include "ObstacleTicTacToe/ObstaclesTicTacToe.h"
#include "SUS Game/SUS_Game.h"
#include "XO Game/XO_Classes.h"
#include "word_TicTacToe/word_tic_tac_toe.h"
﻿#include "DiamondTicTacToe.h"
#include "InfinityTicTacToe.h"
int main() {

	cin.exceptions(std::ios::failbit | std::ios::badbit);
	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

	std::vector<std::shared_ptr<IGame>> games;

	games.push_back(std::make_shared<SUS>());                // 1
	games.push_back(std::make_shared<FourInARow>());         // 2
	games.push_back(std::make_shared<Word_TTT>());           // 4
	games.push_back(std::make_shared<NumericalTicTacToe>()); // 9
	games.push_back(std::make_shared<ObstaclesTicTacToe>());// 10
	games.push_back(std::make_shared<DiamondTicTacToe>());   // 11
	games.push_back(std::make_shared<XO>()); // Demo
	games.push_back(std::make_shared<InfinityTicTacToe>());
	Menu menu(games);

	menu.runMenuLoop();

	return 0; // Exit successfully
}

// =====================================================================
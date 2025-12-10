#include <limits>
#include <memory>
#include <vector>

#include "FourX4/FourX4.h"
#include "Infinity/Infinity.h"
#include "Memory/Memory.h"
#include "Menu/Menu.h"
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
  srand(static_cast<unsigned int>(time(0)));

  std::vector<std::shared_ptr<IGame>> games;

  ////////////////// Individual Games \\\\\\\\\\\\\\\\\\\\\\\

  games.push_back(std::make_shared<SUS>());              // 1 Youssef
  games.push_back(std::make_shared<FourInARow>());       // 2 Mohamed
  games.push_back(std::make_shared<Five>());             // 3 Moatasem
  games.push_back(std::make_shared<Word>());             // 4 Hamza
  games.push_back(std::make_shared<Misere>());           // 5 Youssef
  games.push_back(std::make_shared<DiamondTicTacToe>()); // 6 Mohamed
  games.push_back(std::make_shared<FourX4TicTacToe>());  // 7 Moatasem
  games.push_back(std::make_shared<Pyramid>());          // 8 Hamza

  ////////////////// Group Games \\\\\\\\\\\\\\\\\\\\\\\

  games.push_back(std::make_shared<Numerical>()); // 9
  games.push_back(std::make_shared<Obstacles>()); // 10
  games.push_back(std::make_shared<Infinity>());  // 11
  // games.push_back(std::make_shared<Ultimate>());      // 12
  // games.push_back(std::make_shared<Memory>());        // 13
  games.push_back(std::make_shared<XO>()); // Demo
  Menu menu(games);

  menu.runMenuLoop();

  return 0;
}

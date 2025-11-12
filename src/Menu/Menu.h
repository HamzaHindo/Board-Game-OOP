#pragma once
#include "IGame.h"
#include <memory>
#include <vector>

class Menu {
  std::vector<std::shared_ptr<IGame>> games;

public:
  Menu(const std::vector<std::shared_ptr<IGame>> games);

  void runMenuLoop();
};

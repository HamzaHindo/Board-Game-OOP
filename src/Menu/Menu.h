#pragma once
#include <vector>
#include <memory>
#include "IGame.h"

class Menu {
	std::vector<std::shared_ptr<IGame>> games;

public:
	Menu(const std::vector<std::shared_ptr<IGame>> games);

	void runMenuLoop();

};
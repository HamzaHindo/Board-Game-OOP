#pragma once
#include <string>

class IGame {
public:
	virtual std::string getName() const = 0;
	virtual std::pair<int, int> getBoardSize() const = 0;
	virtual std::string getDesc() const = 0;
	virtual void descripe() const = 0;
	virtual void runGame() = 0;
};
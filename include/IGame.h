#pragma once
#include <string>

class IGame {
private:
	std::string gameName;
	int boardSize;
	std::string gameDesc;
public:
	IGame(std::string name, int size, std::string desc="");
	std::string getName() const;
	int getBoardSize() const;
	std::string getDesc() const;
	void descripe() const;
	virtual void runGame() = 0;
};
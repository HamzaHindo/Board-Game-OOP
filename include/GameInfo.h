#pragma once


class GameInfo {
private:
	std::string gameName;
	int boardSize;
	std::string gameDesc;
public:
	GameInfo(std::string name, int size, std::string desc);
	std::string getName() const;
	int getSize() const;
	int getDesc() const;
};
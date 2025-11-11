#include "IGame.h"
#include <iostream>
#include <string>

IGame::IGame(std::string name, int size, std::string desc)
    : gameName(name), boardSize(size), gameDesc(desc) {
    if (gameDesc == "") {
        gameDesc = gameName;
    }
}

std::string IGame::getName() const {
    return gameName;
}

int IGame::getBoardSize() const {
    return boardSize;
}

std::string IGame::getDesc() const {
    return gameDesc;
}

void IGame::descripe() const {
    std::cout << "----- " << gameName << " -----\n";
    std::cout << gameDesc << "\n\n";
}

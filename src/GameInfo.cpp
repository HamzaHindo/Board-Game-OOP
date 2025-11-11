#include "GameInfo.h"


GameInfo::GameInfo(std::string name, int size, std::string desc = "") : gameName(name), gameSize(size), gameDesc(desc);


std::string GameInfo::getName() const {
	return gameName;
}

std::string GameInfo::getSize() const {
	return gameSize;
}

std::string GameInfo::getDesc() const {
	return gameDesc;
}
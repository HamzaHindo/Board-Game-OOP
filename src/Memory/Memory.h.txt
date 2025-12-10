#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// Forward declaration
class Memory_Board;

class Memory_Board : public Board<char> {
private:
	vector<vector<bool>> hidden;
	int last_move_x, last_move_y;

public:
	Memory_Board();
	bool update_board(Move<char>* move) override;
	bool is_win(Player<char>* player) override;
	bool is_lose(Player<char>*) override { return false; }
	bool is_draw(Player<char>* player) override;
	bool game_is_over(Player<char>* player) override;
	bool is_hidden(int x, int y) const;
	pair<int, int> get_last_move() const;
	void reveal_board();
};

class Memory_AI {
public:
	pair<int, int> selectMove(Memory_Board* board);
};

class Memory_UI : public UI<char> {
public:
	Memory_UI();
	void display_board_matrix(const vector<vector<char>>& matrix) const override;
	void display_with_last_move(Memory_Board* board);
	void display_final_board(Memory_Board* board);
	Move<char>* get_move(Player<char>* player) override;
	Player<char>** setup_players() override;
};

class MemoryTicTacToe : public IGame {
private:
	Memory_UI* ui;
	Memory_Board* board;

public:
	MemoryTicTacToe();
	~MemoryTicTacToe();
	void runGame() override;
	std::string getName() const override;
	std::string getDesc() const override;
	std::pair<int, int> getBoardSize() const override;
	void descripe() const override;
};

#endif
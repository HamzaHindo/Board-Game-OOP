#ifndef FOUR_IN_A_ROW_H
#define FOUR_IN_A_ROW_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include "FourInARow_AI.h"
#include <vector>
#include <string>

using namespace std;

class FourInARow_Board : public Board<char> {
private:
	char blank_symbol = '.';
	const int WIN_LENGTH = 4;
	vector<int> column_heights;

	bool check_horizontal_win(int row, int col, char sym) const;
	bool check_vertical_win(int row, int col, char sym) const;
	bool check_diagonal_win(int row, int col, char sym) const;
	bool check_anti_diagonal_win(int row, int col, char sym) const;

public:
	FourInARow_Board();
	bool update_board(Move<char>* move);
	bool is_win(Player<char>* player);
	bool is_lose(Player<char>*) { return false; }
	bool is_draw(Player<char>* player);
	bool game_is_over(Player<char>* player);
	bool is_valid_column(int column) const;
	int get_next_row(int column) const;
	bool can_place_in_column(int column) const;
};

class FourInARow_UI : public UI<char> {
private:
	GameAIStrategy* ai_strategy;

public:
	FourInARow_UI(GameAIStrategy* strategy = nullptr);
	~FourInARow_UI();
	Player<char>* create_player(string& name, char symbol, PlayerType type);
	virtual Move<char>* get_move(Player<char>* player);
	void display_board_matrix(const vector<vector<char>>& matrix) const;
	void set_ai_strategy(GameAIStrategy* strategy) { ai_strategy = strategy; }

private:
	bool is_valid_column_input(int column) const;
};

class FourInARow : public IGame {
private:
	FourInARow_UI* ui;
	FourInARow_Board* board;
	MinMaxAI* ai_strategy;

public:
	FourInARow();
	~FourInARow();
	void runGame() override;
	std::string getName() const override;
	std::string getDesc() const override;
	std::pair<int, int> getBoardSize() const override;
	void descripe() const override;
};

#endif
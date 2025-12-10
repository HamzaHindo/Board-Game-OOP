#ifndef DIAMOND_TICTACTOE_H
#define DIAMOND_TICTACTOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include "DiamondAI_Backtracking.h"
#include <string>
#include <vector>
#include <utility>

using namespace std;

class LineValidator {
private:
	const vector<vector<char>>* board;
	static const int LINE_3 = 3;
	static const int LINE_4 = 4;

public:
	LineValidator(const vector<vector<char>>* b) : board(b) {}

	bool check_line(int row, int col, int dr, int dc, char mark, int length) const;
	int count_line_length(int row, int col, int dr, int dc, char mark) const;
	bool has_line_of_length(int row, int col, char mark, int length) const;
};

class DiamondGridValidator {
private:
	static const int GRID_SIZE = 7;

public:
	bool is_valid_position(int row, int col) const;
	bool is_in_diamond(int row, int col) const;
	static const int SIZE;
};

class Diamond_Board : public Board<char> {
private:
	LineValidator line_validator;
	DiamondGridValidator grid_validator;

	bool has_line_3_and_4(int row, int col, char mark) const;
	vector<pair<int, int>> get_all_neighbors(int row, int col) const;

public:
	Diamond_Board();
	bool update_board(Move<char>* move) override;
	bool is_win(Player<char>* player) override;
	bool is_lose(Player<char>*) override { return false; }
	bool is_draw(Player<char>* player) override;
	bool game_is_over(Player<char>* player) override;

	const DiamondGridValidator& get_grid_validator() const { return grid_validator; }
	const LineValidator& get_line_validator() const { return line_validator; }
};

class Diamond_UI : public UI<char> {
private:
	AIStrategy* ai_strategy;

public:
	Diamond_UI(AIStrategy* strategy = nullptr);
	~Diamond_UI();
	Player<char>* create_player(string& name, char symbol, PlayerType type);
	virtual Move<char>* get_move(Player<char>* player);
	void display_board_matrix(const vector<vector<char>>& matrix) const;
	void set_ai_strategy(AIStrategy* strategy) { ai_strategy = strategy; }

private:
	bool is_valid_input(int row, int col, const Diamond_Board* board);
};

class DiamondTicTacToe : public IGame {
private:
	Diamond_UI* ui;
	Diamond_Board* board;

	void play_turn(Player<char>* player);
	bool check_game_end(Player<char>* current_player, bool& game_over);

public:
	DiamondTicTacToe();
	~DiamondTicTacToe();
	void runGame() override;
	std::string getName() const override;
	std::string getDesc() const override;
	std::pair<int, int> getBoardSize() const override;
	void descripe() const override;
};

#endif
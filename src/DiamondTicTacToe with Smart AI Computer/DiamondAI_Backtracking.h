#ifndef DIAMOND_AI_BACKTRACKING_H
#define DIAMOND_AI_BACKTRACKING_H

#include <vector>
#include <utility>

using namespace std;

class Diamond_Board;

class AIStrategy {
public:
	virtual ~AIStrategy() {}
	virtual void selectMove(Diamond_Board* board, char playerMark, int& row, int& col) = 0;
};

class BacktrackingAI : public AIStrategy {
private:
	const int MAX_DEPTH = 6;
	vector<pair<int, int>> available_moves;

	void find_available_moves(Diamond_Board* board);
	pair<int, pair<int, int>> minimax(Diamond_Board* board, int depth, bool is_maximizing,
		char ai_mark, char opponent_mark);
	int evaluate_board(Diamond_Board* board, char ai_mark, char opponent_mark);
	bool is_winning_position(Diamond_Board* board, int row, int col, char mark);
	bool is_blocking_position(Diamond_Board* board, int row, int col,
		char opponent_mark, char ai_mark);

public:
	BacktrackingAI() = default;
	virtual ~BacktrackingAI() {}
	void selectMove(Diamond_Board* board, char playerMark, int& row, int& col) override;
};

#endif
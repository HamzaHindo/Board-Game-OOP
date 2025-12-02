#ifndef FOUR_IN_ROW_AI_H
#define FOUR_IN_ROW_AI_H

#include <vector>
#include <utility>

using namespace std;

class Diamond_Board;
class FourInARow_Board;

class GameAIStrategy {
public:
	virtual ~GameAIStrategy() {}
	virtual void selectMove(FourInARow_Board* board, char playerMark, int& column) = 0;
};

class MinMaxAI : public GameAIStrategy {
private:
	const int MAX_DEPTH = 7;
	const int WINDOW_SIZE = 4;

	pair<int, int> minimax(FourInARow_Board* board, int depth, bool is_maximizing,
		char ai_mark, char opponent_mark, int alpha, int beta);

	int evaluate_position(FourInARow_Board* board, char ai_mark, char opponent_mark);

	int count_line_strength(FourInARow_Board* board, int row, int col, int dr, int dc, char mark);

	int evaluate_window(char mark, int count_mark, int count_empty, int count_opponent);

	bool can_place_in_column(FourInARow_Board* board, int column);

	int get_next_row(FourInARow_Board* board, int column);

public:
	MinMaxAI() = default;
	virtual ~MinMaxAI() {}
	void selectMove(FourInARow_Board* board, char playerMark, int& column) override;
};

#endif
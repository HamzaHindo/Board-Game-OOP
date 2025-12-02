#include "FourInARow_AI.h"
#include "FourInARow.h"
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

bool MinMaxAI::can_place_in_column(FourInARow_Board* board, int column) {
	return board->can_place_in_column(column);
}

int MinMaxAI::get_next_row(FourInARow_Board* board, int column) {
	return board->get_next_row(column);
}

int MinMaxAI::count_line_strength(FourInARow_Board* board, int row, int col,
	int dr, int dc, char mark) {
	const auto& board_matrix = board->get_board_matrix();
	int count = 1;

	int r = row + dr;
	int c = col + dc;
	while (r >= 0 && r < 6 && c >= 0 && c < 7 && board_matrix[r][c] == mark) {
		count++;
		r += dr;
		c += dc;
	}

	r = row - dr;
	c = col - dc;
	while (r >= 0 && r < 6 && c >= 0 && c < 7 && board_matrix[r][c] == mark) {
		count++;
		r -= dr;
		c -= dc;
	}

	return count;
}

int MinMaxAI::evaluate_window(char mark, int count_mark, int count_empty, int count_opponent) {
	int score = 0;

	if (count_mark == 4) return 10000;
	else if (count_mark == 3 && count_empty == 1) return 500;
	else if (count_mark == 3) return 50;
	else if (count_mark == 2 && count_empty == 2) return 100;
	else if (count_mark == 2) return 10;
	else if (count_mark == 1 && count_empty == 3) return 5;

	if (count_opponent == 3 && count_empty == 1) return -400;
	else if (count_opponent == 3) return -40;
	else if (count_opponent == 2 && count_empty == 2) return -80;
	else if (count_opponent == 2) return -8;

	return 0;
}

int MinMaxAI::evaluate_position(FourInARow_Board* board, char ai_mark, char opponent_mark) {
	const auto& board_matrix = board->get_board_matrix();
	int score = 0;
	int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 7; ++j) {
			if (board_matrix[i][j] == '.')
				continue;

			for (auto& dir : directions) {
				int line_len = count_line_strength(board, i, j, dir[0], dir[1], board_matrix[i][j]);

				if (board_matrix[i][j] == ai_mark) {
					if (line_len >= 4) return 10000;
					if (line_len == 3) score += 500;
					if (line_len == 2) score += 50;
				}
				else if (board_matrix[i][j] == opponent_mark) {
					if (line_len >= 4) return -10000;
					if (line_len == 3) score -= 400;
					if (line_len == 2) score -= 50;
				}
			}
		}
	}

	return score;
}

pair<int, int> MinMaxAI::minimax(FourInARow_Board* board, int depth, bool is_maximizing,
	char ai_mark, char opponent_mark, int alpha, int beta) {
	char current_mark = is_maximizing ? ai_mark : opponent_mark;

	int best_score = is_maximizing ? INT_MIN : INT_MAX;
	int best_column = -1;

	int eval = evaluate_position(board, ai_mark, opponent_mark);
	if (eval >= 10000 || eval <= -10000) {
		return make_pair(eval, 0);
	}

	if (depth == 0) {
		return make_pair(eval, 0);
	}

	for (int col = 0; col < 7; ++col) {
		if (!can_place_in_column(board, col))
			continue;

		int row = get_next_row(board, col);
		if (row < 0) continue;

		board->get_board_matrix()[row][col] = current_mark;

		pair<int, int> result = minimax(board, depth - 1, !is_maximizing, ai_mark, opponent_mark, alpha, beta);
		int score = result.first;

		board->get_board_matrix()[row][col] = '.';

		if (is_maximizing) {
			if (score > best_score) {
				best_score = score;
				best_column = col;
				alpha = max(alpha, score);
			}
		}
		else {
			if (score < best_score) {
				best_score = score;
				best_column = col;
				beta = min(beta, score);
			}
		}

		if (beta <= alpha)
			break;
	}

	if (best_column == -1) {
		for (int col = 0; col < 7; ++col) {
			if (can_place_in_column(board, col)) {
				best_column = col;
				break;
			}
		}
	}

	return make_pair(best_score, best_column);
}

void MinMaxAI::selectMove(FourInARow_Board* board, char playerMark, int& column) {
	char opponent_mark = (playerMark == 'X') ? 'O' : 'X';

	pair<int, int> result = minimax(board, MAX_DEPTH, true, playerMark, opponent_mark,
		INT_MIN, INT_MAX);
	column = result.second;

	if (column == -1) {
		for (int col = 0; col < 7; ++col) {
			if (can_place_in_column(board, col)) {
				column = col;
				break;
			}
		}
	}
}
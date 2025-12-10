#include "DiamondAI_Backtracking.h"
#include "DiamondTicTacToe.h"
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

void BacktrackingAI::find_available_moves(Diamond_Board* board) {
	available_moves.clear();
	const auto& grid_validator = board->get_grid_validator();
	const auto& board_matrix = board->get_board_matrix();

	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			if (grid_validator.is_valid_position(i, j) && board_matrix[i][j] == '.') {
				available_moves.push_back({ i, j });
			}
		}
	}
}

bool BacktrackingAI::is_winning_position(Diamond_Board* board, int row, int col, char mark) {
	auto& line_validator = board->get_line_validator();
	int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

	vector<int> line_lengths;
	for (int i = 0; i < 4; ++i) {
		int len = line_validator.count_line_length(row, col, directions[i][0],
			directions[i][1], mark);
		line_lengths.push_back(len);
	}

	int count_3_or_more = 0, count_4_or_more = 0;
	for (int i = 0; i < 4; ++i) {
		if (line_lengths[i] >= 3) count_3_or_more++;
		if (line_lengths[i] >= 4) count_4_or_more++;
	}

	return (count_3_or_more >= 1 && count_4_or_more >= 1);
}

bool BacktrackingAI::is_blocking_position(Diamond_Board* board, int row, int col,
	char opponent_mark, char ai_mark) {
	auto& line_validator = board->get_line_validator();
	int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

	vector<int> opponent_lines;
	for (int i = 0; i < 4; ++i) {
		int len = line_validator.count_line_length(row, col, directions[i][0],
			directions[i][1], opponent_mark);
		opponent_lines.push_back(len);
	}

	int dangerous_lines = 0;
	for (int i = 0; i < 4; ++i) {
		if (opponent_lines[i] >= 2)
			dangerous_lines++;
	}

	return dangerous_lines >= 1;
}

int BacktrackingAI::evaluate_board(Diamond_Board* board, char ai_mark, char opponent_mark) {
	const auto& board_matrix = board->get_board_matrix();
	const auto& grid_validator = board->get_grid_validator();
	int score = 0;

	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			if (!grid_validator.is_valid_position(i, j))
				continue;

			if (board_matrix[i][j] == ai_mark) {
				if (is_winning_position(board, i, j, ai_mark))
					return 1000;
			}
			else if (board_matrix[i][j] == opponent_mark) {
				if (is_winning_position(board, i, j, opponent_mark))
					return -1000;
				else if (is_blocking_position(board, i, j, opponent_mark, ai_mark))
					score -= 50;
			}
		}
	}

	return score;
}

pair<int, pair<int, int>> BacktrackingAI::minimax(Diamond_Board* board, int depth,
	bool is_maximizing, char ai_mark, char opponent_mark, int alpha, int beta) {
	char current_mark = is_maximizing ? ai_mark : opponent_mark;

	int best_score = is_maximizing ? INT_MIN : INT_MAX;
	pair<int, int> best_move = make_pair(-1, -1);

	find_available_moves(board);

	if (depth == 0 || available_moves.empty()) {
		int score = evaluate_board(board, ai_mark, opponent_mark);
		return make_pair(score, best_move);
	}

	for (auto& move : available_moves) {
		int row = move.first;
		int col = move.second;

		board->get_board_matrix()[row][col] = current_mark;

		pair<int, pair<int, int>> result = minimax(board, depth - 1, !is_maximizing, ai_mark, opponent_mark, alpha, beta);
		int score = result.first;

		board->get_board_matrix()[row][col] = '.';

		if (is_maximizing) {
			if (score > best_score) {
				best_score = score;
				best_move = make_pair(row, col);
				alpha = max(alpha, score);
			}
		}
		else {
			if (score < best_score) {
				best_score = score;
				best_move = make_pair(row, col);
				beta = min(beta, score);
			}
		}

		if (beta <= alpha) {
			break;
		}
	}

	return make_pair(best_score, best_move);
}

void BacktrackingAI::selectMove(Diamond_Board* board, char playerMark, int& row, int& col) {
	char opponent_mark = (playerMark == 'X') ? 'O' : 'X';

	pair<int, pair<int, int>> result = minimax(board, MAX_DEPTH, true, playerMark, opponent_mark, INT_MIN, INT_MAX);
	pair<int, int> best_move = result.second;

	if (best_move.first == -1 || best_move.second == -1) {
		find_available_moves(board);
		if (!available_moves.empty()) {
			best_move = available_moves[rand() % available_moves.size()];
		}
	}

	row = best_move.first;
	col = best_move.second;
}
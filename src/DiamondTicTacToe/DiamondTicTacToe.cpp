#include "DiamondTicTacToe.h"
#include <iostream>
#include <cstdlib>

using namespace std;

const int DiamondGridValidator::SIZE = 5;

bool DiamondGridValidator::is_in_diamond(int row, int col) const {
	if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
		return false;

	int center = GRID_SIZE / 2;
	int distance = abs(row - center) + abs(col - center);
	return distance <= center;
}

bool DiamondGridValidator::is_valid_position(int row, int col) const {
	return is_in_diamond(row, col);
}

bool LineValidator::check_line(int row, int col, int dr, int dc, char mark, int length) const {
	for (int i = 0; i < length; ++i) {
		int r = row + i * dr;
		int c = col + i * dc;

		if (r < 0 || r >= 5 || c < 0 || c >= 5)
			return false;

		if ((*board)[r][c] != mark)
			return false;
	}
	return true;
}

int LineValidator::count_line_length(int row, int col, int dr, int dc, char mark) const {
	int length = 1;

	int r = row + dr;
	int c = col + dc;
	while (r >= 0 && r < 5 && c >= 0 && c < 5 && (*board)[r][c] == mark) {
		length++;
		r += dr;
		c += dc;
	}

	r = row - dr;
	c = col - dc;
	while (r >= 0 && r < 5 && c >= 0 && c < 5 && (*board)[r][c] == mark) {
		length++;
		r -= dr;
		c -= dc;
	}

	return length;
}

bool LineValidator::has_line_of_length(int row, int col, char mark, int length) const {
	int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

	for (auto& dir : directions) {
		if (check_line(row, col, dir[0], dir[1], mark, length))
			return true;
	}
	return false;
}

Diamond_Board::Diamond_Board()
	: Board<char>(5, 5),
	line_validator(&board),
	grid_validator() {

	for (auto& row : board)
		for (auto& cell : row)
			cell = '.';
}

bool Diamond_Board::update_board(Move<char>* move) {
	int row = move->get_x();
	int col = move->get_y();
	char mark = move->get_symbol();

	if (!grid_validator.is_valid_position(row, col))
		return false;

	if (board[row][col] != '.')
		return false;

	board[row][col] = toupper(mark);
	n_moves++;
	return true;
}

bool Diamond_Board::has_line_3_and_4(int row, int col, char mark) const {
	int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

	vector<int> line_lengths;
	for (int i = 0; i < 4; ++i) {
		int len = line_validator.count_line_length(row, col, directions[i][0], directions[i][1], mark);
		line_lengths.push_back(len);
	}

	int count_3_or_more = 0;
	int count_4_or_more = 0;
	bool has_different_directions = false;

	for (int i = 0; i < 4; ++i) {
		if (line_lengths[i] >= 3)
			count_3_or_more++;
		if (line_lengths[i] >= 4)
			count_4_or_more++;
	}

	
	if (count_3_or_more >= 1 && count_4_or_more >= 1) {
	
		for (int i = 0; i < 4; ++i) {
			if (line_lengths[i] >= 3) {
				for (int j = i + 1; j < 4; ++j) {
					if (line_lengths[j] >= 3 && i != j) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Diamond_Board::is_win(Player<char>* player) {
	char mark = player->get_symbol();

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (board[i][j] == mark && has_line_3_and_4(i, j, mark))
				return true;
		}
	}
	return false;
}

bool Diamond_Board::is_draw(Player<char>* player) {
	if (is_win(player))
		return false;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (grid_validator.is_valid_position(i, j) && board[i][j] == '.')
				return false;
		}
	}
	return true;
}

bool Diamond_Board::game_is_over(Player<char>* player) {
	return is_win(player) || is_draw(player);
}

Diamond_UI::Diamond_UI() : UI<char>(2) {}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
	string type_str = (type == PlayerType::HUMAN) ? "human" : "computer";
	cout << "Creating " << type_str << " player: " << name << " (" << symbol << ")\n";
	return new Player<char>(name, symbol, type);
}

bool Diamond_UI::is_valid_input(int row, int col, const Diamond_Board* board) {
	if (!board->get_grid_validator().is_valid_position(row, col)) {
		cout << "Position not in diamond! Choose valid position.\n";
		return false;
	}

	if (board->get_board_matrix()[row][col] != '.') {
		cout << "Position already occupied!\n";
		return false;
	}

	return true;
}

void Diamond_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
	cout << "\n";

	for (int i = 0; i < 5; ++i) {
		int spaces_before = abs(2 - i) * 2;
		for (int j = 0; j < spaces_before; ++j)
			cout << " ";

		for (int j = 0; j < 5; ++j) {
			int center = 2;
			if (abs(i - center) + abs(j - center) <= center) {
				cout << matrix[i][j] << " ";
			}
		}
		cout << "\n";
	}
	cout << "\n";
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
	Diamond_Board* dia_board = static_cast<Diamond_Board*>(player->get_board_ptr());

	if (player->get_type() == PlayerType::HUMAN) {
		cout << "\n" << player->get_name() << "'s Turn (" << player->get_symbol() << ")\n";

		int row, col;
		while (true) {
			cout << "Enter position (row col) [0-4]: ";
			cin >> row >> col;
			if (is_valid_input(row, col, dia_board))
				break;
		}
		return new Move<char>(row, col, player->get_symbol());
	}
	else {
		int row, col;
		do {
			row = rand() % 5;
			col = rand() % 5;
		} while (!dia_board->get_grid_validator().is_valid_position(row, col) ||
			dia_board->get_board_matrix()[row][col] != '.');

		cout << "\nComputer places " << player->get_symbol()
			<< " at (" << row << ", " << col << ")\n";
		return new Move<char>(row, col, player->get_symbol());
	}
}

DiamondTicTacToe::DiamondTicTacToe() {
	ui = new Diamond_UI();
	board = new Diamond_Board();
}

DiamondTicTacToe::~DiamondTicTacToe() {
	delete board;
	delete ui;
}

void DiamondTicTacToe::play_turn(Player<char>* player) {
	Move<char>* move = ui->get_move(player);
	while (!board->update_board(move)) {
		delete move;
		move = ui->get_move(player);
	}
	delete move;
}

bool DiamondTicTacToe::check_game_end(Player<char>* current_player, bool& game_over) {
	if (board->is_win(current_player)) {
		ui->display_message(current_player->get_name() + " wins by completing 3+4 lines!");
		game_over = true;
		return true;
	}

	if (board->is_draw(current_player)) {
		ui->display_message("Draw!");
		game_over = true;
		return true;
	}

	return false;
}

void DiamondTicTacToe::runGame() {
	descripe();

	string nameP1, nameP2;
	cout << "Enter Player 1 name: ";
	getline(cin >> ws, nameP1);
	cout << "Enter Player 2 name: ";
	getline(cin >> ws, nameP2);

	cout << "\nChoose Player 1 type:\n1. Human\n2. Computer\n==> ";
	int choice1;
	cin >> choice1;
	PlayerType type1 = (choice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

	cout << "Choose Player 2 type:\n1. Human\n2. Computer\n==> ";
	int choice2;
	cin >> choice2;
	PlayerType type2 = (choice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

	Player<char>** players = new Player<char>*[2];
	players[0] = ui->create_player(nameP1, 'X', type1);
	players[1] = ui->create_player(nameP2, 'O', type2);

	players[0]->set_board_ptr(board);
	players[1]->set_board_ptr(board);

	ui->display_board_matrix(board->get_board_matrix());

	bool game_over = false;
	while (!game_over) {
		for (int i = 0; i < 2; ++i) {
			play_turn(players[i]);
			ui->display_board_matrix(board->get_board_matrix());

			if (check_game_end(players[i], game_over))
				break;
		}
	}

	for (int i = 0; i < 2; ++i) {
		delete players[i];
	}
	delete[] players;
}

std::string DiamondTicTacToe::getName() const {
	return "Diamond Tic-Tac-Toe";
}

std::string DiamondTicTacToe::getDesc() const {
	return "Diamond Tic-Tac-Toe - Unique Strategy Game!\n"
		"5x5 diamond-shaped grid\n"
		"Win by completing BOTH a line of 3 AND a line of 4\n"
		"Lines must be in different directions!\n"
		"Lines can share one common mark.";
}

std::pair<int, int> DiamondTicTacToe::getBoardSize() const {
	return { 5, 5 };
}

void DiamondTicTacToe::descripe() const {
	cout << "\n========================================\n"
		<< getDesc() << "\n"
		<< "Board Size: (" << getBoardSize().first << ", "
		<< getBoardSize().second << ")\n"
		<< "========================================\n\n";
}
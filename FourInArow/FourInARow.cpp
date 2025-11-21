#include "FourInARow.h"
#include <iostream>
#include <iomanip>

using namespace std;

FourInARow_Board::FourInARow_Board() : Board<char>(6, 7) {
	column_heights.resize(7, 0);

	for (auto& row : board)
		for (auto& cell : row)
			cell = blank_symbol;
}

bool FourInARow_Board::is_valid_column(int column) const {
	return column >= 0 && column < columns;
}

bool FourInARow_Board::can_place_in_column(int column) const {
	if (!is_valid_column(column))
		return false;
	return column_heights[column] < rows;
}

int FourInARow_Board::get_next_row(int column) const {
	if (!is_valid_column(column))
		return -1;
	return rows - 1 - column_heights[column];
}

bool FourInARow_Board::update_board(Move<char>* move) {
	int column = move->get_y();
	char mark = move->get_symbol();

	if (!is_valid_column(column))
		return false;

	if (!can_place_in_column(column))
		return false;

	int row = get_next_row(column);
	if (row < 0)
		return false;

	board[row][column] = toupper(mark);
	column_heights[column]++;
	n_moves++;

	return true;
}

bool FourInARow_Board::check_horizontal_win(int row, int col, char sym) const {
	if (col + WIN_LENGTH > columns)
		return false;
	for (int k = 0; k < WIN_LENGTH; ++k) {
		if (board[row][col + k] != sym)
			return false;
	}
	return true;
}

bool FourInARow_Board::check_vertical_win(int row, int col, char sym) const {
	if (row + WIN_LENGTH > rows)
		return false;
	for (int k = 0; k < WIN_LENGTH; ++k) {
		if (board[row + k][col] != sym)
			return false;
	}
	return true;
}

bool FourInARow_Board::check_diagonal_win(int row, int col, char sym) const {
	if (row + WIN_LENGTH > rows || col + WIN_LENGTH > columns)
		return false;
	for (int k = 0; k < WIN_LENGTH; ++k) {
		if (board[row + k][col + k] != sym)
			return false;
	}
	return true;
}

bool FourInARow_Board::check_anti_diagonal_win(int row, int col, char sym) const {
	if (row + WIN_LENGTH > rows || col - WIN_LENGTH + 1 < 0)
		return false;
	for (int k = 0; k < WIN_LENGTH; ++k) {
		if (board[row + k][col - k] != sym)
			return false;
	}
	return true;
}

bool FourInARow_Board::is_win(Player<char>* player) {
	char sym = player->get_symbol();

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (board[i][j] != sym)
				continue;

			if (check_horizontal_win(i, j, sym) ||
				check_vertical_win(i, j, sym) ||
				check_diagonal_win(i, j, sym) ||
				check_anti_diagonal_win(i, j, sym)) {
				return true;
			}
		}
	}

	return false;
}

bool FourInARow_Board::is_draw(Player<char>* player) {
	if (is_win(player))
		return false;

	for (int i = 0; i < columns; ++i) {
		if (can_place_in_column(i))
			return false;
	}
	return true;
}

bool FourInARow_Board::game_is_over(Player<char>* player) {
	return is_win(player) || is_draw(player);
}

FourInARow_UI::FourInARow_UI() : UI<char>(3) {}

Player<char>* FourInARow_UI::create_player(string& name, char symbol,
	PlayerType type) {
	string actual_type = (type == PlayerType::HUMAN) ? "human" : "computer";

	cout << "Creating " << actual_type << " player: " << name << " (" << symbol
		<< ")\n";

	return new Player<char>(name, symbol, type);
}

bool FourInARow_UI::is_valid_column_input(int column) const {
	return column >= 0 && column < 7;
}



Move<char>* FourInARow_UI::get_move(Player<char>* player) {
	FourInARow_Board* game_board =
		static_cast<FourInARow_Board*>(player->get_board_ptr());

	if (player->get_type() == PlayerType::HUMAN) {
		cout << "\n" << player->get_name() << "'s Turn (" << player->get_symbol()
			<< ")\n";

		int column;
		while (true) {
			cout << "Enter column (0-6): ";
			cin >> column;

			if (!is_valid_column_input(column)) {
				cout << "Invalid column! Please enter values between 0 and 6.\n";
				continue;
			}

			if (!game_board->can_place_in_column(column)) {
				cout << "Column " << column << " is full! Choose another.\n";
				continue;
			}

			break;
		}
		return new Move<char>(0, column, player->get_symbol());
	}
	else if (player->get_type() == PlayerType::COMPUTER) {
		int column;
		do {
			column = rand() % 7;
		} while (!game_board->can_place_in_column(column));

		cout << "\nComputer (" << player->get_name() << ") places "
			<< player->get_symbol() << " at column " << column << "\n";

		return new Move<char>(0, column, player->get_symbol());
	}

	return new Move<char>(0, 0, player->get_symbol());
}

FourInARow::FourInARow() {
	ui = new FourInARow_UI();
	board = new FourInARow_Board();
}

FourInARow::~FourInARow() {
	delete board;
	delete ui;
}

void FourInARow::runGame() {
	descripe();

	string nameP1;
	cout << "Enter Player 1 name: ";
	getline(cin >> ws, nameP1);

	string nameP2;
	cout << "Enter Player 2 name: ";
	getline(cin >> ws, nameP2);

	cout << "\nChoose Player 1 type:\n1. Human\n2. Computer\n==> ";
	int typeChoice1;
	cin >> typeChoice1;
	PlayerType type1 =
		(typeChoice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

	cout << "Choose Player 2 type:\n1. Human\n2. Computer\n==> ";
	int typeChoice2;
	cin >> typeChoice2;
	PlayerType type2 =
		(typeChoice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

	Player<char>** players = new Player<char>*[2];
	players[0] = ui->create_player(nameP1, 'X', type1);
	players[1] = ui->create_player(nameP2, 'O', type2);

	GameManager<char> game(board, players, ui);
	game.run();

	for (int i = 0; i < 2; ++i) {
		delete players[i];
	}
	delete[] players;
}

std::string FourInARow::getName() const { return "Four-in-a-Row"; }

std::string FourInARow::getDesc() const {
	return "Four-in-a-Row - Connect Four Battle!\n"
		"Players take turns placing their mark in the lowest available "
		"square of any column.\n"
		"First player to get four marks in a row (horizontally, vertically, "
		"or diagonally) wins!";
}

std::pair<int, int> FourInARow::getBoardSize() const { return { 6, 7 }; }

void FourInARow::descripe() const {
	std::cout << "\n========================================\n";
	std::cout << getDesc() << "\n";
	std::cout << "Board Size: (" << getBoardSize().first << ", "
		<< getBoardSize().second << ")\n";
	std::cout << "========================================\n\n";
}

//--------------------------------------- IMPLEMENTATION

#include "Pyramid.h"
#include <cctype>
#include <iomanip>
#include <iostream>

using namespace std;

//--------------------------------------- Pyramid_Board Implementation

Pyramid_Board::Pyramid_Board() : Board(3, 5) {
  // Initialize all cells with blank_symbol
  board.resize(3);
  for (int i = 0; i < 3; i++) {
    board[i].resize(2 * i + 1);
    for (int j = 0; j < 2 * i + 1; j++) {
      board[i][j] = blank_symbol;
    }
  }
}

bool Pyramid_Board::update_board(Move<char> *move) {
  int x = move->get_x();
  int y = move->get_y();
  char mark = move->get_symbol();

  y = y - (2 - x);
  if (!(x < 0 || x >= board.size() || y < 0 || y >= board[x].size()) &&
      (board[x][y] == blank_symbol || mark == 0)) {

    if (mark == 0) { // Undo move
      n_moves--;
      board[x][y] = blank_symbol;
    } else { // Apply move
      n_moves++;
      board[x][y] = toupper(mark);
    }
    return true;
  }
  return false;
}

bool Pyramid_Board::is_win(Player<char> *player) {
  auto all_equal = [&](char a, char b, char c) {
    return a == b && b == c && a != blank_symbol;
  };
  if (all_equal(board[0][0], board[1][0], board[2][0]) ||
      all_equal(board[0][0], board[1][2], board[2][4]) ||
      all_equal(board[0][0], board[1][1], board[2][2]))
    return true;

  for (int i = 1; i < 3; i++) {
    for (int j = 0; j < board[i].size() - 2; j++) {
      if (all_equal(board[i][j], board[i][j + 1], board[i][j + 2]))
        return true;
    }
  }

  return false;
}

bool Pyramid_Board::is_draw(Player<char> *player) {
  return (n_moves == 9 && !is_win(player));
}

bool Pyramid_Board::game_is_over(Player<char> *player) {
  return is_win(player) || is_draw(player);
}

//--------------------------------------- Pyramid_UI Implementation

Pyramid_UI::Pyramid_UI() : UI<char>(3) {}

Player<char> *Pyramid_UI::create_player(string &name, char symbol,
                                        PlayerType type) {
  // Create player based on type
  cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";

  return new Player<char>(name, symbol, type);
}

void Pyramid_UI::display_board_matrix(
    const vector<vector<char>> &matrix) const {
  if (matrix.empty() || matrix[0].empty())
    return;

  int rows = matrix.size();
  int max_cells = 2 * (rows - 1) + 1; // number of cells in the last row
  int total_width =
      max_cells * (cell_width + 2) + 1; // total width for centering

  // Print column headers
  cout << "\n   ";
  for (int j = 0; j < max_cells; ++j) {
    cout << setw(cell_width + 1) << j;
  }
  cout << "\n   " << string(total_width, '-') << "\n";

  for (int i = 0; i < rows; ++i) {
    int cells_in_row = 2 * i + 1;
    int leading_spaces = (max_cells - cells_in_row) / 2;

    cout << setw(2) << i << " ";
    // Print leading spaces for centering
    cout << string(leading_spaces * (cell_width + 1), ' ');

    cout << "|";

    for (int j = 0; j < cells_in_row; ++j) {
      cout << setw(cell_width) << matrix[i][j] << " |";
    }

    cout << "\n   " << string(total_width, '-') << "\n";
  }

  cout << endl;
}

Move<char> *Pyramid_UI::get_move(Player<char> *player) {
  int x, y;

  if (player->get_type() == PlayerType::HUMAN) {
    cout << "\nPlease enter your move x and y (0 to 2): ";
    cin >> x >> y;
  } else if (player->get_type() == PlayerType::COMPUTER) {
    x = rand() % player->get_board_ptr()->get_rows();
    y = rand() % player->get_board_ptr()->get_columns();
  }
  return new Move<char>(x, y, player->get_symbol());
}

Player<char> **Pyramid_UI::setup_players() {
  Player<char> **players = new Player<char> *[2];
  vector<string> type_options = {"Human", "Computer"};

  string nameX = get_player_name("Player X");
  PlayerType typeX = get_player_type_choice("Player X", type_options);
  players[0] = create_player(nameX, static_cast<char>('X'), typeX);

  string nameO = get_player_name("Player O");
  PlayerType typeO = get_player_type_choice("Player O", type_options);
  players[1] = create_player(nameO, static_cast<char>('O'), typeO);
  return players;
}

Pyramid::Pyramid() {}

Pyramid::~Pyramid() {
  delete board;
  delete ui;
}

void Pyramid::runGame() {

  descripe();
  Player<char> **players = ui->setup_players();
  // Create the game manager with the board and the array of players.
  GameManager<char> game(board, players, ui);
  // Run the game loop.
  game.run();

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Pyramid::getName() const { return "Pyramid Tic Tac Toe"; }

std::string Pyramid::getDesc() const {
  return "Pyramid Tic Tac Toe - a thrilling twist on the classic game!\n"
         "Challenge your friend or play against the computer on a "
         "pyramid-shaped board, "
         "where each row has more positions than the one above.\n"
         "Be the first to align three symbols in a row, column, or diagonal "
         "and claim victory at the peak of the pyramid!";
}

std::pair<int, int> Pyramid::getBoardSize() const { return {3, 5}; }

void Pyramid::descripe() const {
  std::cout << getDesc() << '\n';
  std::cout << "Board Size: ( " << getBoardSize().first << ", "
            << getBoardSize().second << " ).\n\n";
}

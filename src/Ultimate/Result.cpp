

//--------------------------------------- IMPLEMENTATION

#include "Result.h"

using namespace std;

//--------------------------------------- Result_Board Implementation

Result_Board::Result_Board() : Board(3, 3) {
  for (auto &row : board)
    for (auto &cell : row)
      cell = blank_symbol;
}

bool Result_Board::update_board(Move<char> *move) {
  int x = move->get_x();
  int y = move->get_y();
  char mark = move->get_symbol();

  // Validate move and apply if valid
  if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
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

bool Result_Board::is_win(Player<char> *player) {
  const char sym = player->get_symbol();

  auto all_equal = [&](char a, char b, char c) {
    return a == b && b == c && a != blank_symbol;
  };

  // Check rows and columns
  for (int i = 0; i < rows; ++i) {
    if ((all_equal(board[i][0], board[i][1], board[i][2]) &&
         board[i][0] == sym) ||
        (all_equal(board[0][i], board[1][i], board[2][i]) &&
         board[0][i] == sym))
      return true;
  }

  // Check diagonals
  if ((all_equal(board[0][0], board[1][1], board[2][2]) &&
       board[1][1] == sym) ||
      (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
    return true;

  return false;
}

bool Result_Board::is_draw(Player<char> *player) {
  return (n_moves == 9 && !is_win(player));
}

bool Result_Board::game_is_over(Player<char> *player) {
  return is_win(player) || is_draw(player);
}

//--------------------------------------- Result_UI Implementation

Result_UI::Result_UI() : UI<char>(3) {}

Player<char> *Result_UI::create_player(string &name, char symbol,
                                       PlayerType type) {
  // Create player based on type
  cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";

  return new Player<char>(name, symbol, type);
}

void Result_UI::display_board_matrix(const vector<vector<char>> &matrix) const {
  if (matrix.empty() || matrix[0].empty())
    return;

  int rows = matrix.size();
  int cols = matrix[0].size();

  cout << "\n    ";
  for (int j = 0; j < cols; ++j)
    cout << setw(cell_width + 1) << j;
  cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

  for (int i = 0; i < rows; ++i) {
    cout << setw(2) << i << " |";
    for (int j = 0; j < cols; ++j)
      cout << setw(cell_width) << matrix[i][j] << " |";
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
  }
  cout << endl;
}

Player<char> **Result_UI::setup_players() {
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
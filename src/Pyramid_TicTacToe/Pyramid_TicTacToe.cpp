
//--------------------------------------- IMPLEMENTATION

#include "Pyramid_TicTacToe.h"
#include <cctype>
#include <iostream>

using namespace std;

//--------------------------------------- Pyramid_Board Implementation

Pyramid_Board::Pyramid_Board() : Board(3, 5) {
  // Initialize all cells with blank_symbol

  board.resize(3);
  for (int i = 0; i < 3; i++) {
    board.resize(2 * i + 1);
    for (int j = 0; j < 2 * i + 1; j++) {
      board[i][j] = blank_symbol;
    }
  }
}

bool Pyramid_Board::update_board(Move<char> *move) {
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

bool Pyramid_Board::is_win(Player<char> *player) {
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
  int cols = matrix[0].size();

  cout << "\n    ";
  for (int j = 0; j < cols; ++j)
    cout << setw(cell_width + 1) << j;
  cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

  for (int i = 0; i < matrix.size(); ++i) {
    cout << setw(2) << i << " |";
    for (int j = 0; j < matrix[i].size(); ++j)
      cout << setw(cell_width) << matrix[i][j] << " |";
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
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

std::string Pyramid::getName() const { return "Tic Tac Toe"; }

std::string Pyramid::getDesc() const {
  return "Tic Tac Toe Battle - the classic game that tests your mind and quick "
         "thinking!\nChallenge your friend or play against the computer, and "
         "try to be the first to line up three symbols in a row.\nCan you "
         "outsmart your opponent and claim victory?";
}

std::pair<int, int> Pyramid::getBoardSize() const { return {3, 3}; }

void Pyramid::descripe() const {
  std::cout << getDesc() << '\n';
  std::cout << "Board Size: ( " << getBoardSize().first << ", "
            << getBoardSize().second << " ).\n\n";
}

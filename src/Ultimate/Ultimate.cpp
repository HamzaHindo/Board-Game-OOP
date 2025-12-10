
//--------------------------------------- IMPLEMENTATION

#include "Ultimate.h"
#include "../XO/XO.h"
#include "BoardGame_Classes.h"
#include <cctype> // for toupper()
#include <iomanip>
#include <iostream>

using namespace std;

//--------------------------------------- Ultimate_Board Implementation

Ultimate_Board::Ultimate_Board() : Board(9, 9) {
  // Initialize all cells with blank_symbol
  for (auto &row : board)
    for (auto &cell : row)
      cell = blank_symbol;
}

bool Ultimate_Board::update_board(Move<char> *move) {
  int x = move->get_x();
  int y = move->get_y();
  char mark = move->get_symbol();

  currentSubGame[0] = x / 3;
  currentSubGame[1] = y / 3;

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

bool Ultimate_Board::is_win(Player<char> *player) {
  const char sym = player->get_symbol();

  auto all_equal = [&](char a, char b, char c) {
    return a == b && b == c && a != blank_symbol;
  };

  int x_placement = currentSubGame[0] * 3;
  int y_placement = currentSubGame[1] * 3;
  // Check rows and columns
  for (int i = 0; i < 3; ++i) {

    i += x_placement;
    if ((all_equal(board[i][y_placement], board[i][y_placement + 1],
                   board[i][y_placement + 2]) &&
         board[i][0 + y_placement] == sym)) {
      getResultPtr()->update_board(
          new Move<char>(currentSubGame[0], currentSubGame[1], sym));
    }

    i -= x_placement;
    i += y_placement;
    if ((all_equal(board[x_placement][i], board[x_placement + 1][i],
                   board[x_placement + 2][i]) &&
         board[0 + x_placement][i] == sym))
      getResultPtr()->update_board(
          new Move<char>(currentSubGame[0], currentSubGame[1], sym));
    i -= y_placement;
  }

  // Check diagonals
  if ((all_equal(board[0 + x_placement][0 + y_placement],
                 board[1 + x_placement][1 + y_placement],
                 board[2 + x_placement][2 + y_placement]) &&
       board[1 + x_placement][1 + y_placement] == sym) ||
      (all_equal(board[0 + x_placement][2 + y_placement],
                 board[1 + x_placement][1 + y_placement],
                 board[2 + x_placement][0 + y_placement]) &&
       board[1 + x_placement][1 + y_placement] == sym))
    getResultPtr()->update_board(
        new Move<char>(currentSubGame[0], currentSubGame[1], sym));

  return false;
}

bool Ultimate_Board::is_draw(Player<char> *player) { return false; }

bool Ultimate_Board::game_is_over(Player<char> *player) { return false; }

//--------------------------------------- Ultimate_UI Implementation

Ultimate_UI::Ultimate_UI() : UI<char>(3) {}

Player<char> *Ultimate_UI::create_player(string &name, char symbol,
                                         PlayerType type) {
  // Create player based on type
  cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";

  return new Player<char>(name, symbol, type);
}

void Ultimate_UI::display_board_matrix(
    const vector<vector<char>> &matrix) const {

  if (matrix.empty() || matrix[0].empty())
    return;

  int rows = matrix.size();
  int cols = matrix[0].size();

  // Print the second matrix
  cout << "\n    ";
  for (int j = 0; j < cols; ++j) {
    cout << setw(cell_width + 1) << j;
    if ((j + 1) % 3 == 0 && j != 8)
      cout << "    ";
  }
  cout << "\n   " << string((cell_width + 2) * cols + 4, '-') << "\n";

  for (int i = 0; i < rows; ++i) {
    cout << setw(2) << i << " |";
    for (int j = 0; j < cols; ++j) {
      if ((j + 1) % 3 == 0 && j != 8) {
        cout << setw(cell_width) << matrix[i][j] << " ||";
      } else {
        cout << setw(cell_width) << matrix[i][j] << " |";
      }
    }
    cout << "\n   " << string((cell_width + 2) * cols + 4, '-') << "\n";
    if ((i + 1) % 3 == 0 && i != 8) {
      cout << "   " << string((cell_width + 2) * cols + 4, '-') << "\n";
    }
  }
  cout << endl;
}

Move<char> *Ultimate_UI::get_move(Player<char> *player) {
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

Player<char> **Ultimate_UI::setup_players() {
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

Ultimate::Ultimate() {}

Ultimate::~Ultimate() {
  delete board;
  delete ui;
  delete resultBoardptr;
  delete resultUI;
}

void Ultimate::runGame() {

  descripe();
  Player<char> **players = ui->setup_players();
  // Create the game manager with the board and the array of players.
  Ultimate_Game_Manager game(board, resultBoardptr, players, ui, resultUI);
  // Run the game loop.

  game.run();

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Ultimate::getName() const { return "Tic Tac Toe"; }

std::string Ultimate::getDesc() const {
  return "Tic Tac Toe Battle - the classic game that tests your mind and quick "
         "thinking!\nChallenge your friend or play against the computer, and "
         "try to be the first to line up three symbols in a row.\nCan you "
         "outsmart your opponent and claim victory?";
}

std::pair<int, int> Ultimate::getBoardSize() const { return {3, 3}; }

void Ultimate::descripe() const {
  std::cout << getDesc() << '\n';
  std::cout << "Board Size: ( " << getBoardSize().first << ", "
            << getBoardSize().second << " ).\n\n";
}

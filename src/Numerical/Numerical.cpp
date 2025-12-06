#include "Numerical.h"
#include <iostream>

using namespace std;

Numerical_Board::Numerical_Board() : Board<int>(3, 3) {
  for (auto &row : board)
    for (auto &cell : row)
      cell = blank_symbol;
}

bool Numerical_Board::update_board(Move<int> *move) {
  int x = move->get_x();
  int y = move->get_y();
  int number = move->get_symbol();

  if (x < 0 || x >= rows || y < 0 || y >= columns)
    return false;

  if (board[x][y] != blank_symbol)
    return false;

  if (number != 0 && is_number_used(number))
    return false;

  if (number == 0) {
    if (board[x][y] != blank_symbol) {
      unmark_number_used(board[x][y]);
      n_moves--;
      board[x][y] = blank_symbol;
    }
  } else {
    mark_number_used(number);
    n_moves++;
    board[x][y] = number;
  }
  return true;
}

bool Numerical_Board::is_win(Player<int> *player) {
  for (int i = 0; i < rows; ++i) {
    if (board[i][0] != blank_symbol && board[i][1] != blank_symbol &&
        board[i][2] != blank_symbol) {
      if (board[i][0] + board[i][1] + board[i][2] == WIN_SUM)
        return true;
    }
  }

  for (int j = 0; j < columns; ++j) {
    if (board[0][j] != blank_symbol && board[1][j] != blank_symbol &&
        board[2][j] != blank_symbol) {
      if (board[0][j] + board[1][j] + board[2][j] == WIN_SUM)
        return true;
    }
  }

  if (board[0][0] != blank_symbol && board[1][1] != blank_symbol &&
      board[2][2] != blank_symbol) {
    if (board[0][0] + board[1][1] + board[2][2] == WIN_SUM)
      return true;
  }

  if (board[0][2] != blank_symbol && board[1][1] != blank_symbol &&
      board[2][0] != blank_symbol) {
    if (board[0][2] + board[1][1] + board[2][0] == WIN_SUM)
      return true;
  }

  return false;
}

bool Numerical_Board::is_draw(Player<int> *player) {
  return (n_moves == 9 && !is_win(player));
}

bool Numerical_Board::game_is_over(Player<int> *player) {
  return is_win(player) || is_draw(player);
}

bool Numerical_Board::is_number_used(int number) const {
  return used_numbers.find(number) != used_numbers.end();
}

void Numerical_Board::mark_number_used(int number) {
  used_numbers.insert(number);
}

void Numerical_Board::unmark_number_used(int number) {
  used_numbers.erase(number);
}

vector<int> Numerical_Board::get_available_numbers(bool is_player1) const {
  vector<int> available;
  if (is_player1) {
    for (int num : {1, 3, 5, 7, 9}) {
      if (!is_number_used(num))
        available.push_back(num);
    }
  } else {
    for (int num : {2, 4, 6, 8}) {
      if (!is_number_used(num))
        available.push_back(num);
    }
  }
  return available;
}

Numerical_UI::Numerical_UI() : UI<int>(4) {}

Player<int> *Numerical_UI::create_player(string &name, int symbol,
                                         PlayerType type) {
  string player_type_str = (symbol == 1) ? "Odd Numbers" : "Even Numbers";
  string actual_type = (type == PlayerType::HUMAN) ? "human" : "computer";

  cout << "Creating " << actual_type << " player: " << name << " ("
       << player_type_str << ")\n";

  return new Player<int>(name, symbol, type);
}

bool Numerical_UI::is_valid_number(int number, bool is_player1) const {
  if (is_player1) {
    return (number >= 1 && number <= 9 && number % 2 == 1);
  } else {
    return (number >= 2 && number <= 8 && number % 2 == 0);
  }
}

Player<int> **Numerical_UI::setup_players() { return new Player<int> *[2]; }

Move<int> *Numerical_UI::get_move(Player<int> *player) {
  bool is_player1 = (player->get_symbol() == 1);
  Numerical_Board *num_board =
      static_cast<Numerical_Board *>(player->get_board_ptr());
  vector<int> available = num_board->get_available_numbers(is_player1);

  if (player->get_type() == PlayerType::HUMAN) {
    cout << "\n"
         << player->get_name() << "'s Turn ("
         << (is_player1 ? "Odd Numbers" : "Even Numbers") << ")\n";
    cout << "Available numbers: ";
    for (int num : available)
      cout << num << " ";
    cout << "\n";

    int x, y, number;
    while (true) {
      cout << "Enter position (row col) and number (0 to 2): ";
      cin >> x >> y >> number;

      if (x < 0 || x > 2 || y < 0 || y > 2) {
        cout << "Invalid position! Please enter values between 0 and 2.\n";
        continue;
      }

      if (!is_valid_number(number, is_player1)) {
        cout << "Invalid number! ";
        cout << (is_player1 ? "Use odd numbers (1,3,5,7,9)"
                            : "Use even numbers (2,4,6,8)")
             << "\n";
        continue;
      }

      if (num_board->is_number_used(number)) {
        cout << "Number " << number << " has already been used!\n";
        continue;
      }

      break;
    }
    return new Move<int>(x, y, number);
  } else if (player->get_type() == PlayerType::COMPUTER) {
    if (available.empty()) {
      cout << "No available numbers for computer player!\n";
      return new Move<int>(-1, -1, 0);
    }

    int random_number = available[rand() % available.size()];
    int x, y;

    do {
      x = rand() % 3;
      y = rand() % 3;
    } while (player->get_board_ptr()->get_board_matrix()[x][y] != 0);

    cout << "\nComputer (" << player->get_name() << ") places " << random_number
         << " at (" << x << ", " << y << ")\n";

    return new Move<int>(x, y, random_number);
  }

  return new Move<int>(-1, -1, 0);
}

Numerical::Numerical() {
  ui = new Numerical_UI();
  board = new Numerical_Board();
}

Numerical::~Numerical() {
  delete board;
  delete ui;
}

void Numerical::runGame() {

  descripe();

  string nameP1;
  cout << "Enter Player 1 (Odd Numbers) name: ";
  getline(cin >> ws, nameP1);

  string nameP2;
  cout << "Enter Player 2 (Even Numbers) name: ";
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

  Player<int> **players = new Player<int> *[2];
  players[0] = ui->create_player(nameP1, 1, type1);
  players[1] = ui->create_player(nameP2, 2, type2);

  GameManager<int> game(board, players, ui);
  game.run();

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Numerical::getName() const { return "Numerical Tic-Tac-Toe"; }

std::string Numerical::getDesc() const {
  return "Numerical Tic-Tac-Toe - A mathematical challenge!\n"
         "Player 1 uses odd numbers (1, 3, 5, 7, 9)\n"
         "Player 2 uses even numbers (2, 4, 6, 8)\n"
         "Form a line (row, column, or diagonal) that sums to 15 to win!\n"
         "Each number can only be used once. Strategy and math skills "
         "required!";
}

std::pair<int, int> Numerical::getBoardSize() const { return {3, 3}; }

void Numerical::descripe() const {
  std::cout << "\n========================================\n";
  std::cout << getDesc() << "\n";
  std::cout << "Board Size: (" << getBoardSize().first << ", "
            << getBoardSize().second << ")\n";
  std::cout << "========================================\n\n";
}

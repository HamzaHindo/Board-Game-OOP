#include "Obstacles.h"
#include <cstdlib>
#include <iostream>

using namespace std;

bool WinChecker::check_line(int row, int col, int dr, int dc, char mark) const {
  for (int i = 0; i < WIN_LENGTH; ++i) {
    int r = row + i * dr;
    int c = col + i * dc;

    if (r < 0 || r >= rows || c < 0 || c >= columns)
      return false;

    if ((*board)[r][c] != mark)
      return false;
  }
  return true;
}

bool WinChecker::has_won(int row, int col, char mark) const {
  int directions[][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

  for (auto &dir : directions) {
    if (check_line(row, col, dir[0], dir[1], mark))
      return true;
  }
  return false;
}

bool ObstacleManager::is_valid_position(int row, int col) const {
  return row >= 0 && row < rows && col >= 0 && col < columns;
}

bool ObstacleManager::add_single_obstacle() {
  int row, col;
  int attempts = 0;

  while (attempts < 50) {
    row = rand() % rows;
    col = rand() % columns;

    if (!is_obstacle(row, col) && (*board)[row][col] == '.' &&
        is_valid_position(row, col)) {
      place_obstacle(row, col);
      return true;
    }
    attempts++;
  }
  return false;
}

void ObstacleManager::initialize() {
  for (int i = 0; i < INITIAL_OBSTACLES; ++i)
    add_single_obstacle();
}

void ObstacleManager::add_obstacles_after_round() {
  for (int i = 0; i < OBSTACLES_PER_ROUND; ++i)
    add_single_obstacle();
}

bool ObstacleManager::is_obstacle(int row, int col) const {
  return obstacles.find({row, col}) != obstacles.end();
}

void ObstacleManager::place_obstacle(int row, int col) {
  if (is_valid_position(row, col)) {
    obstacles.insert({row, col});
    (*board)[row][col] = '#';
    obstacle_count++;
  }
}

Obstacles_Board::Obstacles_Board()
    : Board<char>(6, 6), win_checker(&board, 6, 6), obstacle_mgr(&board, 6, 6) {

  for (auto &row : board)
    for (auto &cell : row)
      cell = '.';

  obstacle_mgr.initialize();
}

bool Obstacles_Board::update_board(Move<char> *move) {
  int row = move->get_x();
  int col = move->get_y();
  char mark = move->get_symbol();

  if (row < 0 || row >= rows || col < 0 || col >= columns)
    return false;

  if (board[row][col] != '.')
    return false;

  if (obstacle_mgr.is_obstacle(row, col))
    return false;

  board[row][col] = toupper(mark);
  n_moves++;
  return true;
}

bool Obstacles_Board::is_win(Player<char> *player) {
  char sym = player->get_symbol();

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (board[i][j] == sym && win_checker.has_won(i, j, sym))
        return true;
    }
  }
  return false;
}

bool Obstacles_Board::is_draw(Player<char> *player) {
  if (is_win(player))
    return false;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (board[i][j] == '.' && !obstacle_mgr.is_obstacle(i, j))
        return false;
    }
  }
  return true;
}

bool Obstacles_Board::game_is_over(Player<char> *player) {
  return is_win(player) || is_draw(player);
}

Obstacles_UI::Obstacles_UI() : UI<char>(2) {}

Player<char> *Obstacles_UI::create_player(string &name, char symbol,
                                          PlayerType type) {
  string type_str = (type == PlayerType::HUMAN) ? "human" : "computer";
  cout << "Creating " << type_str << " player: " << name << " (" << symbol
       << ")\n";
  return new Player<char>(name, symbol, type);
}

bool Obstacles_UI::get_valid_input(int &row, int &col,
                                   const Obstacles_Board *board) {
  if (row < 0 || row >= 6 || col < 0 || col >= 6) {
    cout << "Invalid position! Enter values between 0-5.\n";
    return false;
  }

  if (board->get_obstacle_manager().is_obstacle(row, col)) {
    cout << "Obstacle at (" << row << ", " << col << ")!\n";
    return false;
  }

  if (board->get_board_matrix()[row][col] != '.') {
    cout << "Position already occupied!\n";
    return false;
  }

  return true;
}

void Obstacles_UI::display_board_matrix(
    const vector<vector<char>> &matrix) const {
  if (matrix.empty())
    return;

  int rows = matrix.size();
  int cols = matrix[0].size();

  cout << "\n  ";
  for (int j = 0; j < cols; ++j)
    cout << j << " ";
  cout << "\n  ";
  for (int j = 0; j < cols; ++j)
    cout << "--";
  cout << "\n";

  for (int i = 0; i < rows; ++i) {
    cout << i << "|";
    for (int j = 0; j < cols; ++j)
      cout << matrix[i][j] << " ";
    cout << "\n";
  }

  cout << "  ";
  for (int j = 0; j < cols; ++j)
    cout << "--";
  cout << "\n\n";
}

Move<char> *Obstacles_UI::get_move(Player<char> *player) {
  Obstacles_Board *obs_board =
      static_cast<Obstacles_Board *>(player->get_board_ptr());

  if (player->get_type() == PlayerType::HUMAN) {
    cout << "\n"
         << player->get_name() << "'s Turn (" << player->get_symbol() << ")\n";

    int row, col;
    while (true) {
      cout << "Enter position (row col) [0-5]: ";
      cin >> row >> col;
      if (get_valid_input(row, col, obs_board))
        break;
    }
    return new Move<char>(row, col, player->get_symbol());
  } else {
    int row, col;
    do {
      row = rand() % 6;
      col = rand() % 6;
    } while (obs_board->get_obstacle_manager().is_obstacle(row, col) ||
             obs_board->get_board_matrix()[row][col] != '.');

    cout << "\nComputer places " << player->get_symbol() << " at (" << row
         << ", " << col << ")\n";
    return new Move<char>(row, col, player->get_symbol());
  }
}

Obstacles::Obstacles() {
  ui = new Obstacles_UI();
  board = new Obstacles_Board();
}

Obstacles::~Obstacles() {
  delete board;
  delete ui;
}

void Obstacles::play_turn(Player<char> *player) {
  Move<char> *move = ui->get_move(player);
  while (!board->update_board(move)) {
    delete move;
    move = ui->get_move(player);
  }
  delete move;
}

void Obstacles::add_obstacles_if_needed() {
  turns_played++;
  if (turns_played % 2 == 0) {
    cout << "\n*** Adding 2 new obstacles! ***\n";
    board->get_obstacle_manager().add_obstacles_after_round();
    ui->display_board_matrix(board->get_board_matrix());
  }
}

bool Obstacles::check_game_end(Player<char> *current_player, bool &game_over) {
  if (board->is_win(current_player)) {
    ui->display_message(current_player->get_name() + " wins!");
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

void Obstacles::runGame() {
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

  Player<char> **players = new Player<char> *[2];
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

      add_obstacles_if_needed();
    }
  }

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Obstacles::getName() const { return "Obstacles Tic-Tac-Toe"; }

std::string Obstacles::getDesc() const {
  return "Obstacles Tic-Tac-Toe - Dynamic Strategy!\n"
         "6x6 board with random obstacles (#)\n"
         "2 new obstacles appear after each round\n"
         "First to get 4 in a row wins!";
}

std::pair<int, int> Obstacles::getBoardSize() const { return {6, 6}; }

void Obstacles::descripe() const {
  cout << "\n========================================\n"
       << getDesc() << "\n"
       << "Board Size: (" << getBoardSize().first << ", "
       << getBoardSize().second << ")\n"
       << "========================================\n\n";
}

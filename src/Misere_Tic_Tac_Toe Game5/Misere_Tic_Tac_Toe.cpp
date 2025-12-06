#include "Misere_Tic_Tac_Toe.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Misere_Board::Misere_Board() : Board<char>(3, 3) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      board[i][j] = ' ';
    }
  }
}

bool Misere_Board::update_board(Move<char> *move) {
  int x = move->get_x();
  int y = move->get_y();

  if (x < 0 || x >= rows || y < 0 || y >= columns) {
    return false;
  }

  if (board[x][y] != ' ') {
    return false;
  }

  board[x][y] = move->get_symbol();
  n_moves++;
  return true;
}

void Misere_Board::undo_move(int x, int y) {
  board[x][y] = ' ';
  n_moves--;
}

bool Misere_Board::is_win(Player<char> *player) { return false; }

bool Misere_Board::is_lose(Player<char> *player) {
  char symbol = player->get_symbol();

  for (int i = 0; i < rows; i++) {
    if (board[i][0] == symbol && board[i][1] == symbol &&
        board[i][2] == symbol) {
      return true;
    }
  }

  for (int j = 0; j < columns; j++) {
    if (board[0][j] == symbol && board[1][j] == symbol &&
        board[2][j] == symbol) {
      return true;
    }
  }

  if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
    return true;
  }
  if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
    return true;
  }

  return false;
}

bool Misere_Board::is_draw(Player<char> *player) {
  if (n_moves < rows * columns) {
    return false;
  }
  return true;
}

bool Misere_Board::game_is_over(Player<char> *player) {
  return is_lose(player) || is_draw(player);
}

bool Misere_Board::has_three_in_row(char symbol) {
  for (int i = 0; i < rows; i++) {
    if (board[i][0] == symbol && board[i][1] == symbol &&
        board[i][2] == symbol) {
      return true;
    }
  }

  for (int j = 0; j < columns; j++) {
    if (board[0][j] == symbol && board[1][j] == symbol &&
        board[2][j] == symbol) {
      return true;
    }
  }

  if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
    return true;
  }
  if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
    return true;
  }

  return false;
}

Misere_AI_Player::Misere_AI_Player(char symbol)
    : Player<char>("Computer", symbol, PlayerType::COMPUTER) {}

int Misere_AI_Player::evaluate(Misere_Board *board, int depth) {
  char ai_symbol = this->symbol;
  char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

  if (board->has_three_in_row(ai_symbol)) {
    return -10 + depth;
  }

  if (board->has_three_in_row(opponent_symbol)) {
    return 10 - depth;
  }

  return 0;
}

int Misere_AI_Player::minimax(Misere_Board *board, int depth,
                              bool is_maximizing, int alpha, int beta) {
  char ai_symbol = this->symbol;
  char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

  int score = evaluate(board, depth);
  if (score == 10 - depth || score == -10 + depth) {
    return score;
  }

  bool is_full = true;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board->get_cell(i, j) == ' ') {
        is_full = false;
        break;
      }
    }
    if (!is_full)
      break;
  }
  if (is_full)
    return 0;

  if (depth > 8)
    return 0;

  if (is_maximizing) {
    int best_score = numeric_limits<int>::min();

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board->get_cell(i, j) == ' ') {
          Move<char> move(i, j, ai_symbol);
          board->update_board(&move);

          int current_score = minimax(board, depth + 1, false, alpha, beta);
          best_score = max(best_score, current_score);

          board->undo_move(i, j);

          alpha = max(alpha, best_score);
          if (beta <= alpha) {
            return best_score;
          }
        }
      }
    }
    return best_score;
  } else {
    int best_score = numeric_limits<int>::max();

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board->get_cell(i, j) == ' ') {
          Move<char> move(i, j, opponent_symbol);
          board->update_board(&move);

          int current_score = minimax(board, depth + 1, true, alpha, beta);
          best_score = min(best_score, current_score);

          board->undo_move(i, j);

          beta = min(beta, best_score);
          if (beta <= alpha) {
            return best_score;
          }
        }
      }
    }
    return best_score;
  }
}

Move<char> *Misere_AI_Player::get_best_move() {
  Misere_Board *board = dynamic_cast<Misere_Board *>(this->boardPtr);
  if (!board)
    return nullptr;

  int best_score = numeric_limits<int>::min();
  int best_x = -1, best_y = -1;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board->get_cell(i, j) == ' ') {
        Move<char> move(i, j, this->symbol);
        board->update_board(&move);

        int score = minimax(board, 0, false, numeric_limits<int>::min(),
                            numeric_limits<int>::max());

        board->undo_move(i, j);

        if (score > best_score) {
          best_score = score;
          best_x = i;
          best_y = j;
        }
      }
    }
  }

  cout << "Computer plays: " << best_x << " " << best_y << endl;

  return new Move<char>(best_x, best_y, this->symbol);
}

Misere_UI::Misere_UI()
    : UI<char>("Welcome to Misere Tic-Tac-Toe!\nGOAL: Avoid getting three in a "
               "row!\nThe player who gets three in a row LOSES!\n",
               3) {}

Move<char> *Misere_UI::get_move(Player<char> *player) {
  int x, y;

  if (player->get_type() == PlayerType::HUMAN) {
    cout << player->get_name() << " (" << player->get_symbol()
         << "), enter your move (row column): ";
    cin >> x >> y;
  } else {
    Misere_AI_Player *ai_player = dynamic_cast<Misere_AI_Player *>(player);
    if (ai_player) {
      return ai_player->get_best_move();
    }
  }

  return new Move<char>(x, y, player->get_symbol());
}

Player<char> **Misere_UI::setup_players() {
  Player<char> **players = new Player<char> *[2];
  vector<string> type_options = {"Human", "Computer"};

  string nameX = get_player_name("Player X");
  PlayerType typeX = get_player_type_choice("Player X", type_options);
  players[0] = create_player(nameX, 'X', typeX);

  string nameO = get_player_name("Player O");
  PlayerType typeO = get_player_type_choice("Player O", type_options);
  players[1] = create_player(nameO, 'O', typeO);

  return players;
}

Player<char> *Misere_UI::create_player(string &name, char symbol,
                                       PlayerType type) {
  if (type == PlayerType::COMPUTER) {
    return new Misere_AI_Player(symbol);
  } else {
    return new Player<char>(name, symbol, PlayerType::HUMAN);
  }
}

// ===================== Misere =====================

Misere::Misere() {
  ui = new Misere_UI();
  board = new Misere_Board();
}

Misere::~Misere() {
  delete board;
  delete ui;
}

void Misere::runGame() {

  descripe();
  Player<char> **players = ui->setup_players();
  GameManager<char> game(board, players, ui);
  game.run();

  for (int i = 0; i < 2; ++i) {
    delete players[i];
  }
  delete[] players;
}

std::string Misere::getName() const { return "MISERE TIC-TAC-TOE"; }
std::string Misere::getDesc() const {
  return "Rules: The player who gets 3 in a row LOSES!"
         "Try to force your opponent to make 3 in a row.";
}
std::pair<int, int> Misere::getBoardSize() const { return {3, 3}; }

void Misere::descripe() const {
  std::cout << getDesc() << '\n';
  std::cout << "Board Size: ( " << getBoardSize().first << ", "
            << getBoardSize().second << " ).\n\n";
}

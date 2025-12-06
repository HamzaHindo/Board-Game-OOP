#include "word_tic_tac_toe.h"
#include <cctype>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

Word_TTT_Board::Word_TTT_Board() : Board(3, 3) {
  // Initialize all cells with blank_symbol
  for (auto &row : board)
    for (auto &cell : row)
      cell = blank_symbol;
}

bool Word_TTT_Board::update_board(Move<char> *move) {
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

bool Word_TTT_Board::is_win(Player<char> *player) {
  const char sym = player->get_symbol();

  string filename = "dic.txt";
  std::ifstream dic(filename);

  if (!dic.is_open()) {
    cout << "Could not open file: " << filename << std::endl;
    return false;
  }

  auto getWord = [](char a, char b, char c) -> string {
    return string({static_cast<char>(toupper(a)), static_cast<char>(toupper(b)),
                   static_cast<char>(toupper(c))});
  };

  string line;
  while (getline(dic, line)) {
    for (int i = 0; i < rows; ++i) {
      if (getWord(board[i][0], board[i][1], board[i][2]) == line ||
          getWord(board[0][i], board[1][i], board[2][i]) == line)
        return true;
    }

    if (getWord(board[0][0], board[1][1], board[2][2]) == line ||
        getWord(board[0][2], board[1][1], board[2][0]) == line)
      return true;
  }
  return false;
}
bool Word_TTT_Board::is_draw(Player<char> *player) {
  return (n_moves == 9 && !is_win(player));
}

bool Word_TTT_Board::game_is_over(Player<char> *player) {
  return is_win(player) || is_draw(player);
}

//--------------------------------------- Word_TTT_UI Implementation

Word_TTT_UI::Word_TTT_UI() : UI<char>(3) {}

Player<char> **Word_TTT_UI::setup_players() {
  Player<char> **players = new Player<char> *[2];
  vector<string> type_options = {"Human", "Computer"};

  string names[2] = {"1st player", "2nd player"};
  PlayerType types[2];
  for (int i : {0, 1}) {
    names[i] = get_player_name(names[i]);
    types[i] = get_player_type_choice(names[i], type_options);
    players[i] = create_player(names[i], static_cast<char>('a'), types[i]);
  }

  return players;
}

Player<char> *Word_TTT_UI::create_player(string &name, char symbol,
                                         PlayerType type) {
  // Create player based on type
  cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
       << " player: " << name << " (" << symbol << ")\n";

  return new Player<char>(name, symbol, type);
}

Move<char> *Word_TTT_UI::get_move(Player<char> *player) {
  int x, y;
  char c;

  if (player->get_type() == PlayerType::HUMAN) {

    while (true) {
      try {
        cout
            << "\nPlease enter your move x, y (0 to 2) and a letter (a to z): ";
        cin >> x >> y;

        string buffer;
        cin >> buffer;

        if (buffer.size() != 1)
          throw runtime_error("Input must be exactly one character.");

        c = buffer[0];

        if (!isalpha(static_cast<unsigned char>(c)))
          throw runtime_error("Character must be or a-z.");

        break;
      } catch (const std::exception &e) {
        cout << "Invalid input: " << e.what() << ", try again.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

  } else if (player->get_type() == PlayerType::COMPUTER) {

    x = rand() % player->get_board_ptr()->get_rows();
    y = rand() % player->get_board_ptr()->get_columns();
  }
  return new Move<char>(x, y, c);
}

Word_TTT::Word_TTT() {}

Word_TTT::~Word_TTT() {
  delete board;
  delete ui;
}

void Word_TTT::runGame() {

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

std::string Word_TTT::getName() const { return "Word Tic Tac Toe"; }

std::string Word_TTT::getDesc() const {
  return "A fresh twist on the classic Tic-Tac-Toe! Instead of Xs and Os, "
         "players compete by forming words on the grid. Each turn, a player "
         "chooses a word from a predefined dictionary (dic.txt), like ADD or "
         "ADH, and places it strategically. The goal is to align words in a "
         "row, column, or diagonal — similar to traditional Tic-Tac-Toe — but "
         "with an added challenge of vocabulary and strategy.";
}

std::pair<int, int> Word_TTT::getBoardSize() const { return {3, 3}; }

void Word_TTT::descripe() const {
  std::cout << getDesc() << '\n';
  std::cout << "Board Size: ( " << getBoardSize().first << ", "
            << getBoardSize().second << " ).\n\n";
}

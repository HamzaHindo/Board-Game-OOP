#include "InfinityTicTacToe.h"
#include <iostream>
#include <iomanip>

using namespace std;



InfinityTTT_Board::InfinityTTT_Board() : Board<char>(3, 3) {
  
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;

    player_moves['X'] = queue<TimestampedMove>();
    player_moves['O'] = queue<TimestampedMove>();
}

void InfinityTTT_Board::remove_oldest_move(char player_symbol) {
    if (player_moves[player_symbol].empty())
        return;

    TimestampedMove oldest = player_moves[player_symbol].front();
    player_moves[player_symbol].pop();

    board[oldest.x][oldest.y] = blank_symbol;

    cout << "\n[Auto-Removal] " << oldest.symbol << "'s mark at (" << oldest.x
        << ", " << oldest.y << ") has been removed!\n";
}

size_t InfinityTTT_Board::get_player_move_count(char symbol) const {
    auto it = player_moves.find(symbol);
    if (it != player_moves.end()) {
        return it->second.size();
    }
    return 0;
}

bool InfinityTTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    char mark_upper = toupper(mark);

  
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

    
    if (board[x][y] != blank_symbol) {
        return false;
    }

   
    board[x][y] = mark_upper;
    total_moves++;

   
    player_moves[mark_upper].push(TimestampedMove(x, y, mark_upper, total_moves));

    
    if (player_moves[mark_upper].size() > 3) {
        remove_oldest_move(mark_upper);
    }

    n_moves++;
    return true;
}

bool InfinityTTT_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol && a == sym;
        };

    
    for (int i = 0; i < rows; ++i) {
        if (all_equal(board[i][0], board[i][1], board[i][2]))
            return true;
    }

   
    for (int i = 0; i < columns; ++i) {
        if (all_equal(board[0][i], board[1][i], board[2][i]))
            return true;
    }

   
    if (all_equal(board[0][0], board[1][1], board[2][2]))
        return true;

    if (all_equal(board[0][2], board[1][1], board[2][0]))
        return true;

    return false;
}

bool InfinityTTT_Board::is_draw(Player<char>* player) {

    return false;
}

bool InfinityTTT_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}



InfinityTTT_UI::InfinityTTT_UI() : UI<char>(3) {}

Player<char>* InfinityTTT_UI::create_player(string& name, char symbol,
    PlayerType type) {
    string actual_type = (type == PlayerType::HUMAN) ? "human" : "computer";

    cout << "Creating " << actual_type << " player: " << name << " ("
        << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

bool InfinityTTT_UI::is_valid_position(int x, int y) const {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

Move<char>* InfinityTTT_UI::get_move(Player<char>* player) {
    InfinityTTT_Board* game_board =
        static_cast<InfinityTTT_Board*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << "'s Turn ("
            << player->get_symbol() << ")\n";
        cout << "Moves made: " << game_board->get_player_move_count(player->get_symbol()) << "/3\n";

        int x, y;
        while (true) {
            cout << "Enter position (x y, 0-2): ";
            cin >> x >> y;

            if (!is_valid_position(x, y)) {
                cout << "Invalid position! Please enter values between 0 and 2.\n";
                continue;
            }

            
            if (game_board->get_board_matrix()[x][y] != '.') {
                cout << "Cell (" << x << ", " << y << ") is occupied!\n";
                continue;
            }

            break;
        }
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        
        char sym = player->get_symbol();
        char opponent_sym = (sym == 'X') ? 'O' : 'X';
        const auto& matrix = game_board->get_board_matrix();

        cout << "\n" << player->get_name() << "'s Turn (" << sym << ")\n";
        cout << "Moves made: " << game_board->get_player_move_count(sym) << "/3\n";

        
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (matrix[i][j] == '.') {
                   
                    auto test_board = matrix;
                    test_board[i][j] = sym;

                    
                    bool wins = false;

                   
                    if (test_board[i][0] == sym && test_board[i][1] == sym && test_board[i][2] == sym)
                        wins = true;
                    
                    if (test_board[0][j] == sym && test_board[1][j] == sym && test_board[2][j] == sym)
                        wins = true;
                   
                    if (test_board[0][0] == sym && test_board[1][1] == sym && test_board[2][2] == sym)
                        wins = true;
                    if (test_board[0][2] == sym && test_board[1][1] == sym && test_board[2][0] == sym)
                        wins = true;

                    if (wins) {
                        cout << "Computer places " << sym << " at (" << i
                            << ", " << j << ")\n";
                        return new Move<char>(i, j, sym);
                    }
                }
            }
        }

        
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (matrix[i][j] == '.') {
                    
                    auto test_board = matrix;
                    test_board[i][j] = opponent_sym;

                    
                    bool opponent_wins = false;

                    if (test_board[i][0] == opponent_sym && test_board[i][1] == opponent_sym && test_board[i][2] == opponent_sym)
                        opponent_wins = true;
                    
                    if (test_board[0][j] == opponent_sym && test_board[1][j] == opponent_sym && test_board[2][j] == opponent_sym)
                        opponent_wins = true;
                    
                    if (test_board[0][0] == opponent_sym && test_board[1][1] == opponent_sym && test_board[2][2] == opponent_sym)
                        opponent_wins = true;
                    if (test_board[0][2] == opponent_sym && test_board[1][1] == opponent_sym && test_board[2][0] == opponent_sym)
                        opponent_wins = true;

                    if (opponent_wins) {
                        cout << "Computer blocks at (" << i << ", " << j << ")\n";
                        return new Move<char>(i, j, sym);
                    }
                }
            }
        }

       
        int x, y;
        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (matrix[x][y] != '.');

        cout << "Computer plays at (" << x << ", " << y << ")\n";
        return new Move<char>(x, y, sym);
    }

    return new Move<char>(0, 0, player->get_symbol());
}



InfinityTicTacToe::InfinityTicTacToe() {
    ui = new InfinityTTT_UI();
    board = new InfinityTTT_Board();
}

InfinityTicTacToe::~InfinityTicTacToe() {
    delete board;
    delete ui;
}

void InfinityTicTacToe::runGame() {
    descripe();

    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
}

std::string InfinityTicTacToe::getName() const {
    return "Infinity Tic-Tac-Toe";
}

std::string InfinityTicTacToe::getDesc() const {
    return "Infinity Tic-Tac-Toe - The Classic Game with a Twist!\n"
        "Get three marks in a row to win, but beware - after each player makes 3 moves,\n"
        "their oldest mark on the board will vanish!\n"
        "Strategy and timing are key to victory!";
}

std::pair<int, int> InfinityTicTacToe::getBoardSize() const {
    return { 3, 3 };
}

void InfinityTicTacToe::descripe() const {
    std::cout << "\n========================================\n";
    std::cout << getDesc() << "\n";
    std::cout << "Board Size: (" << getBoardSize().first << ", "
        << getBoardSize().second << ")\n";
    std::cout << "========================================\n\n";
}
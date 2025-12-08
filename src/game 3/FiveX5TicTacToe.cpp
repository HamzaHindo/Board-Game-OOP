#include "FiveX5TicTacToe.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

FiveX5_Board::FiveX5_Board() : Board<char>(5, 5) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool FiveX5_Board::creates_sequence(int x, int y, char symbol, int dr, int dc) {
    int count = 1;
    for (int i = 1; i < 3; ++i) {
        int nx = x + i * dr;
        int ny = y + i * dc;
        if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) break;
        if (board[nx][ny] == symbol) count++;
        else break;
    }

    for (int i = 1; i < 3; ++i) {
        int nx = x - i * dr;
        int ny = y - i * dc;
        if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) break;
        if (board[nx][ny] == symbol) count++;
        else break;
    }

    return count >= 3;
}

int FiveX5_Board::count_sequences(char player_symbol) {
    int count = 0;
    int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (board[i][j] != player_symbol) continue;

            for (auto& dir : directions) {
                int line_length = 1;

                for (int k = 1; k < 3; ++k) {
                    int ni = i + k * dir[0];
                    int nj = j + k * dir[1];
                    if (ni < 0 || ni >= 5 || nj < 0 || nj >= 5) break;
                    if (board[ni][nj] == player_symbol) line_length++;
                    else break;
                }

                if (line_length == 3) {
                    int before = 0;
                    int ni = i - dir[0];
                    int nj = j - dir[1];
                    if (ni >= 0 && ni < 5 && nj >= 0 && nj < 5 && board[ni][nj] == player_symbol) {
                        before = 1;
                    }

                    if (before == 0) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

bool FiveX5_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

    if (board[x][y] != blank_symbol) {
        return false;
    }

    board[x][y] = toupper(mark);
    n_moves++;

    update_sequences();

    return true;
}

void FiveX5_Board::update_sequences() {
    x_sequences = count_sequences('X');
    o_sequences = count_sequences('O');
}

bool FiveX5_Board::is_win(Player<char>* player) {
    return false;
}

bool FiveX5_Board::is_draw(Player<char>* player) {
    return get_empty_cells() == 1;
}

bool FiveX5_Board::game_is_over(Player<char>* player) {
    return is_draw(player);
}

int FiveX5_Board::get_empty_cells() const {
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == blank_symbol) count++;
        }
    }
    return count;
}

int FiveX5_AI::count_in_direction(const vector<vector<char>>& board,
    int x, int y, char symbol, int dr, int dc) {
    int count = 0;
    for (int i = 1; i < 3; ++i) {
        int nx = x + i * dr;
        int ny = y + i * dc;
        if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) break;
        if (board[nx][ny] == symbol) count++;
        else break;
    }
    return count;
}

int FiveX5_AI::evaluate_move(const vector<vector<char>>& board,
    int x, int y, char symbol, char opponent_symbol) {
    int score = 0;
    int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

    for (auto& dir : directions) {
        int my_count_pos = count_in_direction(board, x, y, symbol, dir[0], dir[1]);
        int my_count_neg = count_in_direction(board, x, y, symbol, -dir[0], -dir[1]);
        int total_my = my_count_pos + my_count_neg + 1;

        int opp_count_pos = count_in_direction(board, x, y, opponent_symbol, dir[0], dir[1]);
        int opp_count_neg = count_in_direction(board, x, y, opponent_symbol, -dir[0], -dir[1]);
        int total_opp = opp_count_pos + opp_count_neg + 1;

        if (total_my == 3) score += 1000;
        else if (total_my == 2) score += 100;
        else if (total_my == 1) score += 10;

        if (total_opp == 3) score += 800;
        else if (total_opp == 2) score += 80;

        int dist_to_center = abs(x - 2) + abs(y - 2);
        score += (5 - dist_to_center) * 2;
    }

    return score;
}

void FiveX5_AI::selectMove(FiveX5_Board* board, char ai_symbol, int& x, int& y) {
    const auto& matrix = board->get_board_matrix();
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    int best_score = INT_MIN;
    int best_x = -1, best_y = -1;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == '.') {
                int score = evaluate_move(matrix, i, j, ai_symbol, opponent_symbol);

                if (score > best_score) {
                    best_score = score;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }

    x = best_x;
    y = best_y;
}

FiveX5_UI::FiveX5_UI(FiveX5_AI* strategy) : UI<char>(2), ai_strategy(strategy) {}

FiveX5_UI::~FiveX5_UI() {
    if (ai_strategy) delete ai_strategy;
}

Player<char>* FiveX5_UI::create_player(string& name, char symbol, PlayerType type) {
    string actual_type = (type == PlayerType::HUMAN) ? "human" : "computer";

    cout << "Creating " << actual_type << " player: " << name << " ("
        << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

bool FiveX5_UI::is_valid_position(int x, int y) const {
    return x >= 0 && x < 5 && y >= 0 && y < 5;
}

void FiveX5_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    cout << "\n    ";
    for (int j = 0; j < 5; ++j)
        cout << setw(3) << j;
    cout << "\n";
    cout << "   " << string(18, '-') << "\n";

    for (int i = 0; i < 5; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < 5; ++j)
            cout << setw(3) << matrix[i][j];
        cout << "\n";
    }
    cout << "   " << string(18, '-') << "\n\n";
}

Move<char>* FiveX5_UI::get_move(Player<char>* player) {
    FiveX5_Board* game_board = static_cast<FiveX5_Board*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << "'s Turn (" << player->get_symbol() << ")\n";
        cout << "Empty cells: " << game_board->get_empty_cells() << "\n";

        int x, y;
        while (true) {
            cout << "Enter position (x y, 0-4): ";
            cin >> x >> y;

            if (!is_valid_position(x, y)) {
                cout << "Invalid position! Please enter values between 0 and 4.\n";
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
        cout << "\n" << player->get_name() << "'s Turn (Computer AI)\n";

        int x, y;
        ai_strategy->selectMove(game_board, player->get_symbol(), x, y);

        if (x != -1 && y != -1) {
            cout << "Computer places " << player->get_symbol() << " at (" << x << ", " << y << ")\n";
            return new Move<char>(x, y, player->get_symbol());
        }

        cout << "Computer: No valid move found (shouldn't happen)\n";
        return new Move<char>(0, 0, player->get_symbol());
    }

    return new Move<char>(0, 0, player->get_symbol());
}

FiveX5TicTacToe::FiveX5TicTacToe() {
    ai_strategy = new FiveX5_AI();
    ui = new FiveX5_UI(ai_strategy);
    board = new FiveX5_Board();
}

FiveX5TicTacToe::~FiveX5TicTacToe() {
    delete board;
    delete ui;
}

void FiveX5TicTacToe::runGame() {
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
    PlayerType type1 = (typeChoice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

    cout << "Choose Player 2 type:\n1. Human\n2. Computer\n==> ";
    int typeChoice2;
    cin >> typeChoice2;
    PlayerType type2 = (typeChoice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

    Player<char>** gamePlayers = new Player<char>*[2];
    gamePlayers[0] = ui->create_player(nameP1, 'X', type1);
    gamePlayers[1] = ui->create_player(nameP2, 'O', type2);

    gamePlayers[0]->set_board_ptr(board);
    gamePlayers[1]->set_board_ptr(board);

    ui->display_board_matrix(board->get_board_matrix());
    Player<char>* currentPlayer = gamePlayers[0];

    while (true) {
        for (int i : {0, 1}) {
            currentPlayer = gamePlayers[i];
            Move<char>* move = ui->get_move(currentPlayer);

            while (!board->update_board(move))
                move = ui->get_move(currentPlayer);

            ui->display_board_matrix(board->get_board_matrix());

            if (board->is_draw(currentPlayer)) {
                ui->display_message("Game Over - 24 Moves Completed!");
                break;
            }
        }

        if (board->is_draw(currentPlayer)) {
            break;
        }
    }

    cout << "\n========================================\n";
    cout << "FINAL RESULTS:\n";
    cout << nameP1 << " (X): " << board->get_x_sequences() << " three-in-a-row\n";
    cout << nameP2 << " (O): " << board->get_o_sequences() << " three-in-a-row\n";

    if (board->get_x_sequences() > board->get_o_sequences()) {
        cout << "\n" << nameP1 << " WINS!\n";
    }
    else if (board->get_o_sequences() > board->get_x_sequences()) {
        cout << "\n" << nameP2 << " WINS!\n";
    }
    else {
        cout << "\nIT'S A TIE!\n";
    }
    cout << "========================================\n\n";

    for (int i = 0; i < 2; ++i) {
        delete gamePlayers[i];
    }
    delete[] gamePlayers;
}

std::string FiveX5TicTacToe::getName() const {
    return "5x5 Tic-Tac-Toe";
}

std::string FiveX5TicTacToe::getDesc() const {
    return "5x5 Tic-Tac-Toe - Maximum Three-in-a-Row Battle!\n"
        "Players take turns placing marks on a 5x5 grid.\n"
        "The player with the most three-in-a-row sequences wins!\n"
        "Game ends when only 1 empty cell remains (24 total moves).";
}

std::pair<int, int> FiveX5TicTacToe::getBoardSize() const {
    return { 5, 5 };
}

void FiveX5TicTacToe::descripe() const {
    std::cout << "\n========================================\n";
    std::cout << getDesc() << "\n";
    std::cout << "Board Size: (" << getBoardSize().first << ", "
        << getBoardSize().second << ")\n";
    std::cout << "========================================\n\n";
}
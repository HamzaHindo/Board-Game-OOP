#include "FourX4TicTacToe.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

void FourX4_Board::initialize_positions() {
    x_positions.clear();
    o_positions.clear();

    x_positions.push_back({ 0, 0 });
    x_positions.push_back({ 0, 3 });
    x_positions.push_back({ 3, 0 });
    x_positions.push_back({ 3, 3 });

    o_positions.push_back({ 0, 1 });
    o_positions.push_back({ 0, 2 });
    o_positions.push_back({ 3, 1 });
    o_positions.push_back({ 3, 2 });
}

FourX4_Board::FourX4_Board() : Board<char>(4, 4) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;

    initialize_positions();

    for (auto& pos : x_positions)
        board[pos.first][pos.second] = 'X';

    for (auto& pos : o_positions)
        board[pos.first][pos.second] = 'O';
}

bool FourX4_Board::is_adjacent(int x1, int y1, int x2, int y2) {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return (dx + dy == 1) && (dx == 0 || dy == 0);
}

bool FourX4_Board::is_empty_cell(int x, int y) const {
    if (x < 0 || x >= 4 || y < 0 || y >= 4) return false;
    return board[x][y] == blank_symbol;
}

bool FourX4_Board::is_valid_move(int from_x, int from_y, int to_x, int to_y) {
    if (to_x < 0 || to_x >= 4 || to_y < 0 || to_y >= 4) return false;
    if (!is_empty_cell(to_x, to_y)) return false;
    if (!is_adjacent(from_x, from_y, to_x, to_y)) return false;
    return true;
}

bool FourX4_Board::move_token(int from_x, int from_y, int to_x, int to_y, char symbol) {
    if (!is_valid_move(from_x, from_y, to_x, to_y)) return false;

    board[from_x][from_y] = blank_symbol;
    board[to_x][to_y] = symbol;

    auto& positions = (symbol == 'X') ? x_positions : o_positions;
    for (auto& pos : positions) {
        if (pos.first == from_x && pos.second == from_y) {
            pos.first = to_x;
            pos.second = to_y;
            break;
        }
    }

    return true;
}

bool FourX4_Board::check_win(char player_symbol) {
    auto& positions = (player_symbol == 'X') ? x_positions : o_positions;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] != player_symbol) continue;

            int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
            for (auto& dir : directions) {
                int count = 1;
                for (int k = 1; k < 3; ++k) {
                    int ni = i + k * dir[0];
                    int nj = j + k * dir[1];
                    if (ni < 0 || ni >= 4 || nj < 0 || nj >= 4) break;
                    if (board[ni][nj] == player_symbol) count++;
                    else break;
                }

                if (count >= 3) {
                    int before = 0;
                    int ni = i - dir[0];
                    int nj = j - dir[1];
                    if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && board[ni][nj] == player_symbol) {
                        before = 1;
                    }

                    if (before == 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool FourX4_Board::is_win(Player<char>* player) {
    return check_win(player->get_symbol());
}

vector<pair<int, int>>& FourX4_Board::get_player_positions(char symbol) {
    return (symbol == 'X') ? x_positions : o_positions;
}

vector<TokenMove> FourX4_Board::get_valid_moves(char symbol) {
    vector<TokenMove> valid_moves;
    auto& positions = get_player_positions(symbol);

    for (auto& pos : positions) {
        int directions[][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (auto& dir : directions) {
            int nx = pos.first + dir[0];
            int ny = pos.second + dir[1];
            if (is_empty_cell(nx, ny)) {
                valid_moves.push_back(TokenMove(pos.first, pos.second, nx, ny, symbol));
            }
        }
    }
    return valid_moves;
}

int FourX4_AI::count_alignment(const FourX4_Board& board, int x, int y, char symbol) {
    int max_alignment = 0;
    const auto& matrix = board.get_board_matrix();

    int directions[][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    for (auto& dir : directions) {
        int count = 1;
        for (int k = 1; k < 3; ++k) {
            int nx = x + k * dir[0];
            int ny = y + k * dir[1];
            if (nx < 0 || nx >= 4 || ny < 0 || ny >= 4) break;
            if (matrix[nx][ny] == symbol) count++;
            else break;
        }

        for (int k = 1; k < 3; ++k) {
            int nx = x - k * dir[0];
            int ny = y - k * dir[1];
            if (nx < 0 || nx >= 4 || ny < 0 || ny >= 4) break;
            if (matrix[nx][ny] == symbol) count++;
            else break;
        }

        max_alignment = max(max_alignment, count);
    }
    return max_alignment;
}

bool FourX4_AI::would_win(const FourX4_Board& board, const TokenMove& move, char symbol) {
    FourX4_Board temp_board = board;
    temp_board.move_token(move.from_x, move.from_y, move.to_x, move.to_y, symbol);
    return temp_board.check_win(symbol);
}

bool FourX4_AI::blocks_opponent_win(const FourX4_Board& board, const TokenMove& move, char ai_symbol, char opponent_symbol) {
    FourX4_Board temp_board = board;
    temp_board.move_token(move.from_x, move.from_y, move.to_x, move.to_y, ai_symbol);

    auto opponent_moves = temp_board.get_valid_moves(opponent_symbol);
    for (auto& opp_move : opponent_moves) {
        FourX4_Board check_board = temp_board;
        check_board.move_token(opp_move.from_x, opp_move.from_y, opp_move.to_x, opp_move.to_y, opponent_symbol);
        if (check_board.check_win(opponent_symbol)) {
            return false;
        }
    }
    return true;
}

int FourX4_AI::evaluate_move(const FourX4_Board& board, const TokenMove& move, char ai_symbol, char opponent_symbol) {
    int score = 0;

    if (would_win(board, move, ai_symbol)) {
        return 10000;
    }

    FourX4_Board temp_board = board;
    temp_board.move_token(move.from_x, move.from_y, move.to_x, move.to_y, ai_symbol);

    int alignment = count_alignment(temp_board, move.to_x, move.to_y, ai_symbol);
    if (alignment == 3) score += 1000;
    else if (alignment == 2) score += 100;
    else score += 10;

    if (!blocks_opponent_win(board, move, ai_symbol, opponent_symbol)) {
        score -= 500;
    }

    int dist_to_center = abs(move.to_x - 1.5) + abs(move.to_y - 1.5);
    score += (4 - dist_to_center) * 5;

    return score;
}

TokenMove FourX4_AI::selectMove(FourX4_Board* board, char ai_symbol) {
    char opponent_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    auto valid_moves = board->get_valid_moves(ai_symbol);

    if (valid_moves.empty()) {
        return TokenMove(-1, -1, -1, -1, ai_symbol);
    }

    TokenMove best_move = valid_moves[0];
    int best_score = INT_MIN;

    for (auto& move : valid_moves) {
        int score = evaluate_move(*board, move, ai_symbol, opponent_symbol);
        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    return best_move;
}

FourX4_UI::FourX4_UI(FourX4_AI* strategy) : UI<char>(2), ai_strategy(strategy) {}

FourX4_UI::~FourX4_UI() {
    if (ai_strategy) delete ai_strategy;
}

Player<char>* FourX4_UI::create_player(string& name, char symbol, PlayerType type) {
    string actual_type = (type == PlayerType::HUMAN) ? "human" : "computer";
    cout << "Creating " << actual_type << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

void FourX4_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty()) return;

    cout << "\n    ";
    for (int j = 0; j < 4; ++j)
        cout << setw(3) << j;
    cout << "\n";
    cout << "   " << string(14, '-') << "\n";

    for (int i = 0; i < 4; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < 4; ++j)
            cout << setw(3) << matrix[i][j];
        cout << "\n";
    }
    cout << "   " << string(14, '-') << "\n\n";
}

TokenMove FourX4_UI::get_token_move(Player<char>* player, FourX4_Board* board) {
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << "'s Turn (" << player->get_symbol() << ")\n";

        int from_x, from_y, to_x, to_y;
        while (true) {
            cout << "Enter token position (from_x from_y): ";
            cin >> from_x >> from_y;

            if (from_x < 0 || from_x >= 4 || from_y < 0 || from_y >= 4) {
                cout << "Invalid position!\n";
                continue;
            }

            if (board->get_board_matrix()[from_x][from_y] != player->get_symbol()) {
                cout << "That's not your token!\n";
                continue;
            }

            cout << "Enter destination (to_x to_y): ";
            cin >> to_x >> to_y;

            auto valid_moves = board->get_valid_moves(player->get_symbol());
            bool found = false;
            for (auto& move : valid_moves) {
                if (move.from_x == from_x && move.from_y == from_y &&
                    move.to_x == to_x && move.to_y == to_y) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Invalid move! Token can only move to adjacent empty squares.\n";
                continue;
            }

            break;
        }
        return TokenMove(from_x, from_y, to_x, to_y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        cout << "\n" << player->get_name() << " (Computer AI) is thinking...\n";
        TokenMove move = ai_strategy->selectMove(board, player->get_symbol());
        cout << "Computer moves from (" << move.from_x << ", " << move.from_y
            << ") to (" << move.to_x << ", " << move.to_y << ")\n";
        return move;
    }

    return TokenMove(-1, -1, -1, -1, '.');
}

FourX4TicTacToe::FourX4TicTacToe() {
    ai_strategy = new FourX4_AI();
    ui = new FourX4_UI(ai_strategy);
    board = new FourX4_Board();
}

FourX4TicTacToe::~FourX4TicTacToe() {
    delete board;
    delete ui;
}

void FourX4TicTacToe::runGame() {
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

    ui->display_board_matrix(board->get_board_matrix());
    Player<char>* currentPlayer = gamePlayers[0];

    while (true) {
        for (int i : {0, 1}) {
            currentPlayer = gamePlayers[i];

            TokenMove move = ui->get_token_move(currentPlayer, board);

            while (!board->move_token(move.from_x, move.from_y, move.to_x, move.to_y, move.symbol)) {
                cout << "Invalid move! Try again.\n";
                move = ui->get_token_move(currentPlayer, board);
            }

            ui->display_board_matrix(board->get_board_matrix());

            if (board->is_win(currentPlayer)) {
                cout << "\n========================================\n";
                cout << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") WINS!\n";
                cout << "========================================\n\n";
                goto end_game;
            }
        }
    }

end_game:
    for (int i = 0; i < 2; ++i) {
        delete gamePlayers[i];
    }
    delete[] gamePlayers;
}

std::string FourX4TicTacToe::getName() const {
    return "4x4 Tic-Tac-Toe";
}

std::string FourX4TicTacToe::getDesc() const {
    return "4x4 Tic-Tac-Toe - Token Movement Game!\n"
        "Each player starts with 4 tokens in corner positions.\n"
        "Move your tokens to adjacent empty squares (horizontal/vertical only).\n"
        "First player to align 3 tokens in a row wins!";
}

std::pair<int, int> FourX4TicTacToe::getBoardSize() const {
    return { 4, 4 };
}

void FourX4TicTacToe::descripe() const {
    std::cout << "\n========================================\n";
    std::cout << getDesc() << "\n";
    std::cout << "Board Size: (" << getBoardSize().first << ", "
        << getBoardSize().second << ")\n";
    std::cout << "========================================\n\n";
}
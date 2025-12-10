#ifndef FOUR_X_4_TIC_TAC_TOE_H
#define FOUR_X_4_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <string>
#include <vector>
#include <utility>

using namespace std;

struct TokenMove {
    int from_x, from_y;
    int to_x, to_y;
    char symbol;

    TokenMove(int fx = -1, int fy = -1, int tx = -1, int ty = -1, char s = '.')
        : from_x(fx), from_y(fy), to_x(tx), to_y(ty), symbol(s) {
    }
};

class FourX4_Board : public Board<char> {
private:
    char blank_symbol = '.';
    vector<pair<int, int>> x_positions;
    vector<pair<int, int>> o_positions;

    void initialize_positions();
    bool is_adjacent(int x1, int y1, int x2, int y2);
    bool is_valid_move(int from_x, int from_y, int to_x, int to_y);

public:
    bool check_win(char player_symbol);

public:
    FourX4_Board();
    bool update_board(Move<char>* move) { return false; }

    bool move_token(int from_x, int from_y, int to_x, int to_y, char symbol);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; }
    bool is_draw(Player<char>* player) { return false; }
    bool game_is_over(Player<char>* player) { return is_win(player); }

    vector<pair<int, int>>& get_player_positions(char symbol);
    vector<TokenMove> get_valid_moves(char symbol);
    bool is_empty_cell(int x, int y) const;
};

class FourX4_AI {
private:
    int evaluate_move(const FourX4_Board& board, const TokenMove& move, char ai_symbol, char opponent_symbol);
    int count_alignment(const FourX4_Board& board, int x, int y, char symbol);
    bool would_win(const FourX4_Board& board, const TokenMove& move, char symbol);
    bool blocks_opponent_win(const FourX4_Board& board, const TokenMove& move, char ai_symbol, char opponent_symbol);

public:
    TokenMove selectMove(FourX4_Board* board, char ai_symbol);
};

class FourX4_UI : public UI<char> {
private:
    FourX4_AI* ai_strategy;

public:
    FourX4_UI(FourX4_AI* strategy = nullptr);
    ~FourX4_UI();

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player) { return nullptr; }
    TokenMove get_token_move(Player<char>* player, FourX4_Board* board);
    void display_board_matrix(const vector<vector<char>>& matrix) const;
};

class FourX4TicTacToe : public IGame {
private:
    FourX4_UI* ui;
    FourX4_Board* board;
    FourX4_AI* ai_strategy;

public:
    FourX4TicTacToe();
    ~FourX4TicTacToe();

    void runGame() override;
    std::string getName() const override;
    std::string getDesc() const override;
    std::pair<int, int> getBoardSize() const override;
    void descripe() const override;
};

#endif
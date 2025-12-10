#ifndef FIVE_X_5_TIC_TAC_TOE_H
#define FIVE_X_5_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <string>
#include <vector>

using namespace std;

class FiveX5_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int x_sequences = 0;
    int o_sequences = 0;

    int count_sequences(char player_symbol);
    bool creates_sequence(int x, int y, char symbol, int dr, int dc);

public:
    FiveX5_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; }
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    int get_x_sequences() const { return x_sequences; }
    int get_o_sequences() const { return o_sequences; }
    int get_empty_cells() const;
    void update_sequences();
};

class FiveX5_AI {
private:
    int calculate_position_score(const vector<vector<char>>& board,
        int x, int y, char ai_symbol, char opponent_symbol);
    int count_in_direction(const vector<vector<char>>& board,
        int x, int y, char symbol, int dr, int dc);
    int evaluate_move(const vector<vector<char>>& board,
        int x, int y, char symbol, char opponent_symbol);

public:
    void selectMove(FiveX5_Board* board, char ai_symbol, int& x, int& y);
};

class FiveX5_UI : public UI<char> {
private:
    FiveX5_AI* ai_strategy;

public:
    FiveX5_UI(FiveX5_AI* strategy = nullptr);
    ~FiveX5_UI();
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
    void display_board_matrix(const vector<vector<char>>& matrix) const;

private:
    bool is_valid_position(int x, int y) const;
};

class FiveX5TicTacToe : public IGame {
private:
    FiveX5_UI* ui;
    FiveX5_Board* board;
    FiveX5_AI* ai_strategy;

public:
    FiveX5TicTacToe();
    ~FiveX5TicTacToe();

    void runGame() override;
    std::string getName() const override;
    std::string getDesc() const override;
    std::pair<int, int> getBoardSize() const override;
    void descripe() const override;
};

#endif
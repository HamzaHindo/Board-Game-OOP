#ifndef INFINITY_TIC_TAC_TOE_H
#define INFINITY_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "IGame.h"
#include <queue>
#include <string>
#include <vector>
#include <map>

using namespace std;


class TimestampedMove {
public:
    int x;             
    int y;              
    char symbol;        
    int move_number;   

    
    TimestampedMove(int x = 0, int y = 0, char symbol = '.', int move_number = 0)
        : x(x), y(y), symbol(symbol), move_number(move_number) {
    }
};


class InfinityTTT_Board : public Board<char> {
private:
    char blank_symbol = '.';
    map<char, queue<TimestampedMove>> player_moves;    
    int total_moves = 0;                               

    
    void remove_oldest_move(char player_symbol);

public:
    
    InfinityTTT_Board();

   
    bool update_board(Move<char>* move);

    
    bool is_win(Player<char>* player);

    
    bool is_lose(Player<char>*) { return false; }

    
    bool is_draw(Player<char>* player);

    
    bool game_is_over(Player<char>* player);

    
    int get_total_moves() const { return total_moves; }

    
    size_t get_player_move_count(char symbol) const;
};

class InfinityTTT_UI : public UI<char> {
public:
    
    InfinityTTT_UI();

    
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    
    virtual Move<char>* get_move(Player<char>* player);

private:
    
    bool is_valid_position(int x, int y) const;
};


class InfinityTicTacToe : public IGame {
private:
    InfinityTTT_UI* ui;
    InfinityTTT_Board* board;

public:
    InfinityTicTacToe();
    ~InfinityTicTacToe();

    void runGame() override;
    std::string getName() const override;
    std::string getDesc() const override;
    std::pair<int, int> getBoardSize() const override;
    void descripe() const override;
};

#endif 
#ifndef _GAME3_5X5_TICTACTOE_H
#define _GAME3_5X5_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename T>
class Game3_Board : public Board<T> {
public:
    Game3_Board() : Board<T>(5, 5) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                this->board[i][j] = 0;
            }
        }
    }

    bool update_board(Move<T>* move) override {
        int x = move->get_x();
        int y = move->get_y();

        if (x < 0 || x >= 5 || y < 0 || y >= 5) {
            return false;
        }

        if (this->board[x][y] != 0) {
            return false;
        }

        if (this->n_moves >= 24) {
            return false;
        }

        this->board[x][y] = toupper(move->get_symbol());
        this->n_moves++;
        return true;
    }

    int count_three_in_a_row(T symbol) {
        int count = 0;

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j <= 2; j++) {
                if (this->board[i][j] == symbol &&
                    this->board[i][j + 1] == symbol &&
                    this->board[i][j + 2] == symbol) {
                    count++;
                }
            }
        }

        for (int i = 0; i <= 2; i++) {
            for (int j = 0; j < 5; j++) {
                if (this->board[i][j] == symbol &&
                    this->board[i + 1][j] == symbol &&
                    this->board[i + 2][j] == symbol) {
                    count++;
                }
            }
        }

        for (int i = 0; i <= 2; i++) {
            for (int j = 0; j <= 2; j++) {
                if (this->board[i][j] == symbol &&
                    this->board[i + 1][j + 1] == symbol &&
                    this->board[i + 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        for (int i = 0; i <= 2; i++) {
            for (int j = 2; j < 5; j++) {
                if (this->board[i][j] == symbol &&
                    this->board[i + 1][j - 1] == symbol &&
                    this->board[i + 2][j - 2] == symbol) {
                    count++;
                }
            }
        }

        return count;
    }

    bool is_win(Player<T>* player) override {
        if (this->n_moves < 24) {
            return false;
        }

        T player_symbol = toupper(player->get_symbol());
        T opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';

        int player_count = count_three_in_a_row(player_symbol);
        int opponent_count = count_three_in_a_row(opponent_symbol);

        return player_count > opponent_count;
    }

    bool is_lose(Player<T>* player) override {
        if (this->n_moves < 24) {
            return false;
        }

        T player_symbol = toupper(player->get_symbol());
        T opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';

        int player_count = count_three_in_a_row(player_symbol);
        int opponent_count = count_three_in_a_row(opponent_symbol);

        return player_count < opponent_count;
    }

    bool is_draw(Player<T>* player) override {
        if (this->n_moves < 24) {
            return false;
        }

        T player_symbol = toupper(player->get_symbol());
        T opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';

        int player_count = count_three_in_a_row(player_symbol);
        int opponent_count = count_three_in_a_row(opponent_symbol);

        return player_count == opponent_count;
    }

    bool game_is_over(Player<T>* player) override {
        return (this->n_moves >= 24);
    }
};

template <typename T>
class Game3_Random_Player : public Player<T> {
public:
    Game3_Random_Player(T symbol) : Player<T>("Random Computer", symbol, PlayerType::RANDOM) {
        srand(static_cast<unsigned int>(time(0)));
    }

    Move<T>* get_move() {
        int x, y;
        do {
            x = rand() % 5;
            y = rand() % 5;
        } while (this->boardPtr->get_board_matrix()[x][y] != 0);

        return new Move<T>(x, y, this->symbol);
    }
};

template <typename T>
class Game3_UI : public UI<T> {
public:
    Game3_UI() : UI<T>("Welcome to 5x5 Tic Tac Toe!\n"
        "Players alternate placing marks on a 5x5 grid.\n"
        "Game ends when only 1 square remains empty (24 moves total).\n"
        "Winner is the player with the most three-in-a-row sequences!\n", 3) {
    }

    Move<T>* get_move(Player<T>* player) override {
        if (player->get_type() == PlayerType::RANDOM) {
            Game3_Random_Player<T>* comp_player = dynamic_cast<Game3_Random_Player<T>*>(player);
            if (comp_player) {
                this->display_message(player->get_name() + " is making a move...");
                return comp_player->get_move();
            }
        }

        int x, y;
        cout << player->get_name() << " (" << player->get_symbol() << "), enter your move (row col): ";
        cin >> x >> y;

        return new Move<T>(x, y, player->get_symbol());
    }

    Player<T>* create_player(string& name, T symbol, PlayerType type) override {
        if (type == PlayerType::COMPUTER || type == PlayerType::RANDOM) {
            return new Game3_Random_Player<T>(symbol);
        }
        return new Player<T>(name, symbol, PlayerType::HUMAN);
    }
};

#endif
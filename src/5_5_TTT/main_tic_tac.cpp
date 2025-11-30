#include <iostream>
#include "BoardGame_Classes.h"
#include "Game3_5x5_TicTacToe.h"

using namespace std;

int main() {
    Game3_Board<char>* board = new Game3_Board<char>();
    Game3_UI<char>* ui = new Game3_UI<char>();

    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

    return 0;
}
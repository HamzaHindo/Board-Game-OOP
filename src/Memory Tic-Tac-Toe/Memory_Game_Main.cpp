#include "BoardGame_Classes.h"
#include "Memory_Tic_Tac_Toe.h"
#include <iostream>

using namespace std;

int main() {
    cout << "\n============================================\n";
    cout << "   MEMORY TIC-TAC-TOE - BONUS GAME #13\n";
    cout << "============================================\n";
    cout << "Game Rules:\n";
    cout << "- 3x3 board\n";
    cout << "- Marks are HIDDEN after placement\n";
    cout << "- Remember positions to win\n";
    cout << "- First to get 3 in a row wins\n";
    cout << "- Board revealed at game end\n";
    cout << "============================================\n\n";

    Memory_Board<char>* board = new Memory_Board<char>();
    Memory_UI<char>* ui = new Memory_UI<char>();
    Player<char>** players = ui->setup_players();

    Memory_GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

    cout << "\n============================================\n";
    cout << "Thank you for playing Memory Tic-Tac-Toe!\n";
    cout << "============================================\n\n";

    return 0;
}
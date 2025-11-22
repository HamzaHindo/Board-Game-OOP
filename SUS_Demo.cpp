#include "BoardGame_Classes.h"
#include "SUS_Game.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Starting SUS Game...\n\n";

    // Create the game components
    SUS_Board* board = new SUS_Board();
    SUS_UI* ui = new SUS_UI();

    // Setup players
    Player<char>** players = ui->setup_players();

    // Set player symbols in board for score tracking
    board->set_player_symbols(players[0]->get_symbol(), players[1]->get_symbol());

    // Set board pointers for players
    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Set board pointer in UI for score display
    ui->set_sus_board(board);

    // Create and run game manager (DO NOT MODIFY GameManager - Framework Rule!)
    GameManager<char>* game = new GameManager<char>(board, players, ui);
    game->run();

    // Display final detailed scores
    pair<int, int> final_scores = board->get_scores();
    ui->display_final_result(
        (final_scores.first > final_scores.second) ? players[0]->get_name() + " wins!" :
        (final_scores.second > final_scores.first) ? players[1]->get_name() + " wins!" :
        "It's a draw!",
        final_scores
    );

    // Cleanup
    delete game;
    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

    return 0;
}
#ifndef _SUS_GAME_H
#define _SUS_GAME_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <set>

using namespace std;

/**
 * @brief Board class for SUS game
 *
 * A 3x3 board game where players place 'S' or 'U' letters
 * to form "S-U-S" sequences horizontally, vertically, or diagonally.
 * Player scores a point for EACH "S-U-S" sequence they create.
 */
class SUS_Board : public Board<char> {
private:
    int player1_score = 0;  ///< Player 1's SUS count
    int player2_score = 0;  ///< Player 2's SUS count
    char player1_symbol;    ///< Player 1's chosen letter
    char player2_symbol;    ///< Player 2's chosen letter
    set<string> counted_sus; ///< Track already counted SUS sequences
    char last_player_symbol; ///< Symbol of last player who moved

    /**
     * @brief Count NEW SUS sequences after a move
     * @param player_symbol The symbol of player who just moved
     * @return Number of NEW SUS sequences formed
     */
    int count_new_sus_sequences(char player_symbol) {
        int new_count = 0;
        set<string> current_sus;

        // Check all possible SUS sequences on board
        // Horizontal
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == 'S' && board[i][1] == 'U' && board[i][2] == 'S') {
                string key = "H" + to_string(i);
                current_sus.insert(key);
                if (counted_sus.find(key) == counted_sus.end()) {
                    new_count++;
                    counted_sus.insert(key);
                }
            }
        }

        // Vertical
        for (int j = 0; j < 3; j++) {
            if (board[0][j] == 'S' && board[1][j] == 'U' && board[2][j] == 'S') {
                string key = "V" + to_string(j);
                current_sus.insert(key);
                if (counted_sus.find(key) == counted_sus.end()) {
                    new_count++;
                    counted_sus.insert(key);
                }
            }
        }

        // Diagonal (top-left to bottom-right)
        if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S') {
            string key = "D1";
            current_sus.insert(key);
            if (counted_sus.find(key) == counted_sus.end()) {
                new_count++;
                counted_sus.insert(key);
            }
        }

        // Diagonal (top-right to bottom-left)
        if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S') {
            string key = "D2";
            current_sus.insert(key);
            if (counted_sus.find(key) == counted_sus.end()) {
                new_count++;
                counted_sus.insert(key);
            }
        }

        return new_count;
    }

public:
    /**
     * @brief Construct a 3x3 SUS board
     */
    SUS_Board() : Board<char>(3, 3) {
        // Initialize board with empty cells
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = ' ';
            }
        }
        player1_symbol = ' ';
        player2_symbol = ' ';
        last_player_symbol = ' ';
    }

    /**
     * @brief Set player symbols
     */
    void set_player_symbols(char p1, char p2) {
        player1_symbol = p1;
        player2_symbol = p2;
    }

    /**
     * @brief Update the board with a new move
     * @param move The move containing position and symbol
     * @return true if move is valid and applied, false otherwise
     */
    bool update_board(Move<char>* move) override {
        int x = move->get_x();
        int y = move->get_y();
        char symbol = move->get_symbol();

        // Validate position
        if (x < 0 || x >= 3 || y < 0 || y >= 3) {
            cout << "Invalid position! Must be between 0 and 2.\n";
            return false;
        }

        // Check if cell is empty
        if (board[x][y] != ' ') {
            cout << "Cell is already occupied!\n";
            return false;
        }

        // Validate symbol (must be S or U)
        if (symbol != 'S' && symbol != 'U') {
            cout << "Invalid symbol! Must be 'S' or 'U'.\n";
            return false;
        }

        // Place the symbol
        board[x][y] = symbol;
        last_player_symbol = symbol;
        n_moves++;

        // Count new SUS sequences formed by this move
        int new_sus = count_new_sus_sequences(symbol);

        // Add points to the correct player
        if (new_sus > 0) {
            if (symbol == player1_symbol) {
                player1_score += new_sus;
                cout << "Player 1 formed " << new_sus << " SUS sequence(s)! Total: " << player1_score << endl;
            }
            else {
                player2_score += new_sus;
                cout << "Player 2 formed " << new_sus << " SUS sequence(s)! Total: " << player2_score << endl;
            }
        }

        return true;
    }

    /**
     * @brief Check if a player has won
     * Game ends when board is full, winner is who has more SUS sequences
     */
    bool is_win(Player<char>* player) override {
        if (n_moves < 9) return false;

        // Determine winner based on scores
        if (player->get_symbol() == player1_symbol) {
            return player1_score > player2_score;
        }
        else {
            return player2_score > player1_score;
        }
    }

    /**
     * @brief Check if player lost
     */
    bool is_lose(Player<char>* player) override {
        if (n_moves < 9) return false;

        // Player loses if opponent has more points
        if (player->get_symbol() == player1_symbol) {
            return player1_score < player2_score;
        }
        else {
            return player2_score < player1_score;
        }
    }

    /**
     * @brief Check for draw (equal scores when board is full)
     */
    bool is_draw(Player<char>* player) override {
        if (n_moves < 9) return false;
        return (player1_score == player2_score);
    }

    /**
     * @brief Check if game is over
     */
    bool game_is_over(Player<char>* player) override {
        return (n_moves >= 9);
    }

    /**
     * @brief Get current scores
     */
    pair<int, int> get_scores() const {
        return { player1_score, player2_score };
    }

    /**
     * @brief Display current scores
     */
    void display_scores() const {
        cout << "\n--- Current Scores ---\n";
        cout << "Player 1: " << player1_score << " SUS sequences\n";
        cout << "Player 2: " << player2_score << " SUS sequences\n";
        cout << "---------------------\n";
    }
};

/**
 * @brief Player class for SUS game
 * Each player chooses either 'S' or 'U' as their letter
 */
class SUS_Player : public Player<char> {
public:
    /**
     * @brief Construct a SUS player
     * @param name Player's name
     * @param symbol Player's chosen letter ('S' or 'U')
     * @param type Player type (HUMAN or COMPUTER)
     */
    SUS_Player(string name, char symbol, PlayerType type)
        : Player<char>(name, symbol, type) {}
};

/**
 * @brief Random computer player for SUS game
 */
class SUS_Random_Player : public SUS_Player {
public:
    SUS_Random_Player(string name, char symbol)
        : SUS_Player(name, symbol, PlayerType::RANDOM) {
        srand(static_cast<unsigned>(time(0)));
    }
};

/**
 * @brief UI class for SUS game
 */
class SUS_UI : public UI<char> {
private:
    SUS_Board* sus_board_ptr = nullptr;

public:
    /**
     * @brief Construct SUS game UI
     */
    SUS_UI() : UI<char>("=== Welcome to SUS Game! ===", 3) {
        cout << "\nGame Rules:\n";
        cout << "- Each player chooses either 'S' or 'U' as their letter\n";
        cout << "- Players take turns placing their letter on the 3x3 board\n";
        cout << "- Score points for EACH 'S-U-S' sequence you form\n";
        cout << "  (horizontal, vertical, or diagonal)\n";
        cout << "- Player with most SUS sequences wins!\n\n";
    }

    /**
     * @brief Set the SUS board pointer for score display
     */
    void set_sus_board(SUS_Board* board) {
        sus_board_ptr = board;
    }

    /**
     * @brief Get a move from the player (human or computer)
     */
    Move<char>* get_move(Player<char>* player) override {
        // Display current scores before each move
        if (sus_board_ptr) {
            sus_board_ptr->display_scores();
        }

        if (player->get_type() == PlayerType::RANDOM ||
            player->get_type() == PlayerType::COMPUTER) {
            cout << "\n" << player->get_name() << "'s turn (Computer - "
                << player->get_symbol() << ")...\n";

            // Find random empty cell
            vector<vector<char>> board_matrix = player->get_board_ptr()->get_board_matrix();
            vector<pair<int, int>> empty_cells;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board_matrix[i][j] == ' ') {
                        empty_cells.push_back({ i, j });
                    }
                }
            }

            if (!empty_cells.empty()) {
                int idx = rand() % empty_cells.size();
                cout << "Computer plays at (" << empty_cells[idx].first
                    << ", " << empty_cells[idx].second << ")\n";
                return new Move<char>(empty_cells[idx].first,
                    empty_cells[idx].second,
                    player->get_symbol());
            }
            return nullptr;
        }

        // Human player
        cout << "\n" << player->get_name() << "'s turn (Your letter: "
            << player->get_symbol() << ")\n";
        int x, y;
        cout << "Enter row (0-2): ";
        cin >> x;
        cout << "Enter column (0-2): ";
        cin >> y;

        return new Move<char>(x, y, player->get_symbol());
    }

    /**
     * @brief Create a player based on type
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        if (type == PlayerType::RANDOM || type == PlayerType::COMPUTER) {
            return new SUS_Random_Player(name, symbol);
        }
        return new SUS_Player(name, symbol, type);
    }

    /**
     * @brief Setup two players for SUS game
     */
    Player<char>** setup_players() override {
        Player<char>** players = new Player<char>*[2];
        vector<string> type_options = { "Human", "Computer" };

        // Player 1
        string name1 = get_player_name("Player 1");
        char symbol1;
        cout << "Player 1, choose your letter (S or U): ";
        cin >> symbol1;
        symbol1 = toupper(symbol1);
        while (symbol1 != 'S' && symbol1 != 'U') {
            cout << "Invalid! Choose S or U: ";
            cin >> symbol1;
            symbol1 = toupper(symbol1);
        }
        PlayerType type1 = get_player_type_choice("Player 1", type_options);
        players[0] = create_player(name1, symbol1, type1);

        // Player 2
        string name2 = get_player_name("Player 2");
        char symbol2;
        cout << "Player 2, choose your letter (S or U): ";
        cin >> symbol2;
        symbol2 = toupper(symbol2);
        while (symbol2 != 'S' && symbol2 != 'U') {
            cout << "Invalid! Choose S or U: ";
            cin >> symbol2;
            symbol2 = toupper(symbol2);
        }
        PlayerType type2 = get_player_type_choice("Player 2", type_options);
        players[1] = create_player(name2, symbol2, type2);

        return players;
    }

    /**
     * @brief Override display message to show final scores
     */
    void display_final_result(string message, pair<int, int> scores) {
        cout << "\n================================\n";
        cout << "       GAME OVER!\n";
        cout << "================================\n";
        cout << "Final Scores:\n";
        cout << "Player 1: " << scores.first << " SUS sequences\n";
        cout << "Player 2: " << scores.second << " SUS sequences\n";
        cout << "--------------------------------\n";
        cout << message << "\n";
        cout << "================================\n";
    }
};

#endif // _SUS_GAME_H
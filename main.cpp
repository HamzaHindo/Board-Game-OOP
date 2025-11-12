#include "Numerical_TicTacToe.h"

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::vector<std::shared_ptr<IGame>> games;
    games.push_back(std::make_shared<NumericalTicTacToe>());

    Menu menu(games);
    menu.runMenuLoop();

    return 0;
}
#include "Menu.h"
#include <iostream>
#include <limits>


Menu::Menu(const std::vector<std::shared_ptr<IGame>> games): games(games) {};

void Menu::runMenuLoop() {

	std::cout << "   (                    (       (                              \n"
		" ( )\\          )  (     )\\ )    )\\ )       )     )      (      \n"
		" )((_)  (   ( /(  )(   (()/(   (()/(    ( /(    (      ))\\ (   \n"
		"((_)_   )\\  )(_))(()\\   ((_))   /(_))_  )(_))   )\\  ' /((_))\\  \n"
		" | _ ) ((_)((_)_  ((_)  _| |   (_)) __|((_)_  _((_)) (_)) ((_) \n"
		" | _ \\/ _ \\/ _` || '_|/ _` |     | (_ |/ _` || '  \\()/ -_)(_-< \n"
		" |___/\\___/\\__,_||_|  \\__,_|      \\___|\\__,_||_|_|_| \\___|/__/ \n"
		"                                                                \n";


	std::cout << "Please Choose a Game To Play: \n";

	for (int i = 0; i < games.size(); i++) {
		std::cout << i + 1 << "] " << games[i]->getName() << ".\n";
	}
	int choice;
	while (true) {
		std::cout << "==> ";
			std::cin >> choice;
			if (std::cin.fail()) {
				std::cout << "Input type mismatch!\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if ((choice <= games.size() && choice >= 1)) {
				break;
			}
			else {
				std::cout << "Please enter a valid number! \n";
			}
	}

	if (choice == 1) {
		games[choice - 1]->runGame();
	}

}

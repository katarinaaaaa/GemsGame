#include "game_manager.h"

int main() {
	GameManager game;
	int countOfClicks = 0;
	sf::Vector2i previouslyChosenGemIndex;

	while (game.window.isOpen()) {		
		game.ManageEvents(countOfClicks, previouslyChosenGemIndex);
		game.DrawGame();
	}

	return 0;
}

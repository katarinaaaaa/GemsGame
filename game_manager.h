#pragma once
#include <string>
#include "field.h"
#include "bonus.h"

class GameManager {
	static const int WINDOW_WIDTH = 710;
	static const int WINDOW_HEIGHT = 770;

	static const int OFFSET_TOP = 66; //indent from the top border of the window
	static const int OFFSET_LEFT = 3;
	static const int OFFSET_BOTTOM = 4;
	static const int OFFSET_RIGHT = 7;

	static const int POINTS_PER_GEM = 10; //number of points for one destroyed gem

	static const int TEXT_X = 15; //coordinate of the score on the window
	static const int TEXT_Y = 10;

	sf::Texture textures;
	const std::string scoreTitle = "Score: ";
	sf::Text scoreLine; //sfml-object for displaying score
	sf::Font font;


	Field* field = new Field();
	Bonus* bonus = nullptr;
	int score = 0; //score counter

	bool IsClickInRange(const sf::Vector2i& mousePos);
	void ProcessMove(const sf::Vector2i& previouslyChosenGemIndex, const sf::Vector2i& chosenGemIndex);
	void UpdateScore(int numberOfDeletedGems);

public:

	sf::RenderWindow window{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "gems", sf::Style::Close };

	GameManager();
	~GameManager();

	void DrawGame();
	void ManageEvents(int& countOfClicks, sf::Vector2i& previouslyChosenGemIndex);
};

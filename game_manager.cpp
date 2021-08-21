#include "game_manager.h"

GameManager::GameManager() {
	textures.loadFromFile("textures.png");
	field->FillGrid(OFFSET_LEFT, OFFSET_TOP, &textures);
	font.loadFromFile("dolcevita.ttf");
	scoreLine.setFont(font);
	scoreLine.setCharacterSize(40);
	scoreLine.setFillColor(sf::Color::Black);
	scoreLine.setString(scoreTitle + std::to_string(score));
	scoreLine.setPosition(sf::Vector2f(TEXT_X, TEXT_Y));
}

GameManager::~GameManager() {
	delete field;
	if (bonus != nullptr)
		delete bonus;
}

void GameManager::DrawGame() {
	window.clear(sf::Color(219, 251, 255, 0));
	field->DisplayField(window);

	scoreLine.setString(scoreTitle + std::to_string(score));
	window.draw(scoreLine);

	window.display();
}

bool GameManager::IsClickInRange(const sf::Vector2i& mousePos) {
	return (mousePos.x > OFFSET_LEFT && mousePos.y > OFFSET_TOP && mousePos.x < (WINDOW_WIDTH - OFFSET_RIGHT) && mousePos.y < (WINDOW_HEIGHT - OFFSET_BOTTOM));
}

void GameManager::ProcessMove(const sf::Vector2i& previouslyChosenGemIndex, const sf::Vector2i& chosenGemIndex) {
	int numberOfDeletedGems = field->ProcessSwappingCombos(previouslyChosenGemIndex, chosenGemIndex);
	if (numberOfDeletedGems != 0) { //if swapping was successful - bonus may be generated
		if (std::rand() % 4 != 0) {
			if (std::rand() % 2 == 0)
				bonus = new Repainter();
			else
				bonus = new Eraser();
			bonus->SetPosition(previouslyChosenGemIndex, field);
			bonus->SetTexture(field);
		}
	}
	UpdateScore(numberOfDeletedGems);
	DrawGame();
	sf::sleep(sf::seconds(0.5f));
	if (numberOfDeletedGems == 0)
		return;

	int bonusDropDepth = field->DropGemsAfterDeleting(window);
	field->CreateNewGems(window);
	sf::sleep(sf::seconds(0.1f));

	if (bonus != nullptr) { //if bonus was generated
		bonus->ResetPosition(bonusDropDepth);
		if (bonus->Use(field) == true) { //if any gems are deleted
			UpdateScore(5);
			DrawGame();
			sf::sleep(sf::seconds(0.5f));
			field->DropGemsAfterDeleting(window);
			field->CreateNewGems(window);
		}
		else {
			DrawGame();
			sf::sleep(sf::seconds(0.5f));
		}
		delete bonus; //deactivate bonus
		bonus = nullptr;
	}

	while ((numberOfDeletedGems = field->DeleteAllCombosOnField()) != 0) { //rebild field while there is any combos on it
		UpdateScore(numberOfDeletedGems);
		DrawGame();
		sf::sleep(sf::seconds(0.1f));
		field->DropGemsAfterDeleting(window);
		field->CreateNewGems(window);
	}
}

void GameManager::UpdateScore(int numberOfDeletedGems) {
	score += POINTS_PER_GEM * numberOfDeletedGems;
}

void GameManager::ManageEvents(int& countOfClicks, sf::Vector2i& previouslyChosenGemIndex) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			sf::Vector2i chosenGemIndex;
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			if (IsClickInRange(pos)) {
				countOfClicks++;
				int gemSize = field->GetGemSize();
				if (countOfClicks < 2) {
					chosenGemIndex.x = (pos.x - OFFSET_LEFT) / gemSize;
					chosenGemIndex.y = (pos.y - OFFSET_TOP) / gemSize;

					field->gemsGrid[chosenGemIndex.x][chosenGemIndex.y]->MakePressed(gemSize);
					previouslyChosenGemIndex = chosenGemIndex;
				}
				else if (countOfClicks == 2) {
					chosenGemIndex.x = (pos.x - OFFSET_LEFT) / gemSize;
					chosenGemIndex.y = (pos.y - OFFSET_TOP) / gemSize;
					field->gemsGrid[previouslyChosenGemIndex.x][previouslyChosenGemIndex.y]->MakeUnpressed(gemSize);

					if(field->IsGemsNeighbouring(previouslyChosenGemIndex, chosenGemIndex)) {
						ProcessMove(previouslyChosenGemIndex, chosenGemIndex);
						countOfClicks = 0;
					}
					else if (chosenGemIndex != previouslyChosenGemIndex) {
						field->gemsGrid[chosenGemIndex.x][chosenGemIndex.y]->MakePressed(gemSize);
						previouslyChosenGemIndex = chosenGemIndex;
						countOfClicks = 1;
					}
					else
						countOfClicks = 0;
				}
			}
		}
	}
}

#pragma once
#include "gem.h"

class Field {
	static const int FIELD_GEMS_NUM = 7; //number of gems in one field row
	static const int GEM_SIZE = 100;

	bool IsNewGemMakeCombo(const sf::Vector2i& newGemPosition) const;
	int GemVerticalMatchesNum(const sf::Vector2i& gemPosition, const bool isFieldInProcessOfInitialization = false) const;
	int GemHorisontalMatchesNum(const sf::Vector2i& gemPosition, const bool isFieldInProcessOfInitialization = false) const;
	int FindCombosWithGem(const sf::Vector2i& gemPosition); //marks all gems in combos
	void DeleteAllMarkedGems();

public:

	std::vector<std::vector<std::shared_ptr<Gem>>> gemsGrid{ FIELD_GEMS_NUM, std::vector<std::shared_ptr<Gem>> { FIELD_GEMS_NUM, nullptr } };

	void FillGrid(const int offsetLeft, const int offsetTop, sf::Texture* textures);
	void DisplayField(sf::RenderWindow& window) const;

	bool IsGemsNeighbouring(const sf::Vector2i& firstGem, const sf::Vector2i& secondGem);
	int ProcessSwappingCombos(const sf::Vector2i& firstChosenGem, const sf::Vector2i& secondChosenGem);
	int DeleteAllCombosOnField();
	int DropGemsAfterDeleting(sf::RenderWindow& window);
	void CreateNewGems(sf::RenderWindow& window);

	int GetGemSize() const;
	int GetFieldSize() const;
};

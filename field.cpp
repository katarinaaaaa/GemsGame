#include "field.h"

void Field::FillGrid(const int offsetLeft, const int offsetTop, sf::Texture* textures) {
	int x = offsetLeft;
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < FIELD_GEMS_NUM; ++i) {
		int y = offsetTop;
		for (int j = 0; j < FIELD_GEMS_NUM; ++j) {
			auto gem = std::make_shared<Gem>(Gem(static_cast<float>(x), static_cast<float>(y), GEM_SIZE, textures));
			gemsGrid[i][j] = gem;
			y += GEM_SIZE;
			sf::Vector2i newGemPosition{ i, j };
			while (IsNewGemMakeCombo(newGemPosition))
				gemsGrid[i][j]->ChangeTypeToRandomNew(GEM_SIZE);
		}
		x += GEM_SIZE;
	}
}

bool Field::IsNewGemMakeCombo(const sf::Vector2i& newGemPosition) const {
	return (GemHorisontalMatchesNum(newGemPosition, true) > 2 || GemVerticalMatchesNum(newGemPosition, true) > 2);
}

void Field::DisplayField(sf::RenderWindow& window) const {
	for (int i = 0; i < FIELD_GEMS_NUM; ++i) {
		for (int j = 0; j < FIELD_GEMS_NUM; ++j) {
			if (!gemsGrid[i][j]->IsDeleted())
				window.draw(*gemsGrid[i][j]);
		}
	}
}

int Field::GemVerticalMatchesNum(const sf::Vector2i& gemPosition, const bool isFieldInProcessOfInitialization) const {
	TYPE newGemType = gemsGrid[gemPosition.x][gemPosition.y]->GetType();
	int column = gemPosition.x;
	int row = gemPosition.y;
	int numOfVerticalMatches = 1;
	--row;
	while (row >= 0 && gemsGrid[column][row]->GetType() == newGemType) {
		++numOfVerticalMatches;
		--row;
	}
	if (isFieldInProcessOfInitialization == true)
		return numOfVerticalMatches;

	row = gemPosition.y + 1;
	while (row < FIELD_GEMS_NUM && gemsGrid[column][row]->GetType() == newGemType) {
		++numOfVerticalMatches;
		++row;
	}
	return numOfVerticalMatches;
}

int Field::GemHorisontalMatchesNum(const sf::Vector2i& gemPosition, const bool isFieldInProcessOfInitialization) const {
	TYPE newGemType = gemsGrid[gemPosition.x][gemPosition.y]->GetType();
	int column = gemPosition.x;
	int row = gemPosition.y;
	int numOfHorisontalMatches = 1;
	--column;
	while (column >= 0 && gemsGrid[column][row]->GetType() == newGemType) {
		++numOfHorisontalMatches;
		--column;
	}
	if (isFieldInProcessOfInitialization == true)
		return numOfHorisontalMatches;

	column = gemPosition.x + 1;
	while (column < FIELD_GEMS_NUM && gemsGrid[column][row]->GetType() == newGemType) {
		++numOfHorisontalMatches;
		++column;
	}
	return numOfHorisontalMatches;
}

int Field::FindCombosWithGem(const sf::Vector2i& gemPosition) {
	TYPE gemType = gemsGrid[gemPosition.x][gemPosition.y]->GetType();
	int column = gemPosition.x;
	int row = gemPosition.y;
	int numOfGemsInCombos = 0;

	if (GemHorisontalMatchesNum(gemPosition) > 2) {
		--column;
		while (column >= 0 && gemsGrid[column][row]->GetType() == gemType) {
			gemsGrid[column][row]->isInCombo = true;
			++numOfGemsInCombos;
			--column;
		}
		column = gemPosition.x + 1;
		while (column < FIELD_GEMS_NUM && gemsGrid[column][row]->GetType() == gemType) {
			gemsGrid[column][row]->isInCombo = true;
			++numOfGemsInCombos;
			++column;
		}
	}

	if (GemVerticalMatchesNum(gemPosition) > 2) {
		column = gemPosition.x;
		--row;
		while (row >= 0 && gemsGrid[column][row]->GetType() == gemType) {
			gemsGrid[column][row]->isInCombo = true;
			++numOfGemsInCombos;
			--row;
		}
		row = gemPosition.y + 1;
		while (row < FIELD_GEMS_NUM && gemsGrid[column][row]->GetType() == gemType) {
			gemsGrid[column][row]->isInCombo = true;
			++numOfGemsInCombos;
			++row;
		}
	}

	if (numOfGemsInCombos > 0) {
		gemsGrid[gemPosition.x][gemPosition.y]->isInCombo = true;
		++numOfGemsInCombos;
	}
	return numOfGemsInCombos;
}

bool Field::IsGemsNeighbouring(const sf::Vector2i& firstGem, const sf::Vector2i& secondGem) {
	if (firstGem.x == secondGem.x)
		return (firstGem.y == secondGem.y - 1 || firstGem.y == secondGem.y + 1);
	else if (firstGem.y == secondGem.y)
		return (firstGem.x == secondGem.x - 1 || firstGem.x == secondGem.x + 1);
	return false;
}

int Field::ProcessSwappingCombos(const sf::Vector2i& firstChosenGem, const sf::Vector2i& secondChosenGem) {
	int numberOfDeletedGems = 0;
	TYPE firstGemType = gemsGrid[firstChosenGem.x][firstChosenGem.y]->GetType();
	TYPE secondGemType = gemsGrid[secondChosenGem.x][secondChosenGem.y]->GetType();
	gemsGrid[secondChosenGem.x][secondChosenGem.y]->SetType(firstGemType, GEM_SIZE);
	gemsGrid[firstChosenGem.x][firstChosenGem.y]->SetType(secondGemType, GEM_SIZE);

	numberOfDeletedGems += FindCombosWithGem(firstChosenGem);
	numberOfDeletedGems += FindCombosWithGem(secondChosenGem);
	if (numberOfDeletedGems == 0) {
		gemsGrid[firstChosenGem.x][firstChosenGem.y]->SetType(firstGemType, GEM_SIZE);
		gemsGrid[secondChosenGem.x][secondChosenGem.y]->SetType(secondGemType, GEM_SIZE);
	}
	else
		DeleteAllMarkedGems();
	return numberOfDeletedGems;
}

void Field::DeleteAllMarkedGems() {
	for (int i = 0; i < FIELD_GEMS_NUM; ++i) {
		for (int j = 0; j < FIELD_GEMS_NUM; ++j) {
			if (gemsGrid[i][j]->isInCombo) {
				gemsGrid[i][j]->MakeDeleted();
				gemsGrid[i][j]->isInCombo = false;
			}
		}
	}
}

int Field::DeleteAllCombosOnField() {
	int numberOfDeletedGems = 0;
	for (int i = 0; i < FIELD_GEMS_NUM; ++i) {
		for (int j = 0; j < FIELD_GEMS_NUM; ++j) {
			sf::Vector2i gemPosition{ i, j };
			numberOfDeletedGems += FindCombosWithGem(gemPosition);
		}
	}
	DeleteAllMarkedGems();
	return numberOfDeletedGems;
}

int Field::DropGemsAfterDeleting(sf::RenderWindow& window) {
	int bonusMoving = 0;
	for (int row = FIELD_GEMS_NUM - 1; row >= 0; --row) {
		for (int column = 0; column < FIELD_GEMS_NUM; ++column) {
			if (!gemsGrid[column][row]->IsDeleted())
				continue;

			int gemForDropRow = row - 1;
			while (gemForDropRow >= 0 && gemsGrid[column][gemForDropRow]->IsDeleted())
				gemForDropRow--;
			if (gemForDropRow < 0)
				continue;

			while (gemForDropRow != row) {
				gemsGrid[column][gemForDropRow + 1]->SetType(gemsGrid[column][gemForDropRow]->GetType(), GEM_SIZE);
				if (gemsGrid[column][gemForDropRow + 1]->GetType() == TYPE::REPAINTER || gemsGrid[column][gemForDropRow + 1]->GetType() == TYPE::ERASER)
					bonusMoving++;

				gemsGrid[column][gemForDropRow]->MakeDeleted();
				gemForDropRow++;

				window.clear(sf::Color(219, 251, 255, 0));
				DisplayField(window);
				window.display();
				sf::sleep(sf::seconds(0.1f));
			}
		}
	}
	return bonusMoving;
}

void Field::CreateNewGems(sf::RenderWindow& window) {
	for (int row = FIELD_GEMS_NUM - 1; row >= 0; --row) {
		for (int column = 0; column < FIELD_GEMS_NUM; ++column) {
			if (!gemsGrid[column][row]->IsDeleted())
				continue;

			int droppingGemRow = 0;
			gemsGrid[column][droppingGemRow]->ChangeTypeToRandomNew(GEM_SIZE);

			window.clear(sf::Color(219, 251, 255, 0));
			DisplayField(window);
			window.display();
			sf::sleep(sf::seconds(0.1f));

			TYPE newGemType = gemsGrid[column][droppingGemRow]->GetType();
			while (droppingGemRow < row) {
				gemsGrid[column][droppingGemRow + 1]->SetType(newGemType, GEM_SIZE);
				gemsGrid[column][droppingGemRow]->MakeDeleted();
				droppingGemRow++;

				window.clear(sf::Color(219, 251, 255, 0));
				DisplayField(window);
				window.display();
				sf::sleep(sf::seconds(0.1f));
			}
		}
	}
}

int Field::GetGemSize() const {
	return GEM_SIZE;
}

int Field::GetFieldSize() const {
	return FIELD_GEMS_NUM;
}

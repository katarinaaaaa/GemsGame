#include "gem.h"

Gem::Gem(float x, float y, int size, const sf::Texture* image) {
	setPosition(x, y);
	setSize(sf::Vector2f(static_cast<float>(size), static_cast<float>(size)));
	gemType = static_cast<TYPE>(std::rand() % static_cast<int>(TYPE::GEM_TYPES_NUM));
	setTexture(image);
	setTextureRect(sf::IntRect(size * static_cast<int>(gemType), 0, size, size));
}

void Gem::MakePressed(const int gemSize) {
	setTextureRect(sf::IntRect(gemSize * static_cast<int>(gemType), 250, gemSize, gemSize));
}

void Gem::MakeUnpressed(const int gemSize) {
	setTextureRect(sf::IntRect(gemSize * static_cast<int>(gemType), 0, gemSize, gemSize));
}

void Gem::ChangeTypeToRandomNew(const int gemSize) {
	gemType = static_cast<TYPE>(std::rand() % static_cast<int>(TYPE::GEM_TYPES_NUM));
	setTextureRect(sf::IntRect(gemSize * static_cast<int>(gemType), 0, gemSize, gemSize));
}

void Gem::SetType(TYPE newType, const int gemSize) {
	gemType = newType;
	if (gemType == TYPE::REPAINTER)
		setTextureRect(sf::IntRect(gemSize * 3, 100, gemSize, gemSize));
	else if (gemType == TYPE::ERASER)
		setTextureRect(sf::IntRect(gemSize * 4, 100, gemSize, gemSize));
	else
		setTextureRect(sf::IntRect(gemSize * static_cast<int>(gemType), 0, gemSize, gemSize));
}

void Gem::SetType(std::shared_ptr<Gem> gem, const int gemSize) {
	TYPE newType = gem->gemType;
	SetType(newType, gemSize);
}

TYPE Gem::GetType() const {
	return gemType;
}

bool Gem::IsDeleted() const {
	return (gemType == TYPE::VOID);
}

void Gem::MakeDeleted() {
	gemType = TYPE::VOID;
}
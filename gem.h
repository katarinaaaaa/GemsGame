#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class TYPE {
	AIR,
	WATER,
	FIRE,
	EARTH,
	FOREST,
	GEM_TYPES_NUM,
	VOID,
	REPAINTER,
	ERASER,
};

class Gem : public sf::RectangleShape {
	TYPE gemType;

public:

	bool isInCombo = false; //for searching all combos on field

	Gem(float x, float y, int size, const sf::Texture* image);

	void MakePressed(const int gemSize);
	void MakeUnpressed(const int gemSize);

	void ChangeTypeToRandomNew(const int gemSize);
	void SetType(TYPE newType, const int gemSize);
	void SetType(std::shared_ptr<Gem> gem, const int gemSize);
	TYPE GetType() const;

	bool IsDeleted() const;
	void MakeDeleted();
};

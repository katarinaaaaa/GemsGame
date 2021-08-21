#pragma once
#include "gem.h"
#include "field.h"

class Bonus {
protected:

	static const int AREA_SIZE = 3; //size of area of deleted gem where bonus can appear
	sf::Vector2i containingGemPosition; //gem on which bonus is located

	virtual bool IsPositionAppropriate(const sf::Vector2i& newPosition, int fieldSize) const;
	virtual bool IsEqualTargets(const std::vector<sf::Vector2i>& gemsPositions, int newGemNum) const;

	virtual std::vector<sf::Vector2i> GetTargets(Field* field) const = 0;

public:

	virtual ~Bonus() {}
	virtual void SetPosition(const sf::Vector2i& deletedGem, Field* field);
	virtual void ResetPosition(int dropDepth); //sets new bonus position after gem dropping

	virtual void SetTexture(Field* field) = 0;
	virtual bool Use(Field* field) const = 0; //return true if any gems are deleted and there is need to rebuild the field
};


class Repainter : public Bonus {
	TYPE gemType;

	bool IsAppropriateForRepaint(const std::vector<sf::Vector2i>& gemsPositions, int newGemNum, Field* field) const; //checks if new target is valid
	virtual std::vector<sf::Vector2i> GetTargets(Field* field) const override;

public:
	virtual void SetTexture(Field* field) override;
	virtual bool Use(Field* field) const override;
};


class Eraser : public Bonus {
	virtual std::vector<sf::Vector2i> GetTargets(Field* field) const override;

public:
	virtual void SetTexture(Field* field) override;
	virtual bool Use(Field* field) const override;
};

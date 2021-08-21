#include "bonus.h"

bool Bonus::IsPositionAppropriate(const sf::Vector2i& newPosition, int fieldSize) const {
    return (newPosition.x >= 0 && newPosition.x < fieldSize && newPosition.y >= 0 && newPosition.y < fieldSize);
}

void Bonus::SetPosition(const sf::Vector2i& deletedGem, Field* field) {
    do {
        containingGemPosition.x = deletedGem.x + (std::rand() % (AREA_SIZE + AREA_SIZE) - AREA_SIZE);
        containingGemPosition.y = deletedGem.y + (std::rand() % (AREA_SIZE + AREA_SIZE) - AREA_SIZE);
    } while (!IsPositionAppropriate(containingGemPosition, field->GetFieldSize()) || field->gemsGrid[containingGemPosition.x][containingGemPosition.y]->IsDeleted());
}

void Bonus::ResetPosition(int dropDepth) {
    containingGemPosition.y += dropDepth;
}

bool Bonus::IsEqualTargets(const std::vector<sf::Vector2i>& gemsPositions, int newGemNum) const {
    sf::Vector2i newGem = gemsPositions[newGemNum];
    for (int curGem = 0; curGem < newGemNum; ++curGem) {
        if (gemsPositions[curGem] == newGem)
            return true;
    }
    return false;
}


void Repainter::SetTexture(Field* field) {
    gemType = field->gemsGrid[containingGemPosition.x][containingGemPosition.y]->GetType();
    field->gemsGrid[containingGemPosition.x][containingGemPosition.y]->SetType(TYPE::REPAINTER, field->GetGemSize());
}

bool Repainter::IsAppropriateForRepaint(const std::vector<sf::Vector2i>& gemsPositions, int newGemNum, Field* field) const {
    if (!IsPositionAppropriate(gemsPositions[newGemNum], field->GetFieldSize()))
        return false;
    if (field->gemsGrid[gemsPositions[newGemNum].x][gemsPositions[newGemNum].y]->GetType() == gemType)
        return false;
    if (IsEqualTargets(gemsPositions, newGemNum))
        return false;
    for (int curGem = 0; curGem < newGemNum; ++curGem) {
        if (field->IsGemsNeighbouring(gemsPositions[newGemNum], gemsPositions[curGem]))
            return false;
    }
    return true;
}

std::vector<sf::Vector2i> Repainter::GetTargets(Field* field) const {
    std::vector<sf::Vector2i> gemsPositions (3, sf::Vector2i(-1, -1));
    gemsPositions[0] = containingGemPosition;
    int numOfTries = 0;
    for (int curGem = 1; curGem < 3; ++curGem) {
        do {
            gemsPositions[curGem].x = containingGemPosition.x + (std::rand() % (AREA_SIZE + AREA_SIZE) - AREA_SIZE);
            gemsPositions[curGem].y = containingGemPosition.y + (std::rand() % (AREA_SIZE + AREA_SIZE) - AREA_SIZE);
            ++numOfTries;
            if (numOfTries > 10000) { //to avoid infinite loop
                gemsPositions[curGem].x = -1;
                continue;
            }
        } while (!IsAppropriateForRepaint(gemsPositions, curGem, field));
    }
    return gemsPositions;
}

bool Repainter::Use(Field* field) const {
    std::vector<sf::Vector2i> targets = GetTargets(field);
    for (const auto& curTarget : targets) {
        if (curTarget.x == -1)
            continue;
        field->gemsGrid[curTarget.x][curTarget.y]->SetType(gemType, field->GetGemSize());
    }
    return false;
}


void Eraser::SetTexture(Field* field) {
    field->gemsGrid[containingGemPosition.x][containingGemPosition.y]->SetType(TYPE::ERASER, field->GetGemSize());
}

std::vector<sf::Vector2i> Eraser::GetTargets(Field* field) const {
    std::vector<sf::Vector2i> gemsPositions(5, sf::Vector2i(-1, -1));
    gemsPositions[0] = containingGemPosition;
    int numOfTries = 0;
    int fieldSize = field->GetFieldSize();
    for (int curGem = 1; curGem < 5; ++curGem) {
        do {
            gemsPositions[curGem].x = rand() % fieldSize;
            gemsPositions[curGem].y = rand() % fieldSize;
            if (numOfTries > 10000) {
                gemsPositions[curGem].x = -1;
                continue;
            }
        } while (IsEqualTargets(gemsPositions, curGem));
    }
    return gemsPositions;
}

bool Eraser::Use(Field* field) const {
    std::vector<sf::Vector2i> targets = GetTargets(field);
    for (const auto& curTarget : targets) {
        if (curTarget.x == -1)
            continue;
        field->gemsGrid[curTarget.x][curTarget.y]->MakeDeleted();
    }
    return true;
}

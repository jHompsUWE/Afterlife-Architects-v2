#pragma once
#include "StructureSprite.h"

#include "StructureData.h"

class StructureKarmaAnchor : public StructureSprite
{
public:
	StructureKarmaAnchor(StructureData* structure_data);
	~StructureKarmaAnchor();

	void TickStructure(GameData* game_data) override;

protected:

private:
};


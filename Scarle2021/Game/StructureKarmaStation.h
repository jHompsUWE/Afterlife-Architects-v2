#pragma once
#include "StructureSprite.h"

class StructureKarmaStation : public StructureSprite
{
public:
	StructureKarmaStation(StructureData* structure_data);
	~StructureKarmaStation();

	void TickStructure(GameData* game_data) override;

protected:

private:
};


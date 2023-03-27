#pragma once
#include <vector>
#include <memory>

#include "VBQuad.h"
#include "PlaneType.h"
#include "StructureData.h"

class StructureSprite : public VBQuad
{
public:
	StructureSprite(StructureData* structure_data);
	~StructureSprite();

	virtual void TickStructure(GameData* game_data) {}

	std::vector<Vector3> GetOccupiedTiles() { return occupied_tiles; }
	int GetTileSize() { return tile_size; }

protected:
	void AddOccupiedTiles(Vector3 tile_pos, int tile_size);
	PlaneType plane;

private:
	const float PI = 3.141592653589793238462643383279502884197169399375105820974944;
	std::vector<Vector3> occupied_tiles; //Tiles occupied by this structure
	int tile_size;
};


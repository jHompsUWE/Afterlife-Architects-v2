#pragma once
#include "Tile.h"
class RaDTile : public Tile
{
public:
	RaDTile(ID3D11Device* GD, ID3D11ShaderResourceView* texture, Vector3 _tile_pos, ZoneType zone_type);
	~RaDTile();

	void ChangeRaD(int RaD_diff);

protected:
	int RaD_value = 0;
	float colour_scaling = 10.0f;

private:
};


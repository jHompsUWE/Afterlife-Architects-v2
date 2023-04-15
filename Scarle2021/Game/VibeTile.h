#pragma once
#include "Tile.h"

class VibeTile : public Tile
{
public:
	VibeTile(ID3D11Device* GD, ID3D11ShaderResourceView* texture, Vector3 _tile_pos, ZoneType zone_type);
	~VibeTile();

	void ChangeVibe(int vibe_diff);
	int GetVibe();

protected:
	int vibe_value = 0;
	float colour_scaling = 10.0f;

private:
};


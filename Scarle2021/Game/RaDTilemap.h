#pragma once
#include <vector>
#include <cmath>

#include "TextureManager.h"
#include "RaDTile.h"

class RaDTilemap
{
public:
	RaDTilemap(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, int _size, Vector3 _start);
	~RaDTilemap();

	void Draw(DrawData* _DD);

	void RaDChange(Vector3 tile_pos, int RaD_diff, int tile_size, int range);

protected:

	std::vector<std::vector<std::unique_ptr<RaDTile>>> RaD_tilemap;
	float size;
	Vector3 start;

private:
};



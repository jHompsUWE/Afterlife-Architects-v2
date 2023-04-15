#pragma once
#include <vector>
#include <cmath>
#include <iostream>

#include "VBQuad.h"

#include "TextureManager.h"

class RaDTilemap
{
public:
	RaDTilemap(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, int _size, Vector3 _start);
	~RaDTilemap();

	void RaDChange(Vector3 tile_pos, int RaD_diff, int tile_size);
	int GetRaD(Vector3 tile_pos);

protected:

	std::vector<std::vector<int>> rad_tilemap;
	float size;
	Vector3 start;

private:
};



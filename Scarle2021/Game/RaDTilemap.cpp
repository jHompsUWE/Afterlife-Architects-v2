#include "pch.h"
#include "RaDTilemap.h"

RaDTilemap::RaDTilemap(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, int _size, Vector3 _start) :
	size(_size), start(_start)
{
	for (int x = 0; x < size; x++)
	{
		rad_tilemap.emplace_back();
		for (int y = 0; y < size; y++)
		{
			rad_tilemap[x].emplace_back(50);
		}
	}
}

RaDTilemap::~RaDTilemap()
{
}

/// <summary>
/// Change tilemap's research and development system at certain tile position at a certain scale and value
/// </summary>
/// <param name="tile_pos">The starting position/structure origin</param>
/// <param name="vibe_diff">The delta of the Research and Development</param>
/// <param name="tile_size">The size of the area to be changed</param>
/// <param name="range">The range around the area to be changed</param>
void RaDTilemap::RaDChange(Vector3 tile_pos, int RaD_diff, int tile_size)
{
	// Change square of area contained
	for (int x = 0; x < tile_size; x++)
	{
		for (int z = 0; z < tile_size; z++)
		{
			rad_tilemap[tile_pos.x + x][tile_pos.z + z] = RaD_diff;
		}
	}
}

int RaDTilemap::GetRaD(Vector3 tile_pos)
{
	return rad_tilemap[tile_pos.x][tile_pos.z];
}

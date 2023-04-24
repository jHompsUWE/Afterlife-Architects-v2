#include "pch.h"
#include "VibeTilemap.h"

VibeTilemap::VibeTilemap(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, int _size, Vector3 _start):
	size(_size), start(_start)
{
	for (int x = 0; x < size; x++)
	{
		vibe_tilemap.emplace_back();
		for (int y = 0; y < size; y++)
		{
			vibe_tilemap[x].emplace_back(std::make_unique<VibeTile>(GD, _texture_manager->GetTextureZone(Vibe), start + Vector3(x, 0, y), Vibe));
			vibe_tilemap[x][y]->UpdateWorldMatrix();
		}
	}
}

VibeTilemap::~VibeTilemap()
{
}

void VibeTilemap::Draw(DrawData* _DD)
{
	for (auto& x : vibe_tilemap)
	{
		for (const auto& y : x)
		{
			if (y->GetVibe() != 0)
			{
				y->Draw(_DD);
			}
		}
	}
}

/// <summary>
/// Change tilemap's vibes system at certain tile position at a certain scale and value
/// </summary>
/// <param name="tile_pos">The starting position/structure origin</param>
/// <param name="vibe_diff">The delta of the vibe</param>
/// <param name="tile_size">The size of the area to be changed</param>
/// <param name="range">The range around the area to be changed</param>
void VibeTilemap::VibeChange(Vector3 tile_pos, int vibe_diff, int tile_size, int range)
{
	std::vector<Vector3> added_positions;

	// Change square of area contained
	for (int x = 0; x < tile_size; x++)
	{
		for (int z = 0; z < tile_size; z++)
		{
			added_positions.emplace_back(Vector3(tile_pos.x + x,0 , tile_pos.z + z));
		}
	}

	// Collect a vector of positions to later check for vibe change
	// Box to the left, right, above and below
	for (int i = 0; i < tile_size; i++)
	{
		for (int r = 0; r < range; r++)
		{
			added_positions.emplace_back(Vector3(tile_pos.x + i, 0, tile_pos.z - r - 1));
			added_positions.emplace_back(Vector3(tile_pos.x + i, 0, tile_pos.z + r + tile_size));
			added_positions.emplace_back(Vector3(tile_pos.x - r - 1, 0, tile_pos.z + i));
			added_positions.emplace_back(Vector3(tile_pos.x + r + tile_size, 0, tile_pos.z + i));
		}
	}
	if (range > 1)
	{
		// Expand for all four corners 
		std::vector<Vector3> corner_positions;
		std::vector<Vector3> corner_expand;
		corner_positions.emplace_back(Vector3(tile_pos.x + tile_size, 0, tile_pos.z + tile_size));
		corner_positions.emplace_back(Vector3(tile_pos.x - 1, 0, tile_pos.z - 1));
		corner_positions.emplace_back(Vector3(tile_pos.x - 1, 0, tile_pos.z + tile_size));
		corner_positions.emplace_back(Vector3(tile_pos.x + tile_size, 0, tile_pos.z - 1));
		for (int i = 0; i < 4; i++)
		{
			corner_expand = CornerExpand(corner_positions[i], range, VibeDirection(i));
			for (int i = 0; i < corner_expand.size(); i++)
			{
				added_positions.emplace_back(corner_expand[i]);
			}
		}
		corner_expand.clear();
	}

	// Depending on an odd or even sized structure, change the structure pivot
	Vector2 offset = Vector2(0,0);
	int range_boost = 0;
	if (tile_size % 2 == 0)
	{
		offset = Vector2(-0.5f, -0.5f);
	}
	else
	{
		range_boost = 1;
	}
	// For each added position, check if it is a valid tile
	// Get the distance of it from the centre of the structure
	// Change vibe value based on the distance
	float pos_dist;
	for (int i = 0; i < added_positions.size(); i++)
	{
		if (ValidTile(added_positions[i]))
		{
			pos_dist = abs(tile_pos.x - added_positions[i].x + (tile_size / 2)+ offset.x)  + abs(tile_pos.z - added_positions[i].z + (tile_size / 2) + offset.y);
			vibe_tilemap[added_positions[i].x][added_positions[i].z]->ChangeVibe(vibe_diff * (1 + range_boost + range - pos_dist + tile_size / 2) / range);
		}
	}
	added_positions.clear();
}

/// <summary>
/// Get vibe of specified tile
/// </summary>
/// <param name="tile_pos">Specified tile to get vibe value</param>
/// <returns>Int vibe value of tile</returns>
int VibeTilemap::GetVibe(Vector3 tile_pos)
{
	return vibe_tilemap[tile_pos.x - start.x][tile_pos.z - start.z]->GetVibe();
}

/// <summary>
/// Get vector of tile positions expanding a certain range and direction
/// </summary>
/// <param name="point">Position of tile</param>
/// <param name="range">Number of tiles to expand out to</param>
/// <param name="direction">Section to expand to</param>
/// <returns>Vector of Vector3 tile positions</returns>
std::vector<Vector3> VibeTilemap::CornerExpand(Vector3 point, int range, VibeDirection direction)
{
	std::vector<Vector3> temp_list;
	int min_x = -range + 1;
	int max_x = range - 1;
	int min_z = -range + 1;
	int max_z = range - 1;

	// Alter min and max pos based on direction based enum
	switch (direction)
	{
	case Vibe_Right:
		max_x = 0;
		min_z = 0;
		break;
	case Vibe_Left:
		min_x = 0;
		max_z = 0;
		break;
	case Vibe_Up:
		min_x = 0;
		min_z = 0;
		break;
	case Vibe_Down:
		max_x = 0;
		max_z = 0;
		break;
	default:
		std::cout << "Vibe error" << std::endl;
		break;
	}
	// Iteration through box of min and max x,y values
	Vector3 temp_vec;
	for (int x = min_x; x <= max_x; x++)
	{
		for (int z = min_z; z <= max_z; z++)
		{
			// If within range then add to vector of positions
			if (abs(x) + abs(z) < range - 1)
			{ 
				temp_vec = Vector3(point.x + x, 0, point.z + z);
				temp_list.emplace_back(temp_vec);
			}
		}
	}

	return temp_list;
}

/// <summary>
/// Checks if tile position is on the map
/// </summary>
/// <param name="tile_pos">Tile position being checked</param>
/// <returns>Boolean of if the tile is valid</returns>
bool VibeTilemap::ValidTile(Vector3 tile_pos)
{
	if (tile_pos.x >= 100) { return false; }
	if (tile_pos.x < 0) { return false; }
	if (tile_pos.z >= 100) { return false; }
	if (tile_pos.z < 0) { return false; }
	return true;
}

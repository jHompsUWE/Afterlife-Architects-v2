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
		for (auto& y : x)
		{
			y->Draw(_DD);
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
	// Change square of area contained
	for (int x = 0; x < tile_size; x++)
	{
		for (int z = 0; z < tile_size; z++)
		{
			vibe_tilemap[tile_pos.x + x][tile_pos.z + z]->ChangeVibe(vibe_diff);
		}
	}

	// Collect a vector of positions to later check for vibe change
	std::vector<Vector3> added_positions;

	Vector3 temp_vec;
	// Box to the left, right, above and below
	for (int i = 0; i < tile_size; i++)
	{
		for (int r = 0; r < range; r++)
		{
			temp_vec = Vector3(tile_pos.x + i, 0, tile_pos.z - r - 1);
			added_positions.emplace_back(temp_vec);
			temp_vec = Vector3(tile_pos.x + i, 0, tile_pos.z + r + tile_size);
			added_positions.emplace_back(temp_vec);
			temp_vec = Vector3(tile_pos.x - r - 1, 0, tile_pos.z + i);
			added_positions.emplace_back(temp_vec);
			temp_vec = Vector3(tile_pos.x + r + tile_size, 0, tile_pos.z + i);
			added_positions.emplace_back(temp_vec);
		}
	}

	if (range > 1)
	{
		// Expand for all four corners 
		Vector3 corner_pos = Vector3(tile_pos.x-1,0,tile_pos.z-1);
		std::vector<Vector3> corner_expand = CornerExpand(corner_pos, range, Vibe_Down);
		for (int i = 0; i < corner_expand.size(); i++)
		{
			added_positions.emplace_back(corner_expand[i]);
		}

		corner_pos = Vector3(tile_pos.x + tile_size, 0, tile_pos.z + tile_size);
		corner_expand = CornerExpand(corner_pos, range, Vibe_Up);
		for (int i = 0; i < corner_expand.size(); i++)
		{
			added_positions.emplace_back(corner_expand[i]);
		}

		corner_pos = Vector3(tile_pos.x + tile_size, 0, tile_pos.z - 1);
		corner_expand = CornerExpand(corner_pos, range, Vibe_Left);
		for (int i = 0; i < corner_expand.size(); i++)
		{
			added_positions.emplace_back(corner_expand[i]);
		}

		corner_pos = Vector3(tile_pos.x - 1, 0, tile_pos.z + tile_size);
		corner_expand = CornerExpand(corner_pos, range, Vibe_Right);
		for (int i = 0; i < corner_expand.size(); i++)
		{
			added_positions.emplace_back(corner_expand[i]);
		}
		corner_expand.clear();
	}

	if (tile_size % 2 == 0)
	{
		float pos_dist;
		for (int i = 0; i < added_positions.size(); i++)
		{
			if (ValidTile(added_positions[i]))
			{
				pos_dist = abs(tile_pos.x - added_positions[i].x - 0.5f + (tile_size / 2)) + abs(tile_pos.z - added_positions[i].z - 0.5f + (tile_size / 2));
				vibe_tilemap[added_positions[i].x][added_positions[i].z]->ChangeVibe(vibe_diff * ( 1 + range - pos_dist + tile_size / 2) / range);
			}
		}
	}
	else
	{
		int pos_dist;
		for (int i = 0; i < added_positions.size(); i++)
		{
			if (ValidTile(added_positions[i]))
			{
				pos_dist = abs(tile_pos.x - added_positions[i].x + (tile_size / 2)) + abs(tile_pos.z - added_positions[i].z + (tile_size / 2));
				vibe_tilemap[added_positions[i].x][added_positions[i].z]->ChangeVibe(vibe_diff * ( 2 + range - pos_dist + tile_size / 2) / range);
			}
		}
	}
	added_positions.clear();
}

std::vector<Vector3> VibeTilemap::CornerExpand(Vector3 point, int range, VibeDirection direction)
{
	std::vector<Vector3> temp_list;
	int min_x = -range + 1;
	int max_x = range - 1;
	int min_z = -range + 1;
	int max_z = range - 1;

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
	Vector3 temp_vec;
	for (int x = min_x; x <= max_x; x++)
	{
		for (int z = min_z; z <= max_z; z++)
		{
			if (abs(x) + abs(z) < range - 1)
			{ 
				temp_vec = Vector3(point.x + x, 0, point.z + z);
				temp_list.emplace_back(temp_vec);
			}
		}
	}

	return temp_list;
}

bool VibeTilemap::ValidTile(Vector3 tile_pos)
{
	if (tile_pos.x >= 100) { return false; }
	if (tile_pos.x < 0) { return false; }
	if (tile_pos.z >= 100) { return false; }
	if (tile_pos.z < 0) { return false; }
	return true;
}

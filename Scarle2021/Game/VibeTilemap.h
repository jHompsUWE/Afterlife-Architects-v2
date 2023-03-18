#pragma once
#include <vector>
#include <cmath>

#include "TextureManager.h"
#include "VibeTile.h"

enum VibeDirection
{
	Vibe_Up,
	Vibe_Down,
	Vibe_Right,
	Vibe_Left
};

class VibeTilemap
{
public:
	VibeTilemap(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, int _size, Vector3 _start);
	~VibeTilemap();

	void Draw(DrawData* _DD);

	void VibeChange(Vector3 tile_pos, int vibe_diff, int tile_size, int range);

protected:
	bool ValidTile(Vector3 tile_pos);
	std::vector<Vector3> CornerExpand(Vector3 point, int range, VibeDirection direction);

	std::vector<std::vector<std::unique_ptr<VibeTile>>> vibe_tilemap;
	float size;
	Vector3 start;

private:
};


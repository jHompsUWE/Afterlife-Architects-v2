#include "pch.h"
#include "RaDTile.h"

RaDTile::RaDTile(ID3D11Device* GD, ID3D11ShaderResourceView* texture, Vector3 _tile_pos, ZoneType zone_type) :
	Tile(GD, _tile_pos, zone_type)
{
	quad->SetTexture(texture);

	is_occupied = true;
}

RaDTile::~RaDTile()
{
}

/// <summary>
/// Changes individual tile colour and vibe value
/// </summary>
/// <param name="vibe_diff">The delta of the vibe</param>
void RaDTile::ChangeRaD(int RaD_diff)
{
	RaD_value += RaD_diff;
	// Create temporary int and colour to produce required colour
	Color new_color;
	int temp_int = RaD_value;
	// Make vibe value absolute
	if (RaD_value < 0)
	{
		temp_int = -temp_int;
	}
	// Scale colour down to 0,0,0
	new_color.x = (255.0f - (temp_int * colour_scaling)) / 255.0f;
	new_color.y = (255.0f - (temp_int * colour_scaling)) / 255.0f;
	new_color.z = (255.0f - (temp_int * colour_scaling)) / 255.0f;

	// If positive vibe, lean towards green
	if (RaD_value > 0)
	{
		new_color.y = 1;
	}
	// If negative vibe, lean towards red
	else if (RaD_value < 0)
	{
		new_color.x = 1;
	}
	quad->SetColor(new_color);
}



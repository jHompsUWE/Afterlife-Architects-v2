#include "pch.h"
#include "VibeTile.h"

VibeTile::VibeTile(ID3D11Device* GD, ID3D11ShaderResourceView* texture, Vector3 _tile_pos, ZoneType zone_type):
	Tile(GD, _tile_pos, zone_type)
{
	quad->SetTexture(texture);
	quad->SetColor(Color(0, 0, 0));

	is_occupied = true;
}

VibeTile::~VibeTile()
{
}

/// <summary>
/// Changes individual tile colour and vibe value
/// </summary>
/// <param name="vibe_diff">The delta of the vibe</param>
void VibeTile::ChangeVibe(int vibe_diff)
{
	vibe_value += vibe_diff;
	// Create temporary int and colour to produce required colour
	Color new_color = Color(0,0,0);
	// Make vibe value absolute
	int temp_int = vibe_value < 0 ? -vibe_value : vibe_value;

	// If positive vibe, lean towards green
	new_color.x = vibe_value > 0 ? 0 : (temp_int * colour_scaling) / 255.0f;
	// If negative vibe, lean towards red
	new_color.y = vibe_value < 0 ? 0 : (temp_int * colour_scaling) / 255.0f;
	quad->SetColor(new_color);
}

int VibeTile::GetVibe()
{
	if (vibe_value > 16)
	{
		return 16;
	}
	return vibe_value;
}


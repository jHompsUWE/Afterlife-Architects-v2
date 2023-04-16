#include "pch.h"
#include "StructureData.h"

StructureData::StructureData(ID3D11Device* _GD, Vector2 _dimensions, Vector3 _tile_pos, int _tile_size, ID3D11ShaderResourceView* _texture, PlaneType _plane):
	GD(_GD), dimensions(_dimensions), tile_pos(_tile_pos), tile_size(_tile_size), texture(_texture), plane(_plane)
{

}

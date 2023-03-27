#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "PlaneType.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct StructureData
{
public:
	StructureData(ID3D11Device* _GD, Vector2 _dimensions, Vector3 _tile_pos, int _tile_size, ID3D11ShaderResourceView* _texture, PlaneType _plane);
	~StructureData() {}

	ID3D11Device* GD;
	Vector2 dimensions;
	Vector3 tile_pos;
	int tile_size;
	ID3D11ShaderResourceView* texture;
	PlaneType plane;

protected:

private:

};


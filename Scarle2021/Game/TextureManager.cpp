#include "pch.h"
#include "TextureManager.h"

TextureManager::TextureManager(ID3D11Device* GD)
{
	// Textures are stored in the same order as the ZoneType enum

#ifdef _ARCADE
	AddTextureZone(GD, "Tile_Void_Compressed");

	AddTextureZone(GD, "Tile_Green_Compressed");
	AddTextureZone(GD, "Tile_Yellow_Compressed");
	AddTextureZone(GD, "Tile_Orange_Compressed");
	AddTextureZone(GD, "Tile_Brown_Compressed");
	AddTextureZone(GD, "Tile_Purple_Compressed");
	AddTextureZone(GD, "Tile_Red_Compressed");
	AddTextureZone(GD, "Tile_Blue_Compressed");

	AddTextureZone(GD, "Tile_Inactive_Green_Compressed");
	AddTextureZone(GD, "Tile_Inactive_Yellow_Compressed");
	AddTextureZone(GD, "Tile_Inactive_Orange_Compressed");
	AddTextureZone(GD, "Tile_Inactive_Brown_Compressed");
	AddTextureZone(GD, "Tile_Inactive_Purple_Compressed");
	AddTextureZone(GD, "Tile_Inactive_Red_Compressed");
	AddTextureZone(GD, "Tile_Inactive_Blue_Compressed");

	AddTextureZone(GD, "Tile_Structure_Compressed");
	AddTextureZone(GD, "Tile_Road_Compressed");
	AddTextureZone(GD, "Tile_Water_Compressed");
	AddTextureZone(GD, "Tile_Lava_Compressed");

	AddTextureZone(GD, "Tile_Karma_Track_Compressed");
	AddTextureZone(GD, "Tile_Karma_Portal_Compressed");

	AddTextureZone(GD, "Tile_Vibes_Compressed");
	AddTextureZone(GD, "Tile_Rock_Compressed");
#elif 
	AddTextureZone(GD, "Tile_Void");

	AddTextureZone(GD, "Tile_Green");
	AddTextureZone(GD, "Tile_Yellow");
	AddTextureZone(GD, "Tile_Orange");
	AddTextureZone(GD, "Tile_Brown");
	AddTextureZone(GD, "Tile_Purple");
	AddTextureZone(GD, "Tile_Red");
	AddTextureZone(GD, "Tile_Blue");

	AddTextureZone(GD, "Tile_Inactive_Green");
	AddTextureZone(GD, "Tile_Inactive_Yellow");
	AddTextureZone(GD, "Tile_Inactive_Orange");
	AddTextureZone(GD, "Tile_Inactive_Brown");
	AddTextureZone(GD, "Tile_Inactive_Purple");
	AddTextureZone(GD, "Tile_Inactive_Red");
	AddTextureZone(GD, "Tile_Inactive_Blue");

	AddTextureZone(GD, "Tile_Structure");
	AddTextureZone(GD, "Tile_Road");
	AddTextureZone(GD, "Tile_Water");
	AddTextureZone(GD, "Tile_Lava");

	AddTextureZone(GD, "Tile_Karma_Track");
	AddTextureZone(GD, "Tile_Karma_Portal");

	AddTextureZone(GD, "Tile_Vibes");
	AddTextureZone(GD, "Tile_Rock");
#endif

	// Textures are stored in the same order as the StructureType enum for both Heaven and Hell
	initializeStructureTextures(GD);
}

TextureManager::~TextureManager()
{
}

/// <summary>
/// Get the texture of the given ZoneType
/// </summary>
/// <param name="zone">ZoneType of the zone</param>
/// <returns>ID3D11ShaderResourceView* pointer to the texture</returns>
ID3D11ShaderResourceView* TextureManager::GetTextureZone(ZoneType zone)
{
	return texture_zone[int(zone)];
}

/// <summary>
/// Get the texture of the given StructureType and Plane
/// </summary>
/// <param name="structure">StructureType of the structure</param>
/// <param name="plane">Heaven/Hell</param>
/// <returns>ID3D11ShaderResourceView* pointer to the texture</returns>
ID3D11ShaderResourceView* TextureManager::GetTextureStructure(StructureType structure, PlaneType plane)
{
	switch (plane)
	{
	case Heaven:
		return texture_heaven_structure[int(structure)];
		break;

	case Hell:
		return texture_hell_structure[int(structure)];
		break;
	}
}

// Referenced from https://gamedev.net/forums/topic/650787-how-to-get-texture-width-and-size/5114247/ by Joyal, 04/12/2013
// Referenced from https://stackoverflow.com/questions/25493929/down-casting-id3d11texture2d
/// <summary>
/// Get the dimensions of a structure texture of the given StructureType and Plane
/// </summary>
/// <param name="structure">StructureType of the structure</param>
/// <param name="plane">Heaven/Hell</param>
/// <returns>Vector2 (width, height) of the texture dimensions</returns>
DirectX::SimpleMath::Vector2 TextureManager::GetSizeStructure(StructureType structure, PlaneType plane)
{
	int size_x = 0;
	int size_y = 0;

	// Get a ptr to resource
	ID3D11Resource* resource = nullptr;

	switch (plane)
	{
	case Heaven:
		texture_heaven_structure[int(structure)]->GetResource(&resource);
		break;

	case Hell:
		texture_hell_structure[int(structure)]->GetResource(&resource);
		break;
	}

	// Query the resource to the get the texture thingy
	ID3D11Texture2D* texture2D = nullptr;
	if (SUCCEEDED(resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture2D)))
	{
		// Get the texture description
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);

		// Use the texture description for what u need here coz it needs to be released
		size_x = desc.Width;
		size_y = desc.Height;

		// Release the texture
		texture2D->Release();
	}

	// Release the resource
	resource->Release();

	return DirectX::SimpleMath::Vector2(size_x, size_y);
}

void TextureManager::initializeStructureTextures(ID3D11Device* GD)
{
	for (auto& structure_type : GameplaySingletons::GetStructureValues()["structureValues"])
	{
		HRESULT hr;
		texture_heaven_structure.emplace_back();
		hr = DirectX::CreateDDSTextureFromFile(GD, Helper::charToWChar(("../Assets/" + structure_type.value("textureHeaven", "unknown") + ".dds").c_str()), nullptr,
			&texture_heaven_structure.back());
		assert(hr == S_OK);

		texture_hell_structure.emplace_back();
		hr = DirectX::CreateDDSTextureFromFile(GD, Helper::charToWChar(("../Assets/" + structure_type.value("textureHell", "unknown") + ".dds").c_str()), nullptr,
			&texture_hell_structure.back());
		assert(hr == S_OK);
	}
}

void TextureManager::AddTextureZone(ID3D11Device* GD, std::string file_name)
{
	texture_zone.emplace_back();
	HRESULT hr = DirectX::CreateDDSTextureFromFile(GD, Helper::charToWChar(("../Assets/" + file_name + ".dds").c_str()), nullptr, &texture_zone.back());
	assert(hr == S_OK);
}

void TextureManager::AddTextureStructure(ID3D11Device* GD, std::string file_name, PlaneType plane)
{
	HRESULT hr;
	switch (plane)
	{
	case Heaven:
		texture_heaven_structure.emplace_back();
		hr = DirectX::CreateDDSTextureFromFile(GD, Helper::charToWChar(("../Assets/" + file_name + ".dds").c_str()), nullptr, 
			&texture_heaven_structure.back());
		assert(hr == S_OK);
		break;

	case Hell:
		texture_hell_structure.emplace_back();
		hr = DirectX::CreateDDSTextureFromFile(GD, Helper::charToWChar(("../Assets/" + file_name + ".dds").c_str()), nullptr,
			&texture_hell_structure.back());
		assert(hr == S_OK);
		break;
	}
}

#pragma once
#include <memory>
#include <vector>

#include "ZoneType.h"
#include "PlaneType.h"
#include "StructureType.h"

#include "GameplaySingletons.h"
#include "PopulationManager.h"
#include "TextureManager.h"
#include "EconomyManager.h"
#include "NewEventManager.h"

#include "VibeTilemap.h"
#include "RaDTilemap.h"

#include "StructureGate.h"
#include "StructureTopia.h"
#include "StructureTrainingCenter.h"
#include "StructureBuilding.h"
#include "StructureKarmaAnchor.h"
#include "StructureKarmaStation.h"

#include "StructureData.h"

class BuildingManager
{
public:
	BuildingManager(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, std::shared_ptr<PopulationManager> _population_manager, std::unique_ptr<VibeTilemap>& _vibe_tilemap, std::unique_ptr<RaDTilemap>& _rad_tilemap,
		std::shared_ptr<EconomyManager> _economy_manager, int _size, Vector3 _start, PlaneType _plane);
	~BuildingManager();

	void Tick(GameData* game_data);
	void Draw(DrawData* _DD);

	void CreateStructure(StructureType structure_type, Vector3 tile_position);
	void DestroyStructure(Vector3 tile_position);
	std::vector<Vector3> GetStructureOccupiedTiles(Vector3 tile_position);

	static int GetSizeOfStructure(StructureType structure_type);
	static int GetCostOfStructure(StructureType structure_type);
	static int GetVibeOfStructure(StructureType structure_type);

protected:

private:
	ID3D11Device* d11_device = nullptr;
	Vector3 start;
	std::vector<std::vector<std::unique_ptr<StructureSprite>>> structure_map;

	std::unique_ptr<VibeTilemap>& vibe_tilemap;
	std::unique_ptr<RaDTilemap>& rad_tilemap;

	std::shared_ptr<PopulationManager> population_manager;
	std::shared_ptr<EconomyManager> economy_manager;
	std::shared_ptr<TextureManager> texture_manager;

	PlaneType plane;
	const char destroy_sound_1[32] = "Demolish1";
	const char destroy_sound_2[32] = "Demolish2";
	const char destroy_sound_3[32] = "Demolish3";
	const char bank_sound[32] = "Bank";
	const char false_click_sound[32] = "Demolish1";
	const char gate_heaven_sound[32] = "GateHeaven";
	const char gate_hell_sound[32] = "GateHell";
	const char institute_heaven_sound[32] = "InstituteHeaven";
	const char institute_hell_sound[32] = "InstituteHell";
	const char limbo_sound[32] = "Limbos";
	const char port_sound[32] = "Port";
	const char special_build_heaven_sound[32] = "SpecialBuildingHeaven";
	const char special_build_hell_sound[32] = "SpecialBuildingHell";
	const char station_sound[32] = "Station";
	const char topia_heaven_sound[32] = "TopiaHeaven";
	const char topia_hell_sound[32] = "TopiaHell";
};


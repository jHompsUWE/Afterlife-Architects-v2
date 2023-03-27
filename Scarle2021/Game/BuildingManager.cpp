#include "pch.h"
#include "BuildingManager.h"
#include <iostream>

BuildingManager::BuildingManager(ID3D11Device* GD, std::shared_ptr<TextureManager> _texture_manager, std::shared_ptr<PopulationManager> _population_manager,
	std::shared_ptr<EconomyManager> _economy_manager, int _size, Vector3 _start, PlaneType _plane) :
	d11_device(GD), start(_start), plane(_plane), texture_manager(_texture_manager), population_manager(_population_manager), economy_manager(_economy_manager)
{
	for (int x = 0; x < _size; x++)
	{
		structure_map.emplace_back();
		for (int y = 0; y < _size; y++)
		{
			structure_map[x].emplace_back(nullptr);
		}
	}
}

BuildingManager::~BuildingManager()
{
}

void BuildingManager::Tick(GameData* game_data)
{
	for (auto& x : structure_map)
	{
		for (auto& y : x)
		{
			if (y)
			{
				y->TickStructure(game_data);
			}
		}
	}
}

void BuildingManager::Draw(DrawData* _DD)
{
	for (auto& x : structure_map)
	{
		for (auto& y : x)
		{
			if (y)
			{
				y->Draw(_DD);
			}
		}
	}
}

/// <summary>
/// Creates a structure of the given type at the given tile position
/// </summary>
/// <param name="structure_type">The type of the structure</param>
/// <param name="tile_position">The position of the strucutre</param>
void BuildingManager::CreateStructure(StructureType structure_type, Vector3 tile_position)
{
	int size = GetSizeOfStructure(structure_type);
	Vector2 dimensions = texture_manager->GetSizeStructure(structure_type, plane);

	StructureData* structure_data = new StructureData(d11_device, Vector2(sqrt(2) * size, sqrt(2) * dimensions.y / dimensions.x * size), 
		tile_position + start, size, texture_manager->GetTextureStructure(structure_type, plane), plane);

	switch (structure_type)
	{
	case Building_Green_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Green,
				100); // The capacity of souls that this building provides
		break;

	case Building_Green_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Green,
				200); // The capacity of souls that this building provides
		break;

	case Building_Yellow_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Yellow,
				100); // The capacity of souls that this building provides
		break;

	case Building_Yellow_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Yellow,
				200); // The capacity of souls that this building provides
		break;

	case Building_Orange_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Orange,
				100); // The capacity of souls that this building provides
		break;

	case Building_Orange_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Orange,
				200); // The capacity of souls that this building provides
		break;

	case Building_Brown_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Brown,
				100); // The capacity of souls that this building provides
		break;

	case Building_Brown_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Brown,
				200); // The capacity of souls that this building provides
		break;

	case Building_Purple_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Purple,
				100); // The capacity of souls that this building provides
		break;

	case Building_Purple_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Purple,
				200); // The capacity of souls that this building provides
		break;

	case Building_Red_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Red,
				100); // The capacity of souls that this building provides
		break;

	case Building_Red_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Red,
				200); // The capacity of souls that this building provides
		break;

	case Building_Blue_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Blue,
				100); // The capacity of souls that this building provides
		break;

	case Building_Blue_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureBuilding>(structure_data, Blue,
				200); // The capacity of souls that this building provides
		break;

	case Bank_T1:
		// TODO: Give actual functionality to this building
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureSprite>(structure_data);
		AL::NewEventManager::GenerateEventSt(AL::event_sound_start, bank_sound, 1.0f, true);
		break;

	case Bank_T2:
		// TODO: Give actual functionality to this building
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureSprite>(structure_data);
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_hell_sound, 1.0f, true);
		}
		break;

	case Gate_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureGate>(structure_data,
				10); // The number of souls this gate generates per year
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, gate_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, gate_hell_sound, 1.0f, true);
		}
		break;

	case Gate_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureGate>(structure_data,
				50); // The number of souls this gate generates per year
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, gate_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, gate_hell_sound, 1.0f, true);
		}
		break;

	case Gate_T3:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureGate>(structure_data,
				100); // The number of souls this gate generates per year
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, gate_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, gate_hell_sound, 1.0f, true);
		}
		break;

	case KarmaAnchor:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureKarmaAnchor>(structure_data);
		break;

	case KarmaPortal:
		// TODO: Give actual functionality to this building
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureSprite>(structure_data);
		break;

	case KarmaStation_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureKarmaStation>(structure_data);
		break;

	case KarmaStation_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureKarmaStation>(structure_data);
		break;

	case KarmaTrack:
		// TODO: Give actual functionality to this building
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureSprite>(structure_data);
		break;

	case Rock_1:
	case Rock_2:
	case Rock_3:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureSprite>(structure_data);
		break;

	case Topia_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureTopia>(structure_data,
				100); // The capacity of angels/demons that this building provides
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, topia_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, topia_hell_sound, 1.0f, true);
		}
		break;

	case Topia_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureTopia>(structure_data,
				200); // The capacity of angels/demons that this building provides
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_hell_sound, 1.0f, true);
		}
		break;

	case TrainingCenter_T1:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureTrainingCenter>(structure_data,
				10); // The number of souls this training center convert to angels/demons per year
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_hell_sound, 1.0f, true);
		}
		break;

	case TrainingCenter_T2:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureTrainingCenter>(structure_data,
				20); // The number of souls this training center convert to angels/demons per year
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_hell_sound, 1.0f, true);
		}
		break;

	case TrainingCenter_T3:
		structure_map[tile_position.x][tile_position.z] =
			std::make_unique<StructureTrainingCenter>(structure_data,
				30); // The number of souls this training center convert to angels/demons per year
		if (plane == Heaven)
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_heaven_sound, 1.0f, true);
		}
		else
		{
			AL::NewEventManager::GenerateEventSt(AL::event_sound_start, institute_hell_sound, 1.0f, true);
		}
		break;

	default:
		break;
	}

	delete structure_data;
}

/// <summary>
/// Destroy the structure at the given tile position
/// </summary>
/// <param name="tile_position">Tile position of the structure to be destroyed</param>
void BuildingManager::DestroyStructure(Vector3 tile_position)
{
	if (structure_map[tile_position.x][tile_position.z])
	{
		structure_map[tile_position.x][tile_position.z] = nullptr;
		AL::NewEventManager::GenerateEventSt(AL::event_sound_start, destroy_sound_1, 1.0f ,true);
	}
}

/// <summary>
/// Get the vector of occupied tiles that the structure on tile_position occupies
/// </summary>
/// <param name="tile_position">The position of the structure</param>
/// <returns>A Vector (C++ dynamic list) of Vector3 (x, y, z) that contains the occupied tiles</returns>
std::vector<Vector3> BuildingManager::GetStructureOccupiedTiles(Vector3 tile_position)
{
	if (structure_map[tile_position.x][tile_position.z])
	{
		return structure_map[tile_position.x][tile_position.z]->GetOccupiedTiles();
	}
}

int BuildingManager::GetTotalStructure()
{
	int total = 0;
	for (auto& x : structure_map)
	{
		for (auto& y : x)
		{

		}
	}
	return 0;
}

/// <summary>
/// Get how many tiles a structure occupy
/// </summary>
/// <param name="structure_type">The type of structure</param>
/// <returns>Int of the size of the structure</returns>
int BuildingManager::GetSizeOfStructure(StructureType structure_type)
{
	switch (structure_type)
	{
	case Building_Green_T1:
	case Building_Yellow_T1:
	case Building_Orange_T1:
	case Building_Brown_T1:
	case Building_Purple_T1:
	case Building_Red_T1:
	case Building_Blue_T1:
	case Bank_T1:
	case KarmaTrack:
	case Rock_1:
	case Rock_2:
	case Rock_3:
		return 1;

	case Building_Green_T2:
	case Building_Yellow_T2:
	case Building_Orange_T2:
	case Building_Brown_T2:
	case Building_Purple_T2:
	case Building_Red_T2:
	case Building_Blue_T2:
	case Bank_T2:
		return 2;

	case Gate_T1:
	case Gate_T2:
	case KarmaAnchor:
	case KarmaStation_T1:
	case KarmaStation_T2:
	case TrainingCenter_T1:
	case TrainingCenter_T2:
	case TrainingCenter_T3:
		return 3;

	case KarmaPortal:
	case Gate_T3:
	case Topia_T1:
	case Topia_T2:
		return 4;

	default:
		return 0;
	}
}

/// <summary>
/// Get how much a structure costs to create
/// </summary>
/// <param name="structure_type">The type of structure</param>
/// <returns>Int of the cost of the structure</returns>
int BuildingManager::GetCostOfStructure(StructureType structure_type)
{
	switch (structure_type)
	{
	case Building_Green_T1:
	case Building_Yellow_T1:
	case Building_Orange_T1:
	case Building_Brown_T1:
	case Building_Purple_T1:
	case Building_Red_T1:
	case Building_Blue_T1:
	case Bank_T1:
	case KarmaTrack:
	case Rock_1:
	case Rock_2:
	case Rock_3:
		return 100;

	case Building_Green_T2:
	case Building_Yellow_T2:
	case Building_Orange_T2:
	case Building_Brown_T2:
	case Building_Purple_T2:
	case Building_Red_T2:
	case Building_Blue_T2:
	case Bank_T2:
		return 200;

	case Gate_T1:
	case Gate_T2:
	case KarmaAnchor:
	case KarmaStation_T1:
	case KarmaStation_T2:
	case TrainingCenter_T1:
	case TrainingCenter_T2:
	case TrainingCenter_T3:
		return 300;

	case KarmaPortal:
	case Gate_T3:
	case Topia_T1:
	case Topia_T2:
		return 400;

	default:
		return 0;
	}
}

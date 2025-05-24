#pragma once
#include "stdafx.h"
#include "LevelMapChunkData.h"

class LevelMapLayerData
{
public:
	int layer_id = 0;
	std::string layer_name;
	sf::Vector2i layer_tile_size;

	std::vector<LevelMapChunkData> chunk_data;

	LevelMapChunkData* getChunkFromPosition(sf::Vector2i chunkPosition);
};


#pragma once
#include "stdafx.h"

class LevelMapChunkData
{
public:
	struct Vector2iLess {
		bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const {
			return (a.y < b.y) || (a.y == b.y && a.x < b.x);
		}
	};

	sf::Vector2i chunkPosition;
	sf::Vector2i chunkSize;
	std::map<sf::Vector2i, int, Vector2iLess> chunkData;
};

class LevelMapLayerData
{
public:
	int layer_id = 0;
	std::string layer_name;
	sf::Vector2i layer_tile_size;

	std::vector<LevelMapChunkData> chunk_data;

	LevelMapChunkData* getChunkFromPosition(sf::Vector2i chunkPosition);
};

class LevelMapData
{
public:
	sf::Vector2i map_tile_size;
	sf::Vector2i tile_pixel_size;
	bool is_infinite = true;

	std::vector<LevelMapLayerData> layers;
};




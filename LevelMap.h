#pragma once
#include "stdafx.h"
#include "LevelMapLayerData.h"

class LevelMap
{
public:
	LevelMap();
	~LevelMap();
	sf::Vector2i map_tile_size;
	sf::Vector2i tile_pixel_size;
	bool is_infinite = true;

	std::vector<LevelMapLayerData> layers;

private:
};


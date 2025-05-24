#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "MapTile.h"

class MapChunkLayer
{
public:
	std::unordered_map<sf::Vector2i, std::vector<std::unique_ptr<MapTile>>> tiles;

	std::vector<MapTile*> currentCollisionTiles;
private:
	
};


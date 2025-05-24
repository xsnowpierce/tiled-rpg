#pragma once
#include "stdafx.h"
#include "pugixml.hpp"
#include "LevelMap.h"
#include "LevelMapLayerData.h"
#include "LevelMapChunkData.h"
#include "MapTileCollisionData.h"
#include "MapTile.h"

class LevelMapLoader
{
public:
	LevelMapLoader(sf::Texture& tilemap);
	~LevelMapLoader();

	void loadMap(const char* filepath);

	void loadChunk(sf::Vector2i chunkPosition);
	void unloadChunk(sf::Vector2i chunkPosition);
	void unloadAllChunks();

	void renderMap(sf::RenderTarget &target);
private:

	void loadTileCollisionMap();

	sf::Texture& tilemap;

	LevelMap currentMap;

	struct Vector2iLess {
		bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
			return std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
		}
	};

	std::map<sf::Vector2i, std::vector<std::unique_ptr<MapTile>>, Vector2iLess> tiles;
	std::map<int, MapTileCollisionData> tileCollisionData;
};


#pragma once
#include "stdafx.h"
#include "pugixml.hpp"
#include "LevelMap.h"
#include "LevelMapLayerData.h"
#include "LevelMapChunkData.h"
#include "MapTileCollisionData.h"
#include "MapTile.h"
#include "MapChunk.h"

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

	MapChunk* getCurrentChunk();

	
	
private:

	void loadTileCollisionMap();

	sf::Texture& tilemap;

	LevelMap currentMap;

	
	std::unordered_map<sf::Vector2i, std::unique_ptr<MapChunk>> loadedChunks;

	

	std::map<int, MapTileCollisionData> tileCollisionData;
};


#pragma once
#include "stdafx.h"
#include "pugixml.hpp"
#include "LevelMapData.h"
#include "MapTileCollisionData.h"
#include "MapTile.h"
#include "LoadedMap.h"

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

	LoadedMapChunk* getChunkFromPosition(sf::Vector2i position);

	
	
private:

	void loadTileCollisionMap();

	sf::Texture& tilemap;

	LevelMapData currentMap;
	
	std::unordered_map<sf::Vector2i, LoadedMapChunk> loadedChunks;


	std::map<int, MapTileCollisionData> tileCollisionData;
};


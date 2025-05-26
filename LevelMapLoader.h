#pragma once
#include "stdafx.h"
#include "LevelMapData.h"
#include "MapTileCollisionData.h"
#include "MapTile.h"
#include "LoadedMap.h"
#include "AnimatedTileData.h"

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

	std::unordered_map<sf::Vector2i, LoadedMapChunk>& getLoadedChunks() { return loadedChunks; };
	
private:

	void loadTileMaps();

	void loadCollisionTile(int tileID, pugi::xml_node objectNode);
	void loadAnimatedTile(int tileID, pugi::xml_node animationNode);

	sf::Texture& tilemap;

	LevelMapData currentMap;
	
	std::unordered_map<sf::Vector2i, LoadedMapChunk> loadedChunks;


	std::unordered_map<int, MapTileCollisionData> tileCollisionData;
	std::unordered_map<int, AnimatedTileData> tileAnimationData;
};


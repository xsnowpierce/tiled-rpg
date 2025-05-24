#pragma once
#include "MapUtils.h"
#include <SFML/Graphics.hpp>
#include <map>
#include "MapTile.h"

class LoadedMapLayer {

public:
	LoadedMapLayer(int layerID) { this->id = layerID; }

	void render(sf::RenderTarget& target);
	void update();

	int getLayerID() { return id; };

	std::unordered_map<sf::Vector2i, MapTile> tiles;
	std::vector<MapTile*> collisionTiles;

private:

	int id = 0;
};

class LoadedMapChunk {
public:
	
	LoadedMapChunk(sf::Vector2i chunkPosition) { this->chunkPosition = chunkPosition; };

	void render(sf::RenderTarget& target);
	void update();

	sf::Vector2i getChunkPosition() { return chunkPosition; };

	std::map<int, LoadedMapLayer> layers;

	std::vector<MapTile*> getCollisionTiles();


private:
	
	sf::Vector2i chunkPosition;

};
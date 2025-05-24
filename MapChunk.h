#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "MapTile.h"

namespace std {
	template <>
	struct hash<sf::Vector2i> {
		std::size_t operator()(const sf::Vector2i& v) const noexcept {
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
		}
	};
}

class MapChunk
{
public:
	MapChunk(
		std::unordered_map<sf::Vector2i, std::vector<std::unique_ptr<MapTile>>> tiles, std::vector<MapTile*> collisionTiles);

	~MapChunk();

	void render(sf::RenderTarget& target);

	std::vector<MapTile*>& getCollisionTiles() {
		return currentCollisionTiles;
	}

private:
	std::unordered_map<sf::Vector2i, std::vector<std::unique_ptr<MapTile>>> tiles;

	std::vector<MapTile*> currentCollisionTiles;
};


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


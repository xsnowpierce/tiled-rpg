#pragma once
#include <SFML/Graphics.hpp>

class MapTileCollisionData
{
public:
	sf::Vector2f boxPosition;
	sf::Vector2f boxSize;
	bool is_null = true;
};


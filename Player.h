#pragma once
#include "stdafx.h"
#include "AABB.h"
#include "MapTile.h"

class Player
{
public:
	Player(sf::Texture &texture);
	~Player();

	void update();
	void render(sf::RenderTarget &target);

	AABB &getAABB() { return collisionBox; }

	void checkCollisions(std::vector<AABB> tiles);

	sf::Vector2i getPlayerChunk();

private:
	sf::Sprite playerSprite;
	AABB collisionBox;

	void query_inputs();
};


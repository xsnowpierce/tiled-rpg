#pragma once
#include "stdafx.h"
#include "AABB.h"

class Player
{
public:
	Player(sf::Texture &texture);
	~Player();

	void update();
	void render(sf::RenderTarget &target);
private:
	sf::Sprite playerSprite;
	AABB collisionBox;

	void query_inputs();
};


#pragma once
#include "stdafx.h"
class Sprite
{
public:
	Sprite(sf::Texture &image, sf::IntRect spriteBounds, sf::Vector2i position);
	~Sprite();
	void move(sf::Vector2f value);
	void set_position(sf::Vector2f position);
	void update();
	void render(sf::RenderTarget &target);
private:
	sf::Sprite sprite;
};


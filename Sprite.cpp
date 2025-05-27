#include "pch.h"
#include "Sprite.h"


void Sprite::update()
{
}

void Sprite::render(sf::RenderTarget &target)
{
	target.draw(sprite);
}

Sprite::Sprite(sf::Texture &image, sf::IntRect spriteBounds, sf::Vector2i position) : sprite(image)
{
	this->sprite.setTextureRect(spriteBounds);
	this->sprite.setPosition({(float) position.x, (float) position.y});
}

Sprite::~Sprite()
{

}

void Sprite::move(sf::Vector2f value)
{
	this->sprite.move(value);
}

void Sprite::set_position(sf::Vector2f position)
{
	this->sprite.setPosition(position);
}
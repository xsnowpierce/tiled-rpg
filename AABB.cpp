#include "AABB.h"

AABB::AABB()
{
}

AABB::AABB(sf::Vector2f boxPosition, sf::Vector2f boxSize) :
	rect(boxSize)
{
	createBox(boxPosition, boxSize);
}

AABB::~AABB()
{
}

void AABB::createBox(sf::Vector2f boxPosition, sf::Vector2f boxSize)
{
	rect.setSize(boxSize);
	rect.setPosition(boxPosition);
	rect.setFillColor(sf::Color(255, 0, 0, 100));
	rect.setOutlineColor(sf::Color(255, 0, 0, 255));
	rect.setOutlineThickness(1.f);
	is_valid = true;
}

void AABB::moveBox(sf::Vector2f position)
{
	rect.setPosition(position);
}

void AABB::render(sf::RenderTarget& target)
{
	target.draw(rect);
}

sf::RectangleShape* AABB::getRectangleShape()
{
	return &rect;
}

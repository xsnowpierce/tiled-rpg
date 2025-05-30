#include "pch.h"
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
	rect.setOrigin(boxSize / 2.0f);
	rect.setPosition(boxPosition + getHalfSize());
	setColour(sf::Color::Red);
	rect.setOutlineThickness(1.f);
	is_valid = true;
}

void AABB::moveBox(sf::Vector2f position)
{
	rect.move(position);
}

void AABB::update()
{

}

void AABB::setColour(sf::Color colour)
{
	rect.setFillColor(sf::Color(colour.r, colour.g, colour.b, 100));
	rect.setOutlineColor(sf::Color(colour.r, colour.g, colour.b, 255));
}

void AABB::render(sf::RenderTarget& target)
{
	target.draw(rect);
}

sf::RectangleShape* AABB::getRectangleShape()
{
	return &rect;
}

bool AABB::checkCollision(AABB& other, float weight)
{
	weight = std::clamp(weight, 0.0f, 1.0f);
	float invWeight = 1.0f - weight;

	const sf::Vector2f delta = other.getPosition() - getPosition();
	const sf::Vector2f totalHalfSize = other.getHalfSize() + getHalfSize();

	const float intersectX = std::abs(delta.x) - totalHalfSize.x;
	const float intersectY = std::abs(delta.y) - totalHalfSize.y;

	if (intersectX < 0.0f && intersectY < 0.0f) {
		if (std::abs(intersectX) < std::abs(intersectY)) {
			const float move = (delta.x > 0.0f) ? intersectX : -intersectX;
			moveBox({ move * invWeight, 0.0f });
			other.moveBox({ -move * weight, 0.0f });
		}
		else {
			const float move = (delta.y > 0.0f) ? intersectY : -intersectY;
			moveBox({ 0.0f, move * invWeight });
			other.moveBox({ 0.0f, -move * weight });
		}
		return true;
	}

	return false;
}


void AABB::setPosition(sf::Vector2f position)
{
	this->rect.setPosition(position + getHalfSize());
}

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

void AABB::setColour(sf::Color colour)
{
	rect.setFillColor(sf::Color(colour.r, colour.g, colour.b, 100));
	rect.setOutlineColor(sf::Color(colour.r, colour.g, colour.b, 255));
}

void AABB::render(sf::RenderTarget& target)
{
	//target.draw(rect);
}

sf::RectangleShape* AABB::getRectangleShape()
{
	return &rect;
}

bool AABB::checkCollision(AABB& other, float weight)
{
	sf::Vector2f otherPosition = other.getPosition();
	sf::Vector2f otherHalfSize = other.getHalfSize();
	
	sf::Vector2f ourPosition = getPosition();
	sf::Vector2f ourHalfSize = getHalfSize();

	float deltaX = otherPosition.x - ourPosition.x;
	float deltaY = otherPosition.y - ourPosition.y;

	float intersectX = abs(deltaX) - (otherHalfSize.x + ourHalfSize.x);
	float intersectY = abs(deltaY) - (otherHalfSize.y + ourHalfSize.y);

	if (intersectX < 0.0f && intersectY < 0.0f) {
		weight = std::clamp(weight, 0.0f, 1.0f);

		if (abs(intersectX) < abs(intersectY)) {
			if (deltaX > 0.0f) {
				moveBox({ intersectX * (1.0f - weight), 0.0f });
				other.moveBox({-intersectX * weight, 0.0f });
			}
			else {
				moveBox({ -intersectX * (1.0f - weight), 0.0f });
				other.moveBox({ intersectX * weight, 0.0f });
			}
		}
		else {
			if (deltaY > 0.0f) {
				moveBox({0.0f,  intersectY * (1.0f - weight)});
				other.moveBox({0.0f, -intersectY * weight});
			}
			else {
				moveBox({0.0f, -intersectY * (1.0f - weight)});
				other.moveBox({0.0f, intersectY * weight});
			}
		}

		return true;
	}

	return false;
}

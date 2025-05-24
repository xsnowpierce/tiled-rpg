#pragma once
#include <SFML/Graphics.hpp>

class AABB
{
public:
	AABB();
	AABB(sf::Vector2f boxPosition, sf::Vector2f boxSize);
	~AABB();

	void createBox(sf::Vector2f boxPosition, sf::Vector2f boxSize);

	void moveBox(sf::Vector2f position);

	void update();

	void setColour(sf::Color colour);
	void render(sf::RenderTarget &target);

	bool is_valid = false;

	sf::RectangleShape* getRectangleShape();

	bool checkCollision(AABB& other, float weight);
	sf::Vector2f getPosition() { return rect.getPosition(); }
	sf::Vector2f getHalfSize() { return rect.getSize() / 2.0f; }

private:
	sf::RectangleShape rect;
};
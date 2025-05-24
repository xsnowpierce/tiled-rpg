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
	void render(sf::RenderTarget &target);

	bool is_valid = false;

	sf::RectangleShape* getRectangleShape();

private:
	sf::RectangleShape rect;
};
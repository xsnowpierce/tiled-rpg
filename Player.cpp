#include "Player.h"

Player::Player(sf::Texture &texture) :
	playerSprite(texture), collisionBox({}, {16, 16})
{
	playerSprite.setTextureRect(sf::IntRect({ 16, 0 }, { 16, 16 }));
	playerSprite.setPosition({});

}

Player::~Player()
{
}

void Player::update()
{
	this->query_inputs();
}

void Player::render(sf::RenderTarget& target)
{
	playerSprite.setPosition(collisionBox.getPosition() - collisionBox.getHalfSize());
	target.draw(playerSprite);
	collisionBox.render(target);
}

void Player::checkCollisions(std::vector<MapTile*> tiles)
{
	for (MapTile* tile : tiles) {
		if (tile) {
			tile->getAABB().checkCollision(collisionBox, 1.0f);
		}
	}
}


void Player::query_inputs()
{
	int X_move = 0;
	int Y_move = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
		X_move--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
		X_move++;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
		Y_move--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
		Y_move++;

	this->collisionBox.moveBox({ (float) X_move, (float) Y_move });
}

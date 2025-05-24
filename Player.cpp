#include "Player.h"

Player::Player(sf::Texture &texture) :
	playerSprite(texture), collisionBox({}, {16, 24})
{
	playerSprite.setTextureRect(sf::IntRect({ 16, 0 }, { 16, 16 }));
	playerSprite.setPosition({});
	playerSprite.setScale({ 1, 1.5f });

}

Player::~Player()
{
}

void Player::update()
{
	this->query_inputs();
	collisionBox.moveBox(playerSprite.getPosition());
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(playerSprite);
	collisionBox.render(target);
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

	this->playerSprite.move({ (float) X_move, (float) Y_move });
}

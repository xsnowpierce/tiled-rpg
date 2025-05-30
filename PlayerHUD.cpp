#include "pch.h"
#include "PlayerHUD.h"
#include "Scene.h"

PlayerHUD::PlayerHUD(Scene* targetScene) : heartTexture("resources/images/hearts.png"), heartSprite(heartTexture)
{
	this->scene = targetScene;

	heartSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 12, 12 }));
}

void PlayerHUD::update(float deltaTime)
{
}

void PlayerHUD::render(sf::RenderTarget& target)
{
	sf::Vector2f position = this->scene->getView()->getCenter();
	position.x -= GameSettings::screenWidth / 2.f;
	position.y -= GameSettings::screenHeight / 2.f;
	heartSprite.setPosition(position);
	target.draw(heartSprite);
}

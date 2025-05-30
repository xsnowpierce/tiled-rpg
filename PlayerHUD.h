#pragma once
#include "pch.h"

class Scene;

class PlayerHUD
{
public:
	PlayerHUD(Scene* targetScene);
	void update(float deltaTime);
	void render(sf::RenderTarget& target);
private:
	Scene* scene;
	sf::Texture heartTexture;
	sf::Sprite heartSprite;
};


#pragma once
#include "stdafx.h"
#include "LevelMap.h"
#include "Sprite.h"
#include "LevelMapLoader.h"
#include "Player.h"

class Scene
{
private:
	void renderMap();
	LevelMap currentMap;
	sf::Texture tilemap;
	LevelMapLoader mapLoader;

	sf::Texture playerTexture;
	Player player;
public:
	Scene();
	~Scene();
	void update();
	void render(sf::RenderTarget& target);
	void pollEvent(sf::Event event);
};


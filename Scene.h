#pragma once
#include "MapUtils.h"
#include "stdafx.h"
#include "LevelMapData.h"
#include "Sprite.h"
#include "LevelMapLoader.h"
#include "Player.h"

class Scene
{
private:
	LevelMapData currentMap;
	sf::Texture tilemap;
	LevelMapLoader mapLoader;

	sf::Texture playerTexture;
	Player player;
	sf::View* view;

public:
	Scene(sf::View* view);
	~Scene();
	void update();
	void render(sf::RenderTarget& target);
	void pollEvent(sf::Event event);
};


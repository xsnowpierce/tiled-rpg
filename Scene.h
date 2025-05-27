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

	sf::Vector2f screenMoveStart;
	sf::Vector2f screenMoveFinish;
	float screenMoveProgress = 0.f;
	bool screenIsMoving = false;

	void viewInputs();
	void updateNewScreenMovement(float deltaTime);

	float lerp(float a, float b, float t);

	sf::Vector2i currentMainChunk;
	sf::Vector2i currentTargetChunk;

public:
	Scene(sf::View* view);
	~Scene();
	void update(float deltaTime);
	void render(sf::RenderTarget& target);
	void pollEvent(sf::Event event);

	void pullNewScreen(sf::Vector2f direction);
	

};


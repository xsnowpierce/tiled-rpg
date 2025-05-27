#pragma once
#include "pch.h"
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

	void updateNewScreenMovement(float deltaTime);

	void createScreenBoundaryColliders();
	void checkScreenBoundaryColliders();

	float lerp(float a, float b, float t);

	sf::Vector2i currentMainChunk;
	sf::Vector2i currentTargetChunk;

	AABB leftScreenAABB, rightScreenAABB, topScreenAABB, bottomScreenAABB;

public:
	Scene(sf::View* view);
	~Scene();
	void update(float deltaTime);
	void render(sf::RenderTarget& target);
	void pollEvent(sf::Event event);

	void pullNewScreen(sf::Vector2f direction);
	
	bool isLoadingNewScreen() { return screenIsMoving; }

	sf::View* getView() { return view; }
};


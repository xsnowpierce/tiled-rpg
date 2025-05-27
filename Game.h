#pragma once

#include "pch.h"
#include "Scene.h"

class Game
{
private:
	sf::RenderWindow window;
	sf::View getLetterboxView(sf::View view, sf::Vector2f windowSize);
	sf::View view;
	sf::Vector2i resolution = sf::Vector2i(240, 160);

	Scene* currentScene;

	void initWindow();
	void initScene();

public:
	Game();
	virtual ~Game();

	void updateScene(float deltaTime);
	void update(float deltaTime);
	void renderScene();
	void render();

	const sf::RenderWindow& getWindow() const;
};


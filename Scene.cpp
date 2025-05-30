#include "pch.h"
#include "Scene.h"

float Scene::lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

Scene::Scene(sf::View* view) 
	: tilemap("resources/images/Overworld.png"), 
	mapLoader(tilemap), 
	playerTexture("resources/images/character.png"), 
	player(playerTexture, this),
	playerHUD(this)
{
	this->view = view;
	createScreenBoundaryColliders();
	mapLoader.loadMap("resources/tiled/map.xml");
	mapLoader.loadChunk(player.getPlayerChunk());
	currentMainChunk = player.getPlayerChunk();
}

Scene::~Scene()
{

}

void Scene::update(float deltaTime)
{
	player.update(deltaTime);

	playerHUD.update(deltaTime);

	for (auto& [chunkPos, chunk] : mapLoader.getLoadedChunks()) {
		if(!screenIsMoving)
			chunk.update(deltaTime);
		player.checkCollisions(chunk.getCollisionTiles());
	}

	checkScreenBoundaryColliders();

	if (screenIsMoving) {
		// screen is moving, update movement
		updateNewScreenMovement(deltaTime);
		moveScreenBoundaryColliders();
	}
}

void Scene::render(sf::RenderTarget& target)
{
	mapLoader.renderMap(target);
	player.render(target);
	playerHUD.render(target);

	//view->setCenter(player.getPlayerPosition());
	/*
	leftScreenAABB.render(target);
	rightScreenAABB.render(target);
	topScreenAABB.render(target);
	bottomScreenAABB.render(target);
	*/
}

void Scene::pollEvent(sf::Event event)
{

}

void Scene::pullNewScreen(sf::Vector2f direction)
{
	sf::Vector2f screenDirection = { direction.x * view->getSize().x, direction.y * view->getSize().y };
	screenMoveStart = view->getCenter();
	screenMoveFinish = screenMoveStart + screenDirection;
	screenMoveProgress = 0.f;
	screenIsMoving = true;
	currentTargetChunk = currentMainChunk + sf::Vector2i({ (int)(direction.x * GameSettings::SCREEN_TILE_SIZE_W), (int)(direction.y * GameSettings::SCREEN_TILE_SIZE_H) });
	std::cout << "current chunk: " << currentMainChunk.x << ", " << currentMainChunk.y << std::endl;
	std::cout << "current target chunk: " << currentTargetChunk.x << ", " << currentTargetChunk.y << std::endl;
	mapLoader.loadChunk(currentTargetChunk);
}

void Scene::updateNewScreenMovement(float deltaTime)
{
	const float speed = 1.f;

	if (screenIsMoving) {
		screenMoveProgress += deltaTime * speed;

		if (screenMoveProgress > 1.0f)
			screenMoveProgress = 1.0f;

		sf::Vector2f newOffset = screenMoveStart + (screenMoveFinish - screenMoveStart) * screenMoveProgress;
		sf::Vector2f frameMove = newOffset - view->getCenter();

		view->move(frameMove);

		if (screenMoveProgress >= 1.0f) {
			screenIsMoving = false;
			view->setCenter(screenMoveFinish);
			mapLoader.unloadChunk({ currentMainChunk });
			currentMainChunk = currentTargetChunk;
			currentTargetChunk = sf::Vector2i({});
		}
	}
}

void Scene::createScreenBoundaryColliders()
{
	sf::Vector2f offset(view->getCenter() - (view->getSize() / 2.f));

	leftScreenAABB.createBox({offset.x - 16, offset.y}, { 16, view->getSize().y });
	rightScreenAABB.createBox({offset.x + view->getSize().x, offset.y}, {16, view->getSize().y});
	topScreenAABB.createBox({offset.x, offset.y - 16}, {view->getSize().x, 16});
	bottomScreenAABB.createBox({offset.x, offset.y + view->getSize().y}, {view->getSize().x, 16});
}

void Scene::moveScreenBoundaryColliders()
{
	sf::Vector2f offset(view->getCenter() - (view->getSize() / 2.f));

	leftScreenAABB.setPosition({ offset.x - 16, offset.y });
	rightScreenAABB.setPosition({ offset.x + view->getSize().x, offset.y });
	topScreenAABB.setPosition({ offset.x, offset.y - 16 });
	bottomScreenAABB.setPosition({ offset.x, offset.y + view->getSize().y });
}

void Scene::checkScreenBoundaryColliders()
{
	sf::Vector2f newScreen;
	if (player.checkCollision(rightScreenAABB) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		newScreen.x += 1;
	}
	else if (player.checkCollision(leftScreenAABB) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
		newScreen.x -= 1;
	}

	if (player.checkCollision(topScreenAABB) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
		newScreen.y -= 1;
	}
	else if (player.checkCollision(bottomScreenAABB) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
		newScreen.y += 1;
	}

	if (newScreen.length() != 0 && !screenIsMoving) {
		pullNewScreen(newScreen);
	}
}

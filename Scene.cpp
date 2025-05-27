#include "pch.h"
#include "Scene.h"

float Scene::lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

Scene::Scene(sf::View* view) : tilemap("resources/images/Overworld.png"), mapLoader(tilemap), playerTexture("resources/images/character.png"), player(playerTexture, this)
{
	this->view = view;
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

	for (auto& [chunkPos, chunk] : mapLoader.getLoadedChunks()) {
		if(!screenIsMoving)
			chunk.update(deltaTime);
		player.checkCollisions(chunk.getCollisionTiles());
	}

	if (!screenIsMoving) {
		viewInputs();
	}
	else{
		// screen is moving, update movement
		updateNewScreenMovement(deltaTime);
	}
}

void Scene::render(sf::RenderTarget& target)
{
	mapLoader.renderMap(target);
	player.render(target);
}

void Scene::pollEvent(sf::Event event)
{

}

void Scene::viewInputs()
{
	sf::Vector2f moveDelta = {};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
		moveDelta.y -= 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
		moveDelta.x -= 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		moveDelta.x += 1;

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
		moveDelta.y += 1;
	}

	if (moveDelta.length() > 0.f && !screenIsMoving)
		this->pullNewScreen({ moveDelta });
}

void Scene::pullNewScreen(sf::Vector2f direction)
{
	sf::Vector2f screenDirection = { direction.x * view->getSize().x, direction.y * view->getSize().y };
	screenMoveStart = view->getCenter();
	screenMoveFinish = screenMoveStart + screenDirection;
	screenMoveProgress = 0.f;
	screenIsMoving = true;
	currentTargetChunk = currentMainChunk + sf::Vector2i({ (int)(direction.x * 15.f), (int)(direction.y * 10.f) });
	//std::cout << "current chunk: " << currentMainChunk.x << ", " << currentMainChunk.y << std::endl;
	//std::cout << "current target chunk: " << currentTargetChunk.x << ", " << currentTargetChunk.y << std::endl;
	mapLoader.loadChunk(currentTargetChunk);
}

void Scene::updateNewScreenMovement(float deltaTime)
{
	const float speed = 2.0f;

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
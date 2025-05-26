#include "Scene.h"


Scene::Scene(sf::View* view) : tilemap("resources/images/Overworld.png"), mapLoader(tilemap), playerTexture("resources/images/tiles.png"), player(playerTexture)
{
	this->view = view;
	mapLoader.loadMap("resources/tiled/map.xml");
	mapLoader.loadChunk(player.getPlayerChunk());
	mapLoader.loadChunk({0, -10});
	mapLoader.loadChunk({15, 0});
}

Scene::~Scene()
{

}

void Scene::update()
{
	player.update();

	for (auto& [chunkPos, chunk] : mapLoader.getLoadedChunks()) {
		chunk.update();
		player.checkCollisions(chunk.getCollisionTiles());
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space)) {
		mapLoader.unloadChunk({ 0, 0 });
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
		this->view->move({ 0, -1 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
		this->view->move({ -1, 0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		this->view->move({ 1, 0 });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
		this->view->move({ 0, 1 });
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

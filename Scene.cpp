#include "Scene.h"


Scene::Scene() : tilemap("resources/images/Overworld.png"), mapLoader(tilemap), playerTexture("resources/images/tiles.png"), player(playerTexture)
{
	mapLoader.loadMap("resources/tiled/map.xml");
	mapLoader.loadChunk({0,0});


}

Scene::~Scene()
{

}

void Scene::update()
{
	player.update();
}

void Scene::render(sf::RenderTarget& target)
{
	mapLoader.renderMap(target);
	player.render(target);
}

void Scene::renderMap()
{

}



void Scene::pollEvent(sf::Event event)
{

}

#include "LoadedMap.h"

void LoadedMapChunk::render(sf::RenderTarget& target)
{
	for (auto& pair : layers) {
        pair.second.render(target);
	}
}


void LoadedMapChunk::update()
{
    for (auto& pair : layers) {
        pair.second.update();
    }
}

std::vector<MapTile*> LoadedMapChunk::getCollisionTiles() {
    std::vector<MapTile*> allCollisionTiles;

    for (const auto& [layerID, layer] : layers) {
        allCollisionTiles.insert(
            allCollisionTiles.end(),
            layer.collisionTiles.begin(),
            layer.collisionTiles.end()
        );
    }

    return allCollisionTiles;
}

void LoadedMapLayer::render(sf::RenderTarget& target)
{
    for (auto& pair : tiles) {
        MapTile& tile = pair.second;

        tile.render(target);
    }

}

void LoadedMapLayer::update()
{
    for (auto& pair : tiles) {
        MapTile& tile = pair.second;

        tile.update();
    }
}

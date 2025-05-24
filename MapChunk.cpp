#include "MapChunk.h"

MapChunk::MapChunk(std::unordered_map<sf::Vector2i, std::vector<std::unique_ptr<MapTile>>> tiles, std::vector<MapTile*> collisionTiles)
    : tiles(std::move(tiles)), currentCollisionTiles(std::move(collisionTiles))
{

}

MapChunk::~MapChunk()
{

}

void MapChunk::render(sf::RenderTarget& target)
{
    for (const auto& pair : tiles)
    {
        for (const auto& tile : pair.second)
        {
            tile->render(target);
        }
    }
}

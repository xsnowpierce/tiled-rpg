#pragma once
#include <SFML/Graphics.hpp>
#include "MapTileCollisionData.h"
#include "AABB.h"
#include <iostream>

class MapTile {
public:
    MapTile(sf::Texture& texture, int tileID, sf::Vector2f position, std::optional<MapTileCollisionData> collisionData);
    ~MapTile();

    void render(sf::RenderTarget& target);
    AABB &getAABB() { return collider; }
    sf::Vector2f getTilePosition() { return sprite.getPosition(); }
    sf::Vector2i chunkPosition;

private:
    int tileID = 0;
    void createCollider(MapTileCollisionData collisionData);
    sf::Sprite sprite;
    static sf::IntRect getIntRectFromTileID(int tileID);
    AABB collider;
};

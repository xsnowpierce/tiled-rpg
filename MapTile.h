#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include "MapTileCollisionData.h"
#include "AABB.h"
#include <iostream>
#include "AnimatedTileData.h"

class MapTile {
public:

    MapTile(sf::Texture& texture, int tileID, sf::Vector2f position, std::optional<MapTileCollisionData> collisionData, std::optional<AnimatedTileData> animationData);
    ~MapTile();

    virtual void render(sf::RenderTarget& target);
    virtual void update();

    AABB &getAABB() { return collider; }
    sf::Vector2f getTilePosition() { return sprite.getPosition(); }

    sf::Vector2i chunkPosition;

private:

    int tileID = 0;

    bool is_animated = false;
    int currentAnimationFrame = 0;
    float currentAnimationTimer = 0.0f;
    void progressAnimationFrame();

    sf::Sprite sprite;
    AABB collider;

    void createCollider(MapTileCollisionData collisionData);
    static sf::IntRect getIntRectFromTileID(int tileID);
    
    AnimatedTileData animatedTileSet;
};

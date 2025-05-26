#include "MapTile.h"

MapTile::MapTile(sf::Texture& texture, int tileID, sf::Vector2f position, std::optional<MapTileCollisionData> collisionData, std::optional<AnimatedTileData> animationData) 
    : sprite(texture)
{  
    this->tileID = tileID;
    sprite.setTextureRect(MapTile::getIntRectFromTileID(tileID));
    sprite.setPosition(position);

    if (collisionData.has_value() && collisionData.value().is_null == false) {
        createCollider(collisionData.value());
    }

    if (animationData.has_value()) {
        if (animationData.value().frames.size() > 0) {
            //std::cout << "tile " << tileID << " passed animation data of " << animationData.value().frames.size() << " frames." << std::endl;
            is_animated = true;
            this->animatedTileSet = animationData.value();
        }
        else {
            //std::cout << "tile " << tileID << " has no animation frames." << std::endl;
        }
    }
}

MapTile::~MapTile()
{
}

void MapTile::render(sf::RenderTarget& target)
{
    target.draw(sprite);
    if (collider.is_valid)
        collider.render(target);
}

void MapTile::update()
{
    if (!is_animated) return;

    currentAnimationTimer -= 0.01666666666f;
    //std::cout << currentAnimationTimer << std::endl;
    if (currentAnimationTimer <= 0.0f) {
        progressAnimationFrame();
    }
}

void MapTile::progressAnimationFrame()
{
    currentAnimationFrame++;

    if (animatedTileSet.frames.size() <= currentAnimationFrame) {
        //::cout << "reset frame progression" << std::endl;
        currentAnimationFrame = 0;
    }

    sprite.setTextureRect(getIntRectFromTileID(animatedTileSet.frames[currentAnimationFrame].tileID + 1));
    currentAnimationTimer = animatedTileSet.frames[currentAnimationFrame].delayTime;

    //std::cout << currentAnimationFrame << ", size is " << animatedTileSet.frames.size() << std::endl;
}

void MapTile::createCollider(MapTileCollisionData collisionData)
{
    sf::Vector2f correctPosition({collisionData.boxPosition.x, collisionData.boxPosition.y});
    collider.createBox(sprite.getPosition() + correctPosition, collisionData.boxSize);
    collider.setColour(sf::Color::Blue);
    //std::cout << "collision box for tileID " << tileID << " (" << sprite.getPosition().x << "," << sprite.getPosition().y << ")" << " created at " << collider.getRectangleShape()->getPosition().x << ", " << collider.getRectangleShape()->getPosition().y << std::endl;
}

sf::IntRect MapTile::getIntRectFromTileID(int tileID)
{
    tileID -= 1;
    int columns = 40;
    int tileSize = 16;

    int x = (tileID % columns) * tileSize;
    int y = (tileID / columns) * tileSize;

    return sf::IntRect({ x, y }, { tileSize, tileSize });
}

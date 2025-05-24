#include "MapTile.h"

MapTile::MapTile(sf::Texture& texture, int tileID, sf::Vector2f position, std::optional<MapTileCollisionData> collisionData) :
    sprite(texture)
{  
    this->tileID = tileID;
    sprite.setTextureRect(MapTile::getIntRectFromTileID(tileID));
    sprite.setPosition(position);

    if (collisionData.has_value() && collisionData.value().is_null == false) {
        createCollider(collisionData.value());
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

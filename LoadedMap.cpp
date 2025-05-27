#include "LoadedMap.h"

LoadedMapChunk::LoadedMapChunk(sf::Vector2i chunkPosition) : chunkPosition(chunkPosition) {}

void LoadedMapChunk::render(sf::RenderTarget& target, const sf::Texture& tilemap)
{
    for (auto& [id, layer] : layers) {
        layer.render(target, tilemap);
    }
}

void LoadedMapChunk::update(float deltaTime)
{
    for (auto& [id, layer] : layers) {
        layer.update(deltaTime);
    }
}

std::vector<AABB> LoadedMapChunk::getCollisionTiles() const {
    std::vector<AABB> all;

    for (const auto& [id, layer] : layers) {
        all.insert(all.end(), layer.collisionTiles.begin(), layer.collisionTiles.end());
    }

    return all;
}


LoadedMapLayer::LoadedMapLayer(int layerID) : id(layerID)
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
}

int LoadedMapLayer::getLayerID() const {
    return id;
}

void LoadedMapLayer::render(sf::RenderTarget& target, const sf::Texture& tilemap)
{
    sf::RenderStates states;
    states.texture = &tilemap;
    target.draw(vertices, states);
}

void LoadedMapLayer::update(float deltaTime) {
    if (tilesetWidth == 0) return;

    for (auto& animTile : animatedTiles) {
        if (animTile.vertexIndex + 5 >= vertices.getVertexCount()) {
            continue;
        }

        const auto& animData = tileAnimationData[animTile.baseTileID];

        
        if (animData.frames.empty()) continue;

        float currentFrameDuration = animData.frames.at(animTile.currentFrame).delayTime;
        if (currentFrameDuration <= 0.0f) continue;

        animTile.elapsedTime += deltaTime;
        

        if (animTile.elapsedTime >= currentFrameDuration) {

            animTile.elapsedTime = 0.0f;
            animTile.currentFrame = (animTile.currentFrame + 1) % animData.frames.size();

            int frameTileID = animData.frames.at(animTile.currentFrame).tileID;
            int tu = (frameTileID) % tilesetWidth;
            int tv = (frameTileID) / tilesetWidth;

            int x = animTile.gridPosition.x;
            int y = animTile.gridPosition.y;


            sf::Vertex* quad = &vertices[animTile.vertexIndex];

            quad[0].texCoords = { float(tu * tileSize.x), float(tv * tileSize.y) };
            quad[1].texCoords = { float((tu + 1) * tileSize.x), float(tv * tileSize.y) };
            quad[2].texCoords = { float(tu * tileSize.x), float((tv + 1) * tileSize.y) };

            quad[3].texCoords = { float(tu * tileSize.x), float((tv + 1) * tileSize.y) };
            quad[4].texCoords = { float((tu + 1) * tileSize.x), float(tv * tileSize.y) };
            quad[5].texCoords = { float((tu + 1) * tileSize.x), float((tv + 1) * tileSize.y) };
        }
    }
}



void LoadedMapLayer::buildVertexArray(
    const LevelMapChunkData& chunkData,
    const sf::Texture& tileset,
    sf::Vector2i tileSize,
    const std::unordered_map<int, MapTileCollisionData>& collisionData,
    const std::unordered_map<int, AnimatedTileData>& animationData) 
{
    vertices.clear();

    tilesetWidth = tileset.getSize().x / tileSize.x;
    this->tileAnimationData = animationData;
    this->tileCollisionData = collisionData;
    this->tileSize = tileSize;
    this->chunkSize = chunkData.chunkSize;
    vertices.resize(chunkSize.x * chunkSize.y * 6);


    long v = 0;
    for (int y = 0; y < chunkData.chunkSize.y; ++y) {
        for (int x = 0; x < chunkData.chunkSize.x; ++x) {
            int tileID = chunkData.chunkData.at({ x, y });
            if (tileID == 0) continue;

            int tu = (tileID - 1) % tilesetWidth;
            int tv = (tileID - 1) / tilesetWidth;

            float worldX = (chunkData.chunkPosition.x + x) * tileSize.x;
            float worldY = (chunkData.chunkPosition.y + y) * tileSize.y;

            sf::Vertex* quad = &vertices[v];

            // tri 1
            quad[0].position = { worldX, worldY };
            quad[1].position = { worldX + tileSize.x, worldY };
            quad[2].position = { worldX, worldY + tileSize.y };

            quad[0].texCoords = { float(tu * tileSize.x), float(tv * tileSize.y) };
            quad[1].texCoords = { float((tu + 1) * tileSize.x), float(tv * tileSize.y) };
            quad[2].texCoords = { float(tu * tileSize.x), float((tv + 1) * tileSize.y) };

            // tri 2
            quad[3].position = { worldX, worldY + tileSize.y };
            quad[4].position = { worldX + tileSize.x, worldY };
            quad[5].position = { worldX + tileSize.x, worldY + tileSize.y };

            quad[3].texCoords = { float(tu * tileSize.x), float((tv + 1) * tileSize.y) };
            quad[4].texCoords = { float((tu + 1) * tileSize.x), float(tv * tileSize.y) };
            quad[5].texCoords = { float((tu + 1) * tileSize.x), float((tv + 1) * tileSize.y) };

            // add collision
            auto colDataIt = tileCollisionData.find(tileID);
            if (colDataIt != tileCollisionData.end() && !colDataIt->second.is_null) {
                const auto& colData = colDataIt->second;

                AABB box;
                box.createBox(
                    sf::Vector2f(worldX + colData.boxPosition.x, worldY + colData.boxPosition.y),
                    colData.boxSize
                );
                collisionTiles.push_back(box);
            }

            if (tileAnimationData.find(tileID) != tileAnimationData.end()) {
                AnimatedTileInstance animInstance;
                animInstance.baseTileID = tileID;
                animInstance.vertexIndex = v;
                animInstance.currentFrame = 0;
                animInstance.elapsedTime = 0.f;
                animatedTiles.push_back(animInstance);
            }

            v += 6;
        }
    }

    vertices.resize(v);
}


#include "pch.h"
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
    if (GameSettings::SHOW_DEBUG_COLLIDERS) {
        for (AABB collisionTile : collisionTiles) {
            collisionTile.render(target);
        }
    }
}

void LoadedMapLayer::update(float deltaTime) {
    if (tilesetWidth == 0 || animatedTiles.empty()) return;

    tickTimer += deltaTime;
    if (tickTimer < tickInterval) return;

    // Number of whole ticks that passed
    int ticksToRun = static_cast<int>(tickTimer / tickInterval);
    tickTimer -= ticksToRun * tickInterval;

    for (auto& animTile : animatedTiles) {
        if (animTile.vertexIndex + 5 >= vertices.getVertexCount()) {
            continue;
        }

        const auto& animData = *animTile.animData;
        if (animData.frames.empty()) continue;

        animTile.elapsedTime += ticksToRun * tickInterval;

        float frameDuration = animData.frames.at(animTile.currentFrame).delayTime;
        if (frameDuration <= 0.0f) continue;

        while (animTile.elapsedTime >= frameDuration) {
            animTile.elapsedTime -= frameDuration;
            animTile.currentFrame = (animTile.currentFrame + 1) % animData.frames.size();

            const auto& frame = animData.frames.at(animTile.currentFrame);
            sf::Vertex* quad = &vertices[animTile.vertexIndex];
            for (int i = 0; i < 6; ++i) {
                quad[i].texCoords = frame.uvCoords[i];
            }

            frameDuration = animData.frames.at(animTile.currentFrame).delayTime;
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
    collisionTiles.clear();
    animatedTiles.clear();

    tilesetWidth = tileset.getSize().x / tileSize.x;
    this->tileAnimationData = animationData;
    this->tileCollisionData = collisionData;
    this->tileSize = tileSize;
    this->chunkSize = chunkData.chunkSize;
    vertices.resize(chunkSize.x * chunkSize.y * 6);

    std::vector<std::vector<MapTileCollisionData>> collisionGrid(chunkSize.y, std::vector<MapTileCollisionData>(chunkSize.x));
    long v = 0;

    for (int y = 0; y < chunkSize.y; ++y) {
        for (int x = 0; x < chunkSize.x; ++x) {
            int tileID = chunkData.chunkData.at({ x, y });
            if (tileID == 0) continue;

            int tu = (tileID - 1) % tilesetWidth;
            int tv = (tileID - 1) / tilesetWidth;

            float worldX = float(chunkData.chunkPosition.x + x) * tileSize.x;
            float worldY = float(chunkData.chunkPosition.y + y) * tileSize.y;

            sf::Vertex* quad = &vertices[v];

            quad[0].position = { worldX, worldY };
            quad[1].position = { worldX + tileSize.x, worldY };
            quad[2].position = { worldX, worldY + tileSize.y };

            quad[0].texCoords = { float(tu * tileSize.x), float(tv * tileSize.y) };
            quad[1].texCoords = { float((tu + 1) * tileSize.x), float(tv * tileSize.y) };
            quad[2].texCoords = { float(tu * tileSize.x), float((tv + 1) * tileSize.y) };

            quad[3].position = { worldX, worldY + tileSize.y };
            quad[4].position = { worldX + tileSize.x, worldY };
            quad[5].position = { worldX + tileSize.x, worldY + tileSize.y };

            quad[3].texCoords = { float(tu * tileSize.x), float((tv + 1) * tileSize.y) };
            quad[4].texCoords = { float((tu + 1) * tileSize.x), float(tv * tileSize.y) };
            quad[5].texCoords = { float((tu + 1) * tileSize.x), float((tv + 1) * tileSize.y) };

            auto colDataIt = tileCollisionData.find(tileID);
            if (colDataIt != tileCollisionData.end()) {
                collisionGrid[y][x] = colDataIt->second;
            }

            if (tileAnimationData.find(tileID) != tileAnimationData.end()) {
                AnimatedTileInstance animInstance;
                animInstance.baseTileID = tileID;
                animInstance.animData = &tileAnimationData.at(tileID);
                animInstance.vertexIndex = v;
                animInstance.currentFrame = 0;
                animInstance.elapsedTime = 0.f;
                animatedTiles.push_back(animInstance);
            }

            v += 6;
        }
    }

    vertices.resize(v);

    std::vector<std::vector<bool>> solidSubgrid(chunkSize.y * 2, std::vector<bool>(chunkSize.x * 2, false));
    std::vector<std::vector<bool>> visited(chunkSize.y * 2, std::vector<bool>(chunkSize.x * 2, false));

    for (int y = 0; y < chunkSize.y; ++y) {
        for (int x = 0; x < chunkSize.x; ++x) {
            const MapTileCollisionData& data = collisionGrid[y][x];

            int sx = x * 2;
            int sy = y * 2;

            if (data.collision_top_left)     solidSubgrid[sy][sx] = true;
            if (data.collision_top_right)    solidSubgrid[sy][sx + 1] = true;
            if (data.collision_bottom_left)  solidSubgrid[sy + 1][sx] = true;
            if (data.collision_bottom_right) solidSubgrid[sy + 1][sx + 1] = true;
        }
    }

    int subTileWidth = tileSize.x / 2;
    int subTileHeight = tileSize.y / 2;

    for (int y = 0; y < chunkSize.y * 2; ++y) {
        for (int x = 0; x < chunkSize.x * 2; ++x) {
            if (!solidSubgrid[y][x] || visited[y][x])
                continue;

            int width = 1;
            while (x + width < chunkSize.x * 2 &&
                solidSubgrid[y][x + width] &&
                !visited[y][x + width]) {
                ++width;
            }

            int height = 1;
            bool canGrow = true;
            while (y + height < chunkSize.y * 2 && canGrow) {
                for (int i = 0; i < width; ++i) {
                    if (!solidSubgrid[y + height][x + i] || visited[y + height][x + i]) {
                        canGrow = false;
                        break;
                    }
                }
                if (canGrow) ++height;
            }

            for (int dy = 0; dy < height; ++dy) {
                for (int dx = 0; dx < width; ++dx) {
                    visited[y + dy][x + dx] = true;
                }
            }

            float worldX = float(chunkData.chunkPosition.x) * tileSize.x + x * subTileWidth;
            float worldY = float(chunkData.chunkPosition.y) * tileSize.y + y * subTileHeight;

            sf::Vector2f size = {
                float(width * subTileWidth),
                float(height * subTileHeight)
            };

            AABB box;
            box.createBox(sf::Vector2f(worldX, worldY), size);
            collisionTiles.push_back(box);
        }
    }
}



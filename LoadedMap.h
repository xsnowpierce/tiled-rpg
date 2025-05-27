#pragma once
#include "pch.h"
#include "LevelMapData.h"
#include "AnimatedTileData.h"
#include "MapTileCollisionData.h"
#include "AABB.h"

class LoadedMapLayer {
public:

    LoadedMapLayer(int layerID);

    void render(sf::RenderTarget& target, const sf::Texture& tilemap);
    void update(float deltaTime);

    int getLayerID() const;

    void buildVertexArray(
        const LevelMapChunkData& chunkData,
        const sf::Texture& tilemap,
        sf::Vector2i tileSize,
        const std::unordered_map<int, MapTileCollisionData>& tileCollisionData,
        const std::unordered_map<int, AnimatedTileData> &tileAnimationData
    );

    sf::VertexArray m_vertices;
    std::vector<AABB> collisionTiles;
    std::vector<AnimatedTileInstance> animatedTiles;

    sf::Vector2i tileSize;
    int tilesetWidth = 0;

    std::unordered_map<int, AnimatedTileData> tileAnimationData;
    std::unordered_map<int, MapTileCollisionData> tileCollisionData;

    sf::Vector2i chunkSize;

private:
    int id = 0;
    sf::VertexArray vertices;
};

class LoadedMapChunk {
public:
    LoadedMapChunk(sf::Vector2i chunkPosition);

    void render(sf::RenderTarget& target, const sf::Texture& tilemap);
    void update(float deltaTime);

    sf::Vector2i getChunkPosition() const;
    std::vector<AABB>  getCollisionTiles() const;

    std::map<int, LoadedMapLayer> layers;

private:
    sf::Vector2i chunkPosition;
};
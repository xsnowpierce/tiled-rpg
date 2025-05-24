#include "LevelMapData.h"

LevelMapChunkData* LevelMapLayerData::getChunkFromPosition(sf::Vector2i chunkPosition)
{
    for (std::size_t i = 0; i < chunk_data.size(); ++i) {
        if (chunk_data[i].chunkPosition == chunkPosition) {
            return &chunk_data[i];
        }
    }
    return nullptr;
}

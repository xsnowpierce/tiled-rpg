#include "LevelMapLoader.h"
#include <sstream>
#include <istream>

void LevelMapLoader::loadMap(const char* filepath)
{
    loadTileMaps();

    std::cout << "loading map from (" << filepath << ")" << std::endl;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath);

    pugi::xml_node mapNode = doc.child("map");

    currentMap.map_tile_size = sf::Vector2i(mapNode.attribute("width").as_int(), mapNode.attribute("height").as_int());
    currentMap.tile_pixel_size = sf::Vector2i(mapNode.attribute("tilewidth").as_int(), mapNode.attribute("tileheight").as_int());
    currentMap.is_infinite = mapNode.attribute("infinite").as_int() == 1;

    

    for (pugi::xml_node layerNode : mapNode.children("layer")) {
        LevelMapLayerData layer;
        layer.layer_id = layerNode.attribute("id").as_int();
        layer.layer_name = layerNode.attribute("name").as_string();

        pugi::xml_node dataNode = layerNode.child("data");

        for (pugi::xml_node chunkNode : dataNode.children("chunk")) {
            LevelMapChunkData chunkData;

            chunkData.chunkPosition = sf::Vector2i(chunkNode.attribute("x").as_int(), chunkNode.attribute("y").as_int());
            chunkData.chunkSize = sf::Vector2i(chunkNode.attribute("width").as_int(), chunkNode.attribute("height").as_int());

            std::string csvText = chunkNode.text().get();
            std::stringstream strstm(csvText);
            std::string token;
            std::vector<int> tileIDs;

            while (std::getline(strstm, token, ',')) {
                if (!token.empty()) {
                    tileIDs.push_back(std::stoi(token));
                }
            }

            int index = 0;
            for (int y = 0; y < chunkData.chunkSize.y; y++) {
                for (int x = 0; x < chunkData.chunkSize.x; x++) {
                    int tileID = tileIDs[index++];
                    chunkData.chunkData[sf::Vector2i(x, y)] = tileID;
                }
            }

            layer.chunk_data.push_back(chunkData);
        }

        currentMap.layers.push_back(layer);
    }


}

LevelMapLoader::LevelMapLoader(sf::Texture& tile_map) :
    tilemap (tile_map)
{
}

LevelMapLoader::~LevelMapLoader()
{

}

void LevelMapLoader::loadChunk(sf::Vector2i chunkPosition)
{
    LoadedMapChunk chunk(chunkPosition);

    for (int i = 0; i < currentMap.layers.size(); i++) {

        LoadedMapLayer layer(i);


        LevelMapChunkData* chunkData = currentMap.layers[i].getChunkFromPosition(chunkPosition);
        
        if (chunkData == nullptr)
            continue;

        std::cout << "chunk at " << i << ": " << chunkData->chunkPosition.x << ", " << chunkData->chunkPosition.y << std::endl;

        for (int y = 0; y < chunkData->chunkSize.y; y++) {
            for (int x = 0; x < chunkData->chunkSize.x; x++) {

                if (chunkData->chunkData[{x, y}] == 0)
                    continue;

                int tileID = chunkData->chunkData[{x, y}];

                MapTile tile(
                    tilemap,
                    tileID,
                    sf::Vector2f({
                        chunkData->chunkPosition.x * 16.f + (x * 16.f),
                        chunkData->chunkPosition.y * 16.f + (y * 16.f) }),
                    tileCollisionData[tileID],
                    tileAnimationData[tileID]
                );

                tile.chunkPosition = chunkData->chunkPosition;

                sf::Vector2i key = { chunkData->chunkPosition.x + x, chunkData->chunkPosition.y + y };
                auto inserted = layer.tiles.insert({ key, std::move(tile) });
                if (!tileCollisionData[tileID].is_null) {
                    layer.collisionTiles.push_back(&inserted.first->second);
                }

            }
        }

        chunk.layers.insert({ i, std::move(layer) });

    }

    loadedChunks.insert({ chunkPosition, std::move(chunk) });
    
}

void LevelMapLoader::unloadChunk(sf::Vector2i chunkPosition)
{

}

void LevelMapLoader::unloadAllChunks()
{

}

void LevelMapLoader::renderMap(sf::RenderTarget &target)
{
    for (auto it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
        it->second.render(target);
    }
}

LoadedMapChunk* LevelMapLoader::getChunkFromPosition(sf::Vector2i position)
{
    auto it = loadedChunks.find(position);
    if (it != loadedChunks.end()) {
        return &(it->second);
    }
    return nullptr;
}


void LevelMapLoader::loadTileMaps()
{
    pugi::xml_document doc;
    if (!doc.load_file("resources/tiled/tileset.xml")) {
       // std::cout << "Failed to load tileset.xml." << std::endl;
    }

    pugi::xml_node tileset = doc.child("tileset");

    for (pugi::xml_node tileNode : tileset.children("tile")) {

        int tileID = tileNode.attribute("id").as_int() + 1;

        pugi::xml_node objectNode = tileNode.child("objectgroup").child("object");
        pugi::xml_node animationNode = tileNode.child("animation");

        if (objectNode) {
            loadCollisionTile(tileID, objectNode);
        }

        if (animationNode) {
            loadAnimatedTile(tileID, animationNode);
        }
    }

    for (std::pair<int, AnimatedTileData> pair : tileAnimationData) {
        std::cout << "tile " << pair.first << " has " << pair.second.frames.size() << " animated frames." << std::endl;
    }

}

void LevelMapLoader::loadCollisionTile(int tileID, pugi::xml_node objectNode)
{
    MapTileCollisionData collisionData;

    collisionData.boxPosition = { objectNode.attribute("x").as_float(), objectNode.attribute("y").as_float() };
    collisionData.boxSize = { objectNode.attribute("width").as_float(), objectNode.attribute("height").as_float() };
    collisionData.is_null = false;

    tileCollisionData[tileID] = collisionData;
}

void LevelMapLoader::loadAnimatedTile(int tileID, pugi::xml_node animationNode)
{
    AnimatedTileData tileData;
    int frameID = 0;
    for (pugi::xml_node frameNode : animationNode.children()) {
        
        AnimationTileFrame frame;

        frame.tileID = frameNode.attribute("tileid").as_int();
        frame.delayTime = frameNode.attribute("duration").as_int() / 1000.f;

        tileData.frames[frameID] = frame;
        frameID++;
    }
    tileAnimationData[tileID] = tileData;
    std::cout << "gave tile " << tileID << " " << frameID << " tiles." << std::endl;
}

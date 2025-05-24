#include "LevelMapLoader.h"
#include <sstream>
#include <istream>

void LevelMapLoader::loadMap(const char* filepath)
{
    loadTileCollisionMap();

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
    for (int i = 0; i < currentMap.layers.size(); i++) {

        LevelMapChunkData* chunk = currentMap.layers[i].getChunkFromPosition(chunkPosition);
        if (chunk != nullptr) {

            std::unordered_map<sf::Vector2i, std::vector<std::unique_ptr<MapTile>>> tiles;
            std::vector<MapTile*> currentCollisionTiles;

            for (int y = 0; y < chunk->chunkSize.y; y++) {
                for (int x = 0; x < chunk->chunkSize.x; x++) {

                    if (chunk->chunkData[{x, y}] == 0) {
                        continue;
                    }

                    int tileID = chunk->chunkData[{x, y}];

                    std::unique_ptr<MapTile> tile = std::make_unique<MapTile>(
                        tilemap,
                        tileID,
                        sf::Vector2f({ chunk->chunkPosition.x + x * 16.f, chunk->chunkPosition.y + y * 16.f }),
                        tileCollisionData[tileID]
                    );

                    tile->chunkPosition = chunk->chunkPosition;

                    MapTile* rawPtr = tile.get();
                    tiles[{chunk->chunkPosition.x + x, chunk->chunkPosition.y + y}].push_back(std::move(tile));

                    if (!tileCollisionData[tileID].is_null) {
                        currentCollisionTiles.push_back(rawPtr);
;                    }
                }
            }

            auto loadedChunk = std::make_unique<MapChunk>(std::move(tiles), std::move(currentCollisionTiles));
            loadedChunks[chunk->chunkPosition] = std::move(loadedChunk);

        }
    }
}

void LevelMapLoader::unloadChunk(sf::Vector2i chunkPosition)
{
    
}

void LevelMapLoader::unloadAllChunks()
{

}

void LevelMapLoader::renderMap(sf::RenderTarget &target)
{
    for (auto& [position, chunk] : loadedChunks) {
        if (chunk) {
            chunk->render(target);
        }
    }
}

MapChunk* LevelMapLoader::getCurrentChunk()
{
    auto it = loadedChunks.find({ 0, 0 });
    if (it != loadedChunks.end())
        return it->second.get();
    else
        return nullptr;
}


void LevelMapLoader::loadTileCollisionMap()
{
    pugi::xml_document doc;
    if (!doc.load_file("resources/tiled/tileset.xml")) {
       // std::cout << "Failed to load tileset.xml." << std::endl;
    }

    pugi::xml_node tileset = doc.child("tileset");

    for (pugi::xml_node tileNode : tileset.children("tile")) {
        int tileID = tileNode.attribute("id").as_int() + 1;
        //std::cout << "creating collider for tileID " << tileID << std::endl;
        pugi::xml_node objectNode = tileNode.child("objectgroup").child("object");
        if (!objectNode) {
            //std::cout << "skipping tileID " << tileID << std::endl;
            continue;
        }
            

        MapTileCollisionData collisionData;

        collisionData.boxPosition = { objectNode.attribute("x").as_float(), objectNode.attribute("y").as_float() };
        collisionData.boxSize = { objectNode.attribute("width").as_float(), objectNode.attribute("height").as_float() };
        collisionData.is_null = false;

        tileCollisionData[tileID] = collisionData;
    }

}

#include "pch.h"
#include "LevelMapLoader.h"
#include "AnimatedTileData.h"


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
        LevelMapChunkData* chunkData = currentMap.layers[i].getChunkFromPosition(chunkPosition);
        if (!chunkData) continue;

        LoadedMapLayer layer(i);

        for (auto& [tileID, animData] : tileAnimationData) {
            for (auto& frame : animData.frames) {
                int frameTileID = frame.second.tileID + 1;
                int tu = (frameTileID - 1) % 40;
                int tv = (frameTileID - 1) / 40;

                float u0 = float(tu * 16);
                float v0 = float(tv * 16);
                float u1 = float((tu + 1) * 16);
                float v1 = float((tv + 1) * 16);

                frame.second.uvCoords[0] = { u0, v0 };
                frame.second.uvCoords[1] = { u1, v0 };
                frame.second.uvCoords[2] = { u0, v1 };
                frame.second.uvCoords[3] = { u0, v1 };
                frame.second.uvCoords[4] = { u1, v0 };
                frame.second.uvCoords[5] = { u1, v1 };
            }
        }


        layer.buildVertexArray(*chunkData, tilemap, { 16, 16 }, tileCollisionData, tileAnimationData);

        chunk.layers.insert({ i, std::move(layer) });
    }

    loadedChunks.insert({ chunkPosition, std::move(chunk) });
}

void LevelMapLoader::unloadChunk(sf::Vector2i chunkPosition)
{
    loadedChunks.erase(chunkPosition);
}

void LevelMapLoader::unloadAllChunks()
{
    loadedChunks.clear();
}

void LevelMapLoader::renderMap(sf::RenderTarget& target)
{
    for (auto& [pos, chunk] : loadedChunks) {
        chunk.render(target, tilemap);
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

        pugi::xml_node propertiesNode = tileNode.child("properties");
        pugi::xml_node animationNode = tileNode.child("animation");

        if (propertiesNode) {
            loadCollisionTile(tileID, propertiesNode);
        }

        if (animationNode) {
            loadAnimatedTile(tileID, animationNode);
        }
    }

    for (std::pair<int, AnimatedTileData> pair : tileAnimationData) {
        //std::cout << "tile " << pair.first << " has " << pair.second.frames.size() << " animated frames." << std::endl;
    }

}

void LevelMapLoader::loadCollisionTile(int tileID, pugi::xml_node propertiesNode)
{
    MapTileCollisionData collisionData;

    for (pugi::xml_node child : propertiesNode.children()) {
        bool value = child.attribute("value").as_bool();
        if (std::string(child.attribute("name").as_string()) == "collision_top_left") {
            collisionData.collision_top_left = value;
        }
        else if (std::string(child.attribute("name").as_string()) == "collision_top_right") {
            collisionData.collision_top_right = value;
        }
        else if (std::string(child.attribute("name").as_string()) == "collision_bottom_left") {
            collisionData.collision_bottom_left = value;
        }
        else if (std::string(child.attribute("name").as_string()) == "collision_bottom_right") {
            collisionData.collision_bottom_right = value;
        }
    }

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
    //std::cout << "gave tile " << tileID << " " << frameID << " tiles." << std::endl;
}

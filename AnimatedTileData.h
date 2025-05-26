#pragma once

#include <SFML/Graphics.hpp>
#include <map>

class AnimationTileFrame {
public:
	int tileID;
	float delayTime;
};

class AnimatedTileData {
public:
	std::map<int, AnimationTileFrame> frames;
};

class AnimatedTileInstance {
public:

	std::map<int, AnimationTileFrame> frames;

	sf::Vector2i gridPosition;
	sf::Vector2i chunkPosition;
	int baseTileID;
	long vertexIndex;
	float elapsedTime = 0.0f;
	int currentFrame = 0;
};
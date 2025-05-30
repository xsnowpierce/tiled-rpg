#pragma once

#include "pch.h"

class AnimationTileFrame {
public:
	int tileID;
	float delayTime;

	sf::Vector2f uvCoords[6];
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
	int baseTileID = -1;
	long vertexIndex = -1;
	float elapsedTime = 0.0f;
	int currentFrame = 0;

	const AnimatedTileData* animData = nullptr;
};
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


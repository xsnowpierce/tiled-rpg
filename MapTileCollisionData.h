#pragma once
#include "pch.h"

class MapTileCollisionData
{
public:
	bool collision_top_left = false;
	bool collision_top_right = false;
	bool collision_bottom_left = false;
	bool collision_bottom_right = false;
	bool is_null = true;
};


#pragma once

namespace GameSettings {
	
	inline const char* screenName = "Game";
	inline const int screenWidth = 256;
	inline const int screenHeight = 224;

	inline const bool ENABLE_VSYNC = false;
	inline const int MAX_FPS = -1;

	inline const bool PRINT_FPS_TO_CONSOLE = true;

	// warning: will decrease fps by a substantial amount while enabled.
	inline const bool SHOW_DEBUG_COLLIDERS = false;

	inline float DELTA_TIME = 0.0f;
	inline const float TICKS_PER_SECOND = 60.f;

	inline const int SCREEN_TILE_SIZE_W = 16;
	inline const int SCREEN_TILE_SIZE_H = 14;
}
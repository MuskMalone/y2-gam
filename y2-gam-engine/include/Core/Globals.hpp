#pragma once
#include <limits>
#define FLOAT_MAX (std::numeric_limits<float>::max())

#define UNREFERENCED_PARAMETER(P) (P)

//i saw in glfw3.h that the max GLenum goes up to ~380ish
//hence this is gonna be for a bitset
//each key enum will be the bitshift for a bitset<ENGINE_KEYS_COUNT>
#define ENGINE_KEYS_COUNT 400
#define ENGINE_MOUSEKEYS_COUNT 3

//world center is at 0,0
#define WORLD_LIMIT_X 128
#define WORLD_LIMIT_Y 128
//depth is 0 to 256
#define WORLD_LIMIT_DEPTH 256

#define ENGINE_SCREEN_WIDTH 1920
#define ENGINE_SCREEN_HEIGHT 1080
#ifndef __TEXTURES
#define __TEXTURES

#include "../../defines.h"

#include "stb/stb_image.h"

typedef struct __texture {
  u32 idx;
  vec2f coord;
} texture;

typedef struct __texture_map {
  texture *entries;
} texture_map;

u32 load_texture(const char *path);

#endif

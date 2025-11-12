#ifndef __TEXTURES
#define __TEXTURES

#include "../../defines.h"
#include "../../../vendor/stb/stb_image.h"
#include "../utils/dynamic_array.h"
#include "../logger/log.h"


typedef struct __texture {
  char path[256];
  bool repeat;
  u32 idx;
} texture;

u32 load_texture(const char *path);

void free_all_textures();

#endif

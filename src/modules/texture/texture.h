#ifndef __TEXTURES
#define __TEXTURES

#include "../../defines.h"
#include "../../../vendor/stb/stb_image.h"

typedef struct __texture {
  char path[256];
  bool repeat;
  GLuint idx;
} texture;

typedef struct __texture_map {
  texture *entries;
  GLuint *texId;
} texture_map;

u32 load_texture(const char *path);

#endif

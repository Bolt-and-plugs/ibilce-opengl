#ifndef __APP
#define __APP

#include "modules/logger/log.h"
#include "modules/render/render.h"
// #include "modules/shaders/shaders.h"
#include "modules/texture/texture.h"
#include "modules/gl_draw/gl_draw.h"

#include "GL/freeglut.h"

typedef struct __player {
  float x, y, z;
} Player ;

typedef struct __App {
  const char *name;
  bool debug;
  log_level min_log_level;
  Player player;
  // shader_table st;
} App;


int main(int argc, char **argv);

#endif

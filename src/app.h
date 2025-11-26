#ifndef __APP
#define __APP

#include "modules/logger/log.h"
#include "stdbool.h"
#include "defines.h"

typedef struct __player {
  float x, y, z;
} Player ;

typedef struct __App {
  const char *name;
  bool debug;
  log_level min_log_level;
  Player player;
  int window_height, window_width;
  // shader_table st;
  // ambient lighitng
  GLfloat ambient_light[4];
  GLfloat diffuse_light[4];
  GLfloat light_pos[4];
  GLfloat specular_light[4];
  GLfloat specularity[4];
  
} App;


int main(int argc, char **argv);

#endif

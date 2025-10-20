#ifndef __APP
#define __APP

#include "modules/logger/log.h"
#include "modules/render/render.h"
#include "modules/shaders/shaders.h"
#include "modules/texture/texture.h"

typedef struct __App {
  const char *name;
  bool debug;
  log_level min_log_level;
  shader_table st;
} App;


int main(int argc, char **argv);

#endif

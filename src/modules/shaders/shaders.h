#ifndef _SHADERS
#define _SHADERS

#include "../../defines.h"

#define MAX_SHADER_NAME 64
#define MAX_SHADER_NUM 64

typedef struct __shader {
  GLuint shader;
  GLenum type;
  char name[MAX_SHADER_NAME];
  char *buffer;
} shader;

typedef struct __shader_table {
  shader *entries[MAX_SHADER_NUM];
  int len;
  GLuint program;
} shader_table;

void init_shader_table();

#endif

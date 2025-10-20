#ifndef _VERTEX
#define _VERTEX

typedef struct _vec2 {
  float x, y;
} vec2f;

typedef struct _vec3 {
  float x, y, z;
} vec3f;

typedef struct _vec4 {
  float x, y, z;
} vec4f;

typedef struct __Vertex2f {
  vec2f pos;
} Vertex2f;

typedef struct __Vertex3f {
  vec3f pos;
} Vertex3f;

#endif

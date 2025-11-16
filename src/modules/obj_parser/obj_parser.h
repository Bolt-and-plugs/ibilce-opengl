#ifndef _OBJ
#define _OBJ

#include "../texture/texture.h"

#define DEFAULT_BUFFER_SIZE 128

typedef struct __Material {
  char name[128];
  u32 texture_id;
  Color diffuse;
} Material;

typedef struct __MeshGroup {
  char name[128];
  DynamicArray *faces;
  Material *material;
} MeshGroup;

typedef struct __face_idx {
  int vertex_index;
  int texture_index;
  int normal_index;
} face_idx;

typedef struct __face {
  face_idx corners[4];
  int corner_count;
} face;

typedef struct __obj {
  DynamicArray *vertecies; // v -> vertices (x, y, z, (w - optional))
  DynamicArray *normals;   // vn -> List of vertex normals in (x,y,z) form
  DynamicArray *textures;  // vt ->textures coord (u, [v, w])
  DynamicArray *mesh_groups; // groups of faces with same material
  DynamicArray *materials; // list of materials
  texture texture;
} obj;

#define Model obj

obj *load_model(const char *path);
void free_model(obj *o);

#endif

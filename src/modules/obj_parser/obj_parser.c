// Note: this is not a real parser, probably it will fail for different obj
// Im onlying writing this to have some fun
#include "obj_parser.h"
#include "../logger/log.h"

static obj *init_obj() {
  obj *o = malloc(sizeof(obj));
  o->vertecies = darray_create(sizeof(vec3f), DEFAULT_BUFFER_SIZE);
  o->textures = darray_create(sizeof(vec2f), DEFAULT_BUFFER_SIZE);
  o->normals = darray_create(sizeof(vec3f), DEFAULT_BUFFER_SIZE);
  o->faces = darray_create(sizeof(face), DEFAULT_BUFFER_SIZE);
  return o;
}

obj *load_model(const char *path) {
  c_info("Loading model from path %s", path);

  FILE *fp = fopen(path, "r");
  if (!fp) {
    c_error(FILE_OPEN_FAILURE, "Could not open file");
    return NULL;
  }

  obj *obj = init_obj();

  char line_buffer[DEFAULT_BUFFER_SIZE];
  while (fgets(line_buffer, sizeof(line_buffer), fp)) {
    // vertex
    if (strncmp(line_buffer, "v ", 2) == 0) {
      vec3f v;
      sscanf(line_buffer, "v %f %f %f\n", &v.x, &v.y, &v.z);
      darray_push(obj->vertecies, &v);
    }
    // vertex texture
    else if (strncmp(line_buffer, "vt ", 3) == 0) {
      vec2f vt;
      sscanf(line_buffer, "vt %f %f\n", &vt.x, &vt.y);
      darray_push(obj->textures, &vt);
    }
    // vertex normal
    else if (strncmp(line_buffer, "vn ", 3) == 0) {
      vec3f n;
      sscanf(line_buffer, "%f %f %f\n", &n.x, &n.y, &n.z);
      darray_push(obj->normals, &n);
    }
    // face
    else if (strncmp(line_buffer, "f ", 2) == 0) {
      face f;
      int backslash_count = 0;
      for (size_t i = 0; i < sizeof(line_buffer); i++) {
        if (line_buffer[i] == '\\') {
          backslash_count++;
        }
      }

      // triangle face
      if (backslash_count == 3 * 2)
        f.corner_count = 3;

      // square face
      if (backslash_count == 4 * 2)
        f.corner_count = 4;

      for (int i = 0; i < f.corner_count; i++)
        sscanf(line_buffer, "f %d/%d/%d\n", &f.corners[i].vertex_index,
               &f.corners[i].normal_index, &f.corners[i].texture_index);

      darray_push(obj->faces, &f);
    }
  }

  c_info("Loaded model");
  return obj;
}

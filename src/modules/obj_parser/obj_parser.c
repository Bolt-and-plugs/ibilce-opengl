#include "obj_parser.h"
#include "../logger/log.h"
#include "../texture/texture.h"
#include <string.h> 

static obj *init_obj() {
  obj *o = malloc(sizeof(obj));
  if (!o) return NULL;
  o->vertecies = darray_create(sizeof(vec3f), DEFAULT_BUFFER_SIZE);
  o->textures = darray_create(sizeof(vec2f), DEFAULT_BUFFER_SIZE);
  o->normals = darray_create(sizeof(vec3f), DEFAULT_BUFFER_SIZE);
  o->mesh_groups = darray_create(sizeof(MeshGroup), DEFAULT_BUFFER_SIZE);
  o->materials = darray_create(sizeof(Material), DEFAULT_BUFFER_SIZE); 
  return o;
}

void free_model(obj *o) {
  if (!o) return;

  MeshGroup group;
  for (int i = 0; i < o->mesh_groups->size; i++) { 
      if (darray_get(o->mesh_groups, i, &group) == 0) { 
        darray_destroy(group.faces);
      }
  }

  darray_destroy(o->vertecies);
  darray_destroy(o->textures);
  darray_destroy(o->normals);
  darray_destroy(o->mesh_groups);
  darray_destroy(o->materials);
  free(o);
}


static void get_base_path(const char* path, char* base_path) {
  const char* last_slash = strrchr(path, '/');
  if (last_slash == NULL) {
    last_slash = strrchr(path, '\\');
  }

  if (last_slash != NULL) {
    int len = (last_slash - path) + 1;
    strncpy(base_path, path, len);
    base_path[len] = '\0';
  } else {
    base_path[0] = '\0'; 
  }
}

static void load_materials(obj* obj, const char* mtl_path) {
  c_info("Carregando materiais de %s", mtl_path);
  FILE* fp = fopen(mtl_path, "r");
  if (!fp) {
    c_error(FILE_OPEN_FAILURE, "Não foi possível abrir o arquivo .mtl");
    return;
  }

  char line_buffer[DEFAULT_BUFFER_SIZE];
  Material current_material = {0};
  bool material_pending = false;

  char mtl_base_path[DEFAULT_BUFFER_SIZE];
  get_base_path(mtl_path, mtl_base_path);

  while (fgets(line_buffer, sizeof(line_buffer), fp)) {
    if (strncmp(line_buffer, "newmtl ", 7) == 0) {
      if (material_pending) {
        darray_push(obj->materials, &current_material);
      }
      memset(&current_material, 0, sizeof(Material));
      sscanf(line_buffer, "newmtl %s", current_material.name);
      material_pending = true;
    }
    else if (strncmp(line_buffer, "map_Kd ", 7) == 0 && material_pending) {
      char texture_file[DEFAULT_BUFFER_SIZE];
      sscanf(line_buffer, "map_Kd %s", texture_file);

      char texture_full_path[DEFAULT_BUFFER_SIZE * 2];
      if (texture_file[0] == '/' || texture_file[1] == ':') { 
        snprintf(texture_full_path, sizeof(texture_full_path), "%s", texture_file);
      } else {
        snprintf(texture_full_path, sizeof(texture_full_path), "%s%s", mtl_base_path, texture_file);
      }

      current_material.texture_id = load_texture(texture_full_path);
    }
  }

  if (material_pending) {
    darray_push(obj->materials, &current_material);
  }

  fclose(fp);
  c_info("Materiais carregados: %d", obj->materials->size);
}

obj *load_model(const char *path) {
  c_info("Loading model from path %s", path);

  FILE *fp = fopen(path, "r");
  if (!fp) {
    c_error(FILE_OPEN_FAILURE, "Could not open file");
    return NULL;
  }

  obj *obj = init_obj();
  if (!obj) {
    c_error(MEMORY_ALLOC_ERROR, "Could not init obj");
    fclose(fp);
    return NULL;
  }

  char base_path[DEFAULT_BUFFER_SIZE];
  get_base_path(path, base_path);

  char line_buffer[DEFAULT_BUFFER_SIZE];

  MeshGroup* current_group = NULL;

  while (fgets(line_buffer, sizeof(line_buffer), fp)) {
    // vertex
    if (strncmp(line_buffer, "v ", 2) == 0) {
      vec3f v;
      sscanf(line_buffer, "v %f %f %f", &v.x, &v.y, &v.z);
      darray_push(obj->vertecies, &v);
    }
    // vertex texture
    else if (strncmp(line_buffer, "vt ", 3) == 0) {
      vec2f vt;
      sscanf(line_buffer, "vt %f %f", &vt.x, &vt.y);
      darray_push(obj->textures, &vt);
    }
    // vertex normal
    else if (strncmp(line_buffer, "vn ", 3) == 0) {
      vec3f n;
      sscanf(line_buffer, "vn %f %f %f", &n.x, &n.y, &n.z);
      darray_push(obj->normals, &n);
    }
    // Carregar biblioteca de material
    else if (strncmp(line_buffer, "mtllib ", 7) == 0) {
      char mtl_filename[DEFAULT_BUFFER_SIZE];
      sscanf(line_buffer, "mtllib %s", mtl_filename);

      char mtl_full_path[DEFAULT_BUFFER_SIZE * 2];
      snprintf(mtl_full_path, sizeof(mtl_full_path), "%s%s", base_path, mtl_filename);

      load_materials(obj, mtl_full_path);
    }
    // Usar material
    else if (strncmp(line_buffer, "usemtl ", 7) == 0) {
      char material_name[128];
      sscanf(line_buffer, "usemtl %s", material_name);

      Material* found_material = NULL;
      Material* materials_array = (Material*)obj->materials->data;
      for (int i = 0; i < obj->materials->size; i++) {
        if (strcmp(materials_array[i].name, material_name) == 0) {
          found_material = &materials_array[i]; 
          break;
        }
      }

      MeshGroup new_group = {0};
      strncpy(new_group.name, material_name, 128);
      new_group.material = found_material;
      new_group.faces = darray_create(sizeof(face), DEFAULT_BUFFER_SIZE);

      darray_push(obj->mesh_groups, &new_group);

      current_group = (MeshGroup*)((char*)obj->mesh_groups->data +
        (obj->mesh_groups->size - 1) * obj->mesh_groups->element_size);
    }
    // face
    else if (strncmp(line_buffer, "f ", 2) == 0) {
      if (current_group == NULL) {
        c_info("Nenhum material, criando grupo padrão.");
        MeshGroup default_group = {0};
        strcpy(default_group.name, "default_group");
        default_group.material = NULL;
        default_group.faces = darray_create(sizeof(face), DEFAULT_BUFFER_SIZE);
        darray_push(obj->mesh_groups, &default_group);
        current_group = (MeshGroup*)((char*)obj->mesh_groups->data +
          (obj->mesh_groups->size - 1) * obj->mesh_groups->element_size);
      }

      face temp_face_data;
      int items_read = sscanf(line_buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                              &temp_face_data.corners[0].vertex_index, &temp_face_data.corners[0].texture_index, &temp_face_data.corners[0].normal_index,
                              &temp_face_data.corners[1].vertex_index, &temp_face_data.corners[1].texture_index, &temp_face_data.corners[1].normal_index,
                              &temp_face_data.corners[2].vertex_index, &temp_face_data.corners[2].texture_index, &temp_face_data.corners[2].normal_index,
                              &temp_face_data.corners[3].vertex_index, &temp_face_data.corners[3].texture_index, &temp_face_data.corners[3].normal_index);

      if (items_read == 9) {
        temp_face_data.corner_count = 3;
        darray_push(current_group->faces, &temp_face_data); 

      } else if (items_read == 12) {
        face tri1;
        tri1.corner_count = 3;
        tri1.corners[0] = temp_face_data.corners[0];
        tri1.corners[1] = temp_face_data.corners[1];
        tri1.corners[2] = temp_face_data.corners[2];
        darray_push(current_group->faces, &tri1);

        face tri2;
        tri2.corner_count = 3;
        tri2.corners[0] = temp_face_data.corners[0];
        tri2.corners[1] = temp_face_data.corners[2];
        tri2.corners[2] = temp_face_data.corners[3];
        darray_push(current_group->faces, &tri2);
      }
    }
  }

  fclose(fp);

  int total_faces = 0;
  MeshGroup* groups_array = (MeshGroup*)obj->mesh_groups->data;
  for(int i = 0; i < obj->mesh_groups->size; i++) {
    total_faces += groups_array[i].faces->size;
  }

  c_info("Loaded model (%d vertices, %d groups, %d total faces)", obj->vertecies->size, obj->mesh_groups->size, total_faces);
  return obj;
}

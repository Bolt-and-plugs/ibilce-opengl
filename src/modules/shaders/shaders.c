#include "shaders.h"
#include "../../app.h"
#include "../logger/log.h"

extern App app;

char *shader_list[] = {"assets/shaders/default.vert", "assets/shaders/default.frag"};
size_t shader_len = sizeof(shader_list) / sizeof(shader_list[0]);

static char *read_shader_file(const char *filename) {
  FILE *file = fopen(filename, "rb");

  if (!file) {
    c_error(SHADER_LOAD_ERROR, "Failed to open shader file: %s\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(length + 1);
  if (!buffer) {
    c_error(SHADER_LOAD_ERROR, "Failed to allocate memory for shader source\n");
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0';
  fclose(file);
  return buffer;
}

static GLuint compile_shader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    c_error(SHADER_COMPILE_ERROR, "Shader compilation failed:\n%s\n", infoLog);
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

static GLuint create_shader_program() {
  GLuint program = glCreateProgram();

  for (int i = 0; i < app.st.len; i++) {
    glAttachShader(program, app.st.entries[i]->shader);
  }

  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    c_error(SHADER_PROGRAM_CREATION_ERROR,
            "Shader program linking failed:\n%s\n", infoLog);
    glDeleteProgram(program);
    return 0;
  }

  c_info("Shader Program created");
  return program;
}

shader *get_shader_from_filename(const char *filename) {
  if (filename == NULL) {
    c_error(SHADER_LOAD_ERROR, "Filename cannot be null.");
    return NULL;
  }
  const char *dot = strrchr(filename, '.');
  if (dot == NULL || dot == filename) {
    c_error(SHADER_LOAD_ERROR, "No valid extension found in filename: %s",
            filename);
    return NULL;
  }

  const char *extension = dot + 1;

  shader *s = malloc(sizeof(shader));
  if (s == NULL) {
    return NULL;
  }

  strncpy(s->name, filename, sizeof(s->name) - 1);
  s->name[sizeof(s->name) - 1] = '\0';

  if (strcmp(extension, "vert") == 0) {
    s->type = GL_VERTEX_SHADER;
  } else if (strcmp(extension, "frag") == 0) {
    s->type = GL_FRAGMENT_SHADER;
  } else if (strcmp(extension, "geom") == 0) {
    s->type = GL_GEOMETRY_SHADER;
  } else {
    c_error(SHADER_LOAD_ERROR, "Unknown shader type: %s", extension);
    free(s);
    return NULL;
  }

  c_info("Loaded shader '%s' as type '%s'", s->name, extension);

  char *buffer = read_shader_file(s->name);

  if (!buffer) {
    c_error(SHADER_LOAD_ERROR, "Could not read shader file");
    return NULL;
  }

  s->buffer = buffer;
  return s;
}
void print_shader(shader *s) { c_info("%s %d", s->name, s->shader); }

void print_all_shaders() {
  for (int i = 0; i < app.st.len; i++)
    print_shader(app.st.entries[i]);
}

bool insert_into_shader_table(shader *s) {
  app.st.entries[app.st.len++] = s;
  return true;
}

bool load_shaders() {
  for (size_t i = 0; i < shader_len; i++) {
    shader *s = get_shader_from_filename(shader_list[i]);
    s->shader = compile_shader(s->type, s->buffer);
    insert_into_shader_table(s);
  }
  return true;
}

void init_shader_table() {
  c_info("Initializing Shader Table");
  app.st.len = 0;
  if (!load_shaders()) {
    c_error(SHADER_LOAD_ERROR,
            "Could not initialize shader table with %d entries", shader_len);
  };

  if (app.debug)
    print_all_shaders();

  app.st.program = create_shader_program();
}

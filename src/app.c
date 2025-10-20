#include "app.h"

App app;

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    app.debug = true;
    return true;
  }

  app.debug = false;
  return false;
}

void set_debug_mode() {
//  set debug mode
#ifdef BUILD_TYPE
  bool debug = set_envvar(BUILD_TYPE);
  if (debug)
    c_info("Debug mode set");
#endif
  app.min_log_level = get_min_log_level();
}

void init_app() {
  set_debug_mode();

  char *name = malloc(32);
  strcpy(name, "Titulo");
  app.name = name;
}

void clean_app() {
  glfwDestroyWindow(app.window.glfw_window);
  glfwTerminate();
}

int main(int argc, char **argv) {
  init_app();
  init_window(600, 400);
  init_shader_table();

  Vertex3f vertex[4][3] = {
      {{0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}},
      {{0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}},
      {{0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}},
      {{0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}, {0.45f, .1f, 0.1f}}};

  // todo remove this VBO (vertex buffer obj) and VAO (vertex array) shit
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) / sizeof(Vertex3f), vertex,
               GL_STATIC_DRAW);

  const GLint vpos_location = glGetAttribLocation(app.st.program, "aPos");

  const GLint vtexture_location =
      glGetAttribLocation(app.st.program, "aTexCoord");

  if (vpos_location == -1)
    c_error(SHADER_LOAD_ERROR, "Could not find attribute 'aPos' in shader!\n");

  if (vtexture_location == -1)
    c_error(SHADER_LOAD_ERROR, "Could not find attribute 'aTex' in shader!\n");

  // Tell OpenGL how to interpret the vertex data in the VBO.
  glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3f),
                        (void *)offsetof(Vertex3f, pos));
  glEnableVertexAttribArray(vpos_location);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  while (!glfwWindowShouldClose(app.window.glfw_window)) {
    int width, height;
    glfwGetFramebufferSize(app.window.glfw_window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(app.st.program);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(app.window.glfw_window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  clean_app();
  return 0;
}

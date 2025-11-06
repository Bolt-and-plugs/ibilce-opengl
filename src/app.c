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
}

void init_render() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void init_window(int width, int height) {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow(app.name);

  init_render();
}

int main(int argc, char **argv) {
  init_app();
  init_window(600, 400);


  glutMainLoop();
  clean_app();
  return 0;
}

#include "window.h"
#include "../../app.h"

extern App app;

void window_focus_callback(GLFWwindow *window, int focused) {
  if (focused) {
    // The window gained input focus
  } else {
    // The window lost input focus
  }
}

static void window_key_callback(GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool init_window(int width, int height) {
  if (!glfwInit()) {
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *w = glfwCreateWindow(width, height, app.name, NULL, NULL);

  if (!w) {
    glfwTerminate();
    c_crit_error(WINDOW_INITIALIZATION_ERROR, "Could not init window\n");
  }

  glfwMakeContextCurrent(w);

  glfwSetKeyCallback(w, window_key_callback);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    c_error(OPENGL_LOAD_ERROR, "Failed to initialize OpenGL context\n");
    glfwDestroyWindow(w);
    glfwTerminate();
    return false;
  }

  c_info("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));

  app.window.glfw_window = w;
  return true;
}

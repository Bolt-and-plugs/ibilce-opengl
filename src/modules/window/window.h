#ifndef _WINDOW
#define _WINDOW

#include "../../defines.h"
#include "../render/render.h"

typedef struct __window {
  GLFWwindow *glfw_window;
} window;

bool init_window(int x, int y);

#endif

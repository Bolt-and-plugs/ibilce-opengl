#include "app.h"
#include "modules/obj_parser/obj_parser.h"
#include <GL/gl.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

App app;

Model* cube = NULL;
u32 tex_sky = 0;
u32 tex_grass = 0;
u32 tex_tree = 0;


float rad(float angle) { return angle * M_PI / 180.0f; }

vec3f cross(vec3f v1, vec3f v2) {
  return (vec3f){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                 v1.x * v2.y - v1.y * v2.x};
}

vec3f normalize(vec3f v) {
  float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  if (mag == 0.0f)
    return (vec3f){0, 0, 0};
  return (vec3f){v.x / mag, v.y / mag, v.z / mag};
}

vec3f cameraPos = {0.0f, 0.5f, 5.0f};
vec3f cameraFront = {0.0f, 0.0f, -1.0f};
vec3f cameraUp = {0.0f, 1.0f, 0.0f};

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 300.0f;
float lastY = 200.0f;
bool firstMouse = true;

float cameraSpeed = 2.5f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool key_w = false;
bool key_s = false;
bool key_a = false;
bool key_d = false;
bool key_space = false;
bool key_f = false;

bool set_envvar(const char *mode) {
  if (strcmp(mode, "Debug") == 0 || strcmp(mode, "DEBUG") == 0) {
    app.debug = true;
    return true;
  }
  app.debug = false;
  return false;
}

void set_debug_mode() {
#ifdef BUILD_TYPE
  bool debug = set_envvar(BUILD_TYPE);
  if (debug)
    c_info("Debug mode set");
#endif
  app.min_log_level = get_min_log_level();
}


void init_textures() {
  c_info("Carregando texturas do mundo...");
  tex_sky   = load_texture("assets/textures/skybox.jpg");
  tex_grass = load_texture("assets/textures/grass.jpg");
  tex_tree  = load_texture("assets/textures/tree.png");
}

void init_app() {
  set_debug_mode();
  app.name = "Trabalho CG";
}

void clean_app() {
  c_info("Cleaning up application...");
  if (cube) {
    free_model(cube);
  }
  free_all_textures();
  exit(0);
}

void HandleActiveMouseMotion(int x, int y) {
}

void HandlePassiveMouseMotion(int x, int y) {
    int centerX = app.window_width / 2;
    int centerY = app.window_height / 2;

    // Ignore warp events — GLUT sends one after glutWarpPointer()
    if (x == centerX && y == centerY) return;

    float xoffset = x - centerX;
    float yoffset = centerY - y; 

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    vec3f front;
    front.x = cos(rad(yaw)) * cos(rad(pitch));
    front.y = sin(rad(pitch));
    front.z = sin(rad(yaw)) * cos(rad(pitch));
    cameraFront = normalize(front);

    glutWarpPointer(centerX, centerY);
}


void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Handle left click
  };
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    // Handle right click
  };
};

void keyboard(unsigned char key, int x, int y) {
  if (key == 27) { // ESC
    clean_app();
  };

  if (key == 'w' || key == 'W') {
    key_w = true;
  };
  if (key == 's' || key == 'S') {
    key_s = true;
  };
  if (key == 'a' || key == 'A') {
    key_a = true;
  };
  if (key == 'd' || key == 'D') {
    key_d = true;
  };
  if (key == ' ') {
    key_space = true;
  };
  if (key == 'f' || key == 'F') {
    key_f = true;
  };

  if (key == 'e' || key == 'E') {
    // Handle 'E'
  };

};

void keyboardUp(unsigned char key, int x, int y) {
  if (key == 'w' || key == 'W') {
    key_w = false;
  };
  if (key == 's' || key == 'S') {
    key_s = false;
  };
  if (key == 'a' || key == 'A') {
    key_a = false;
  };
  if (key == 'd' || key == 'D') {
    key_d = false;
  };
  if (key == ' ') {
    key_space = false;
  };
  if (key == 'f' || key == 'F') {
    key_f = false;
  };
}

void TeclasEspeciais(int key, int x, int y) {
  if (key == GLUT_KEY_UP) {
    // Handle up arrow
  };
};

void process_input() {
 float speed = cameraSpeed * deltaTime;
  vec3f worldUp = {0.0f, 1.0f, 0.0f};
  vec3f cameraRight = normalize(cross(cameraFront, worldUp));

  if (key_w) {
    cameraPos.x += cameraFront.x * speed;
    cameraPos.z += cameraFront.z * speed;
  }
  if (key_s) {
    cameraPos.x -= cameraFront.x * speed;
    cameraPos.z -= cameraFront.z * speed;
  }
  if (key_a) {
    cameraPos.x -= cameraRight.x * speed;
    cameraPos.z -= cameraRight.z * speed;
  }
  if (key_d) {
    cameraPos.x += cameraRight.x * speed;
    cameraPos.z += cameraRight.z * speed;
  }
  if (key_space) {
    cameraPos.y += speed;
  }
  if (key_f) {
    cameraPos.y -= speed;
  }
}

void render_scene() {
  float currentFrame = glutGet(GLUT_ELAPSED_TIME);
  deltaTime = (currentFrame - lastFrame) / 1000.0f;
  lastFrame = currentFrame;

  process_input();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  vec3f center = {cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y,
                  cameraPos.z + cameraFront.z};
  gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 
            center.x, center.y, center.z,          
            cameraUp.x, cameraUp.y, cameraUp.z);   


  // draw
  draw_skybox(50.0f, tex_sky, cameraPos, (Color){1.0f, 1.0f, 1.0f, 1.0f});


  draw_model(cube, (vec3f){0.0f, 0.0f, -3.0f}, (vec3f){0.0f, 45.0f, 0.0f},
             (vec3f){1.0f, 1.0f, 1.0f});

  glutSwapBuffers();
}

void init_render() {
  c_info("Initializing rendering context...");
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutDisplayFunc(render_scene);
  glutIdleFunc(render_scene); 
}

void init_window(int width, int height) {
  c_info("Initializing window...");
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  app.window_width = width;
  app.window_height = height;
  glutCreateWindow(app.name);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);

  init_render();

  glutMouseFunc(Mouse);
  glutMotionFunc(HandleActiveMouseMotion);
  glutPassiveMotionFunc(HandlePassiveMouseMotion);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(TeclasEspeciais);
  glutKeyboardUpFunc(keyboardUp);
  glutWarpPointer(width / 2, height / 2);
  glutSetCursor(GLUT_CURSOR_NONE);
}

int main(int argc, char **argv) {
  init_app();
  glutInit(&argc, argv);
  init_window(600, 400);
  init_textures();

  cube = load_model("assets/models/Untitled.obj");

  lastFrame = glutGet(GLUT_ELAPSED_TIME);
  glutMainLoop();

  clean_app();
  return 0;
}

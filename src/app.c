#include "app.h"
#include <GL/glut.h> // Make sure glut is included
#include <math.h>    // === NEW === (for sin, cos)
#include <stdbool.h> // === NEW === (for bool)
#include <stdio.h>   // === NEW === (for printf in Mouse func)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

App app;

// === NEW: Helper functions for vector math ===
float rad(float angle) {
  return angle * M_PI / 180.0f;
}

vec3f cross(vec3f v1, vec3f v2) {
  return (vec3f){
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x};
}

vec3f normalize(vec3f v) {
  float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  if (mag == 0.0f) return (vec3f){0,0,0};
  return (vec3f){v.x / mag, v.y / mag, v.z / mag};
}

// === NEW: Camera and Time State ===
vec3f cameraPos = {0.0f, 0.5f, 3.0f}; // Start position
vec3f cameraFront = {0.0f, 0.0f, -1.0f}; // Looking down -Z
vec3f cameraUp = {0.0f, 1.0f, 0.0f};

float yaw = -90.0f; // Yaw is initialized to look down -Z
float pitch = 0.0f;
float lastX = 300.0f; // Initial mouse position (center of 600)
float lastY = 200.0f; // Initial mouse position (center of 400)
bool firstMouse = true;

float cameraSpeed = 0.005f; // Base speed
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- App Functions (Unchanged) ---
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

void init_app() {
  set_debug_mode();
  app.name = "Trabalho CG";
}

void clean_app() {
  c_info("Cleaning up application...");
  exit(0); // Use exit(0) for a clean exit
}

// --- Input Handlers ---

// Handles mouse movement when a button is HELD DOWN (dragging)
void HandleActiveMouseMotion(int x, int y) {
  // 'x' and 'y' are the new mouse coordinates
  // Useful for click-and-drag actions
  // (We'll use Passive for camera)
}

// === UPDATED: Handles mouse movement when NO button is pressed ===
void HandlePassiveMouseMotion(int x, int y) {
  if (firstMouse) {
    lastX = (float)x;
    lastY = (float)y;
    firstMouse = false;
    return;
  }

  float xoffset = (float)x - lastX;
  float yoffset = lastY - (float)y; // Reversed: y-coords go from top to bottom
  lastX = (float)x;
  lastY = (float)y;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Clamp pitch to prevent screen flipping
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  // Recalculate cameraFront vector
  vec3f front;
  front.x = cos(rad(yaw)) * cos(rad(pitch));
  front.y = sin(rad(pitch));
  front.z = sin(rad(yaw)) * cos(rad(pitch));
  cameraFront = normalize(front);
}

// === UPDATED: Mouse button and scroll wheel ===
void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Handle left mouse button click
  };

  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    // Handle right mouse button click
  };

  // === NEW: Scroll wheel controls movement speed ===
  if (button == 3) { // Scroll Up
    cameraSpeed += 0.001f;
    if (cameraSpeed > 0.1f) cameraSpeed = 0.1f;
    // printf("Camera speed: %.3f\n", cameraSpeed);
  };
  if (button == 4) { // Scroll Down
    cameraSpeed -= 0.001f;
    if (cameraSpeed < 0.001f) cameraSpeed = 0.001f;
    // printf("Camera speed: %.3f\n", cameraSpeed);
  };
};

// === UPDATED: Keyboard handler ===
void keyboard(unsigned char key, int x, int y) {
  float speed = cameraSpeed * deltaTime; // Apply delta time
  
  // Calculate right vector (for strafing)
  // We use a "world up" vector for the cross product to keep movement horizontal
  vec3f worldUp = {0.0f, 1.0f, 0.0f};
  vec3f cameraRight = normalize(cross(cameraFront, worldUp));

  if (key == 27) { // Handle ESC key press
    clean_app();
  };

  if (key == 'w' || key == 'W') { // Move forward (horizontal)
    cameraPos.x += cameraFront.x * speed;
    cameraPos.z += cameraFront.z * speed;
  };

  if (key == 's' || key == 'S') { // Move backward (horizontal)
    cameraPos.x -= cameraFront.x * speed;
    cameraPos.z -= cameraFront.z * speed;
  };

  if (key == 'a' || key == 'A') { // Strafe left
    cameraPos.x -= cameraRight.x * speed;
    cameraPos.z -= cameraRight.z * speed;
  };

  if (key == 'd' || key == 'D') { // Strafe right
    cameraPos.x += cameraRight.x * speed;
    cameraPos.z += cameraRight.z * speed;
  };

  if (key == ' ') { // Move up
    cameraPos.y += speed;
  };

  if (key == 'f' || key == 'F') { // Move down
    cameraPos.y -= speed;
  };

  if (key == 'e' || key == 'E') {
    // Handle 'E' key press
  };
};

void TeclasEspeciais(int key, int x, int y) {
  if (key == GLUT_KEY_UP) {
    // Handle up arrow key press
  };
  // ... rest of your arrow key logic
};

// === UPDATED: Render scene with new camera ===
void render_scene() {
  // === NEW: DeltaTime Calculation ===
  float currentFrame = glutGet(GLUT_ELAPSED_TIME);
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // === UPDATED: Use gluLookAt for camera ===
  // Calculate the "look at" point
  vec3f center = {
      cameraPos.x + cameraFront.x,
      cameraPos.y + cameraFront.y,
      cameraPos.z + cameraFront.z
  };
  // Set the camera view
  gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, // Eye/Camera position
            center.x, center.y, center.z,        // Look-at point
            cameraUp.x, cameraUp.y, cameraUp.z); // Up vector

  // --- Your drawing code ---
  Color red = {1.0f, 0.0f, 0.0f, 1.0f};
  draw_floor((vec3f){-2.0f, -2.0f, 0.0f}, (vec3f){2.0f, 2.0f, 0.0f},
             (Color){0.3f, 0.3f, 0.3f, 1.0f}, 0);
  draw_wall((vec3f){-2.0f, -2.0f, 0.0f}, (vec3f){-2.0f, 2.0f, 0.0f}, 2.0f,
            (Color){0.6f, 0.4f, 0.2f, 1.0f}, 0, DRAW_CUBE_SOLID);
  draw_ceiling((vec3f){-2.0f, -2.0f, 0.0f}, (vec3f){2.0f, 2.0f, 0.0f}, 2.0f,
             (Color){0.3f, 0.3f, 0.3f, 1.0f}, 0); // Note: this ceiling is at z=0, might be a bug
  draw_cube((vec3f){0.0f, 0.0f, 0.5f}, 1.0f, red, 0, DRAW_CUBE_SOLID);
  
  // === UPDATED: Fixed draw_skybox call (removed extra color argument) ===
  // The skybox function from the previous chat did not take a color
  draw_skybox(50.0f, 0, cameraPos, (Color){0.5f, 0.7f, 1.0f, 1.0f});

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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // draws
  glutDisplayFunc(render_scene);
  glutIdleFunc(render_scene); // This makes render_scene the main loop
}

// === UPDATED: Window initiation ===
void init_window(int width, int height) {
  c_info("Initializing window...");
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow(app.name);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);

  init_render();

  // === UPDATED: Register ALL input handlers ===
  glutMouseFunc(Mouse);
  glutMotionFunc(HandleActiveMouseMotion);
  glutPassiveMotionFunc(HandlePassiveMouseMotion);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(TeclasEspeciais); 
  
  glutSetCursor(GLUT_CURSOR_NONE); 
}

int main(int argc, char **argv) {
  init_app();
  glutInit(&argc, argv);
  init_window(600, 400);

  lastFrame = glutGet(GLUT_ELAPSED_TIME); // === NEW: Init frame time ===
  glutMainLoop();
  
  clean_app(); // This is rarely reached, but good practice
  return 0;
}

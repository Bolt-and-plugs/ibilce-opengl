#include "app.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

App app;

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

// === Estado da Câmera e Tempo ===
vec3f cameraPos = {0.0f, 0.5f, 5.0f};
vec3f cameraFront = {0.0f, 0.0f, -1.0f};
vec3f cameraUp = {0.0f, 1.0f, 0.0f};

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 300.0f;
float lastY = 200.0f;
bool firstMouse = true;

float cameraSpeed = 0.005f;
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

GLuint texId[TEXTURE_COUNT];

const char *textures[] = {
    "assets/textures/skybox.jpg",
    "assets/textures/grass.jpg",
};

void loadTexture(const char *filename, GLuint texID) {
  int w, h, channels;
  unsigned char *data =
      stbi_load(filename, &w, &h, &channels, 0); // Carrega a imagem do arquivo
  if (!data) {
    printf("Erro ao carregar %s\n", filename);
    exit(1);
  }

  glBindTexture(GL_TEXTURE_2D, texID);

  // Efeito "esticar" textura para preencher o polígono
  /*GL_TEXTURE_WRAP_S: eixo horizontal da textura (U).
        GL_TEXTURE_WRAP_T: eixo vertical da textura (V).
        GL_CLAMP_TO_EDGE: limita a amostra da textura à borda da imagem (evita
     vazamento de pixels ao redor).*/

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ou GL_REPEAT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // ou GL_REPEAT

  /*GL_TEXTURE_MIN_FILTER: quando a textura for reduzida (minificação).
      GL_TEXTURE_MAG_FILTER: quando a textura for ampliada (magnificação).
      GL_LINEAR: faz interpolação linear (suaviza os pixels, evita blocos
     visíveis)*/
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format =
      (channels == 4)
          ? GL_RGBA
          : GL_RGB; // Define o formato da textura de acordo com channels

  // envia os dados da imagem para a GPU como textura 2D
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
               data);

  stbi_image_free(data); // Libera a memória
}

void init_textures() {
  glGenTextures(TEXTURE_COUNT, texId);
  for (int i = 0; i < TEXTURE_COUNT; i++) {
    loadTexture(textures[i], texId[i]);
  }
}

void init_app() {
  set_debug_mode();
  app.name = "Trabalho CG";
}

void clean_app() {
  c_info("Cleaning up application...");
  exit(0);
}

// --- Handlers de Input ---
void HandleActiveMouseMotion(int x, int y) {
  // Para ações de arrastar (não usado para câmera)
}

void HandlePassiveMouseMotion(int x, int y) {
  if (firstMouse) {
    lastX = (float)x;
    lastY = (float)y;
    firstMouse = false;
    return;
  }

  float xoffset = (float)x - lastX;
  float yoffset = lastY - (float)y; // Invertido
  lastX = (float)x;
  lastY = (float)y;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Clamp pitch
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  // Recalcular cameraFront
  vec3f front;
  front.x = cos(rad(yaw)) * cos(rad(pitch));
  front.y = sin(rad(pitch));
  front.z = sin(rad(yaw)) * cos(rad(pitch));
  cameraFront = normalize(front);

  // === CORREÇÃO DO MOUSE ===
  // Se o mouse atingir a borda, teleporte para o centro e
  // REINICIE a lógica do 'firstMouse' para evitar saltos.
  int margin = 1; // Margem de 1 pixel
  if (x >= app.window_width - margin || y >= app.window_height - margin ||
      x <= margin || y <= margin) {
    lastX = app.window_width / 2.0f;
    lastY = app.window_height / 2.0f;
    glutWarpPointer((int)lastX, (int)lastY);
    firstMouse = true; // ESSA é a correção chave
  };
}

void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Handle left click
  };
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    // Handle right click
  };
};

// === ATUALIZADO: Handler de Tecla Pressionada (só define flags) ===
void keyboard(unsigned char key, int x, int y) {
  if (key == 27) { // ESC
    clean_app();
  };

  // Define flags como verdadeiras
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

// === NOVO: Handler de Tecla Solta (define flags como falsas) ===
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
  // ...
};

// === ATUALIZADO: Render Scene (agora processa input) ===
void render_scene() {
  // === Cálculo do DeltaTime ===
  float currentFrame = glutGet(GLUT_ELAPSED_TIME);
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  // === NOVO: Processar Input do Teclado (movido para cá) ===
  float speed = cameraSpeed * deltaTime;
  vec3f worldUp = {0.0f, 1.0f, 0.0f};
  vec3f cameraRight = normalize(cross(cameraFront, worldUp));

  if (key_w) {
    cameraPos.x += cameraFront.x * speed;
    cameraPos.z += cameraFront.z * speed;
  };
  if (key_s) {
    cameraPos.x -= cameraFront.x * speed;
    cameraPos.z -= cameraFront.z * speed;
  };
  if (key_a) {
    cameraPos.x -= cameraRight.x * speed;
    cameraPos.z -= cameraRight.z * speed;
  };
  if (key_d) {
    cameraPos.x += cameraRight.x * speed;
    cameraPos.z += cameraRight.z * speed;
  };
  if (key_space) {
    cameraPos.y += speed;
  };
  if (key_f) {
    cameraPos.y -= speed;
  };
  // === FIM: Processar Input ===

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // === ATUALIZADO: Use gluLookAt para câmera ===
  vec3f center = {cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y,
                  cameraPos.z + cameraFront.z};
  gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, // Eye
            center.x, center.y, center.z,          // Center
            cameraUp.x, cameraUp.y, cameraUp.z);   // Up

  // --- Seu código de desenho ---
  Color red = {1.0f, 0.0f, 0.0f, 1.0f};
  draw_grass((vec3f){-200.0f, -200.0f, 0.0f}, (vec3f){200.0f, 200.0f, 0.0f}, (Color){0.3f, 0.3f, 0.3f, 1.0f}, texId[1]);

  draw_wall((vec3f){-2.0f, -2.0f, 0.0f}, (vec3f){-2.0f, 2.0f, 0.0f}, 2.0f,
            (Color){0.6f, 0.4f, 0.2f, 1.0f}, 0, DRAW_CUBE_SOLID);
  draw_ceiling((vec3f){-2.0f, -2.0f, 0.0f}, (vec3f){2.0f, 2.0f, 0.0f}, 2.0f,
               (Color){0.3f, 0.3f, 0.3f, 1.0f}, 0);
  draw_cube((vec3f){0.0f, 0.0f, 0.5f}, 1.0f, red, 0, DRAW_CUBE_SOLID);

  draw_skybox(50.0f, texId[0], cameraPos, (Color){1.0f, 1.0f, 1.0f, 1.0f});

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
  glutIdleFunc(render_scene); // Isso torna render_scene o loop principal
}

// === ATUALIZADO: Iniciação da Janela ===
void init_window(int width, int height) {
  c_info("Initializing window...");
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  app.window_width = width;
  app.window_height = height;
  glutCreateWindow(app.name);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
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

  lastFrame = glutGet(GLUT_ELAPSED_TIME);
  glutMainLoop();

  clean_app();
  return 0;
}

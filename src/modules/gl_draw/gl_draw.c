#include "gl_draw.h"

void draw_wall(vec3f p1, vec3f p2, float height, Color color, u32 texture_id,
               DrawCubeType type) {
  // Gira o sistema de Z-up (original) para Y-up (câmera)
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  if (texture_id != 0)
    glBindTexture(GL_TEXTURE_2D, texture_id);
  else
    glBindTexture(GL_TEXTURE_2D, 0);

  glBegin(GL_QUADS);
  glColor4f(color.r, color.g, color.b, color.a);
  // Bottom-left
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p1.x, p1.y, 0.0f);

  // Bottom-right
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p2.x, p2.y, 0.0f);

  // Top-right
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(p2.x, p2.y, height);
  // Top-left
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p1.x, p1.y, height);
  glEnd();
  glPopMatrix(); // Restaura a matriz
}

void draw_grass(vec3f p1, vec3f p2, Color color, u32 texture_id) {
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  if (texture_id != 0)
    glBindTexture(GL_TEXTURE_2D, texture_id);
  else
    glBindTexture(GL_TEXTURE_2D, 0);

  glBegin(GL_QUADS);
  glColor4f(color.r, color.g, color.b, color.a);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p1.x, p1.y, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p2.x, p1.y, 0.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(p2.x, p2.y, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p1.x, p2.y, 0.0f);
  glEnd();
  glPopMatrix(); // Restaura a matriz
}

void draw_floor(vec3f p1, vec3f p2, Color color, u32 texture_id) {
  // Gira o sistema de Z-up (original) para Y-up (câmera)
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  if (texture_id != 0)
    glBindTexture(GL_TEXTURE_2D, texture_id);
  else
    glBindTexture(GL_TEXTURE_2D, 0);

  glBegin(GL_QUADS);
  glColor4f(color.r, color.g, color.b, color.a);

  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p1.x, p1.y, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p2.x, p1.y, 0.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(p2.x, p2.y, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p1.x, p2.y, 0.0f);
  glEnd();
  glPopMatrix(); // Restaura a matriz
}

void draw_ceiling(vec3f p1, vec3f p2, float height, Color color,
                  u32 texture_id) {
  // Gira o sistema de Z-up (original) para Y-up (câmera)
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

  if (texture_id != 0)
    glBindTexture(GL_TEXTURE_2D, texture_id);
  else
    glBindTexture(GL_TEXTURE_2D, 0);

  glBegin(GL_QUADS);
  glColor4f(color.r, color.g, color.b, color.a);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p1.x, p1.y, height);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p2.x, p1.y, height);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(p2.x, p2.y, height);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p1.x, p2.y, height);
  glEnd();
  glPopMatrix(); // Restaura a matriz
}

void draw_cube(vec3f position, float size, Color color, u32 texture_id,
               DrawCubeType type) {
  // Esta função já está correta (Y-up), não precisa de rotação
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glColor4f(color.r, color.g, color.b, color.a);

  switch (type) {
  case DRAW_CUBE_WIREFRAME:
    glutWireCube(size);
    break;

  case DRAW_CUBE_SOLID: {
    if (texture_id != 0) // Correção
      glBindTexture(GL_TEXTURE_2D, texture_id);
    else
      glBindTexture(GL_TEXTURE_2D, 0);

    float s = size * 0.5f; // Half-size

    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-s, -s, s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(s, -s, s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(s, s, s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-s, s, s);
    // Back Face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(s, s, -s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(s, -s, -s);
    // Top Face
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-s, s, s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(s, s, s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(s, s, -s);
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-s, -s, -s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(s, -s, -s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(s, -s, s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-s, -s, s);
    // Right face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(s, -s, -s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(s, s, -s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(s, s, s);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(s, -s, s);
    // Left Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-s, -s, -s);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-s, -s, s);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-s, s, s);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-s, s, -s);
    glEnd();
  } break;
  }
  glPopMatrix();
}

void draw_skybox(float size, u32 texture_id, vec3f player_pos, Color color) {
  glPushMatrix();
  glTranslatef(player_pos.x, player_pos.y, player_pos.z);
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_TEXTURE_BIT);

  if (texture_id < 0)
    glColor4f(color.r, color.g, color.b, color.a);
  else  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
  }

  glCullFace(GL_FRONT);
  glDepthMask(GL_FALSE);

  glutSolidSphere(size, 40, 40);
  glDepthMask(GL_TRUE);
  glPopAttrib();
  glPopMatrix();
}

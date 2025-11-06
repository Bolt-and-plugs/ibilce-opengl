#include "gl_draw.h"

void draw_wall(vec3f p1, vec3f p2, float height, Color color, u32 texture_id, DrawCubeType type) {
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texture_id);
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
  glPopMatrix();
}

void draw_floor(vec3f p1, vec3f p2, Color color, u32 texture_id) {
  glBindTexture(GL_TEXTURE_2D, texture_id); 
  glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(p2.x, p1.y, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(p2.x, p2.y, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x, p2.y, 0.0f);
  glEnd();
}

void draw_ceiling(vec3f p1, vec3f p2, float height, Color color, u32 texture_id) {
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBegin(GL_QUADS);
    glColor4f(color.r, color.g, color.b, color.a);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, height);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(p2.x, p1.y, height);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(p2.x, p2.y, height);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x, p2.y, height);
  glEnd();
}

void draw_cube(vec3f position, float size, Color color, u32 texture_id, DrawCubeType type) {
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z); 
  glColor4f(color.r, color.g, color.b, color.a);
  
  switch (type) {
    case DRAW_CUBE_WIREFRAME:
      glutWireCube(size);
      break;
      
    case DRAW_CUBE_SOLID:
      {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        float s = size * 0.5f; // Half-size for convenience

        glBegin(GL_QUADS);
          // Front Face
          glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s,  s);
          glTexCoord2f(1.0f, 0.0f); glVertex3f( s, -s,  s);
          glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s,  s);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s,  s);
          // Back Face
          glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(-s,  s, -s);
          glTexCoord2f(0.0f, 1.0f); glVertex3f( s,  s, -s);
          glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s, -s);
          // Top Face
          glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s, -s);
          glTexCoord2f(0.0f, 0.0f); glVertex3f(-s,  s,  s);
          glTexCoord2f(1.0f, 0.0f); glVertex3f( s,  s,  s);
          glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s, -s);
          // Bottom Face
          glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s, -s);
          glTexCoord2f(0.0f, 1.0f); glVertex3f( s, -s, -s);
          glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s,  s);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s,  s);
          // Right face
          glTexCoord2f(1.0f, 0.0f); glVertex3f( s, -s, -s);
          glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s, -s);
          glTexCoord2f(0.0f, 1.0f); glVertex3f( s,  s,  s);
          glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s,  s);
          // Left Face
          glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, -s);
          glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s,  s);
          glTexCoord2f(1.0f, 1.0f); glVertex3f(-s,  s,  s);
          glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s, -s);
        glEnd();
      }
      break;
  }
  glPopMatrix();
}

void draw_skybox(float size, u32 texture_id, vec3f player_pos, Color color) {
  glPushMatrix();
  glTranslatef(player_pos.x, player_pos.y, player_pos.z);
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_TEXTURE_BIT);
  glEnable(GL_TEXTURE_2D);
  if (texture_id >= 0)
    glBindTexture(GL_TEXTURE_2D, texture_id);
  glCullFace(GL_FRONT); 
  glDepthMask(GL_FALSE);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  if (texture_id >= 0)
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  else
    glColor4f(color.r, color.g, color.b, color.a);

  if (texture_id >= 0) {
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  }

  glutSolidSphere(size, 40, 40);
  glDepthMask(GL_TRUE);
  glPopAttrib();
  glPopMatrix();
}

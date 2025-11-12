#include "gl_draw.h"

static GLUquadric* skyQuadric = NULL;

static void bind_texture(u32 texture_id) {
    if (texture_id == (u32)-1 || texture_id == 0) {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }
}

void draw_wall(vec3f p1, vec3f p2, float height, Color color, u32 texture_id,
               DrawCubeType type) {
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    bind_texture(texture_id);

    glColor4f(color.r, color.g, color.b, color.a);

    // Compute wall normal
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    glNormal3f(-dy, dx, 0.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p2.x, p2.y, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p2.x, p2.y, height);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x, p1.y, height);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}

// ============================================================
// GRASS
// ============================================================
void draw_grass(vec3f p1, vec3f p2, Color color, u32 texture_id) {
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Convert Z-up to Y-up
    bind_texture(texture_id);

    glColor4f(color.r, color.g, color.b, color.a);
    glNormal3f(0.0f, 0.0f, 1.0f); // Facing upward

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p2.x, p1.y, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p2.x, p2.y, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x, p2.y, 0.0f);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}

// ============================================================
// FLOOR
// ============================================================
void draw_floor(vec3f p1, vec3f p2, Color color, u32 texture_id) {
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Convert Z-up to Y-up
    bind_texture(texture_id);

    glColor4f(color.r, color.g, color.b, color.a);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p2.x, p1.y, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p2.x, p2.y, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x, p2.y, 0.0f);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}

// ============================================================
// CEILING
// ============================================================
void draw_ceiling(vec3f p1, vec3f p2, float height, Color color,
                  u32 texture_id) {
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    bind_texture(texture_id);

    glColor4f(color.r, color.g, color.b, color.a);
    glNormal3f(0.0f, 0.0f, -1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, height);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p2.x, p1.y, height);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p2.x, p2.y, height);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x, p2.y, height);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}

// ============================================================
// CUBE
// ============================================================
void draw_cube(vec3f position, float size, Color color, u32 texture_id,
               DrawCubeType type) {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glColor4f(color.r, color.g, color.b, color.a);

    if (type == DRAW_CUBE_WIREFRAME) {
        glutWireCube(size);
        glPopMatrix();
        return;
    }

    glPushAttrib(GL_ENABLE_BIT);
    bind_texture(texture_id);

    float s = size * 0.5f;

    glBegin(GL_QUADS);
        // Front
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-s, -s,  s);
        glTexCoord2f(1, 0); glVertex3f( s, -s,  s);
        glTexCoord2f(1, 1); glVertex3f( s,  s,  s);
        glTexCoord2f(0, 1); glVertex3f(-s,  s,  s);

        // Back
        glNormal3f(0, 0, -1);
        glTexCoord2f(0, 0); glVertex3f( s, -s, -s);
        glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
        glTexCoord2f(1, 1); glVertex3f(-s,  s, -s);
        glTexCoord2f(0, 1); glVertex3f( s,  s, -s);

        // Top
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0); glVertex3f(-s,  s,  s);
        glTexCoord2f(1, 0); glVertex3f( s,  s,  s);
        glTexCoord2f(1, 1); glVertex3f( s,  s, -s);
        glTexCoord2f(0, 1); glVertex3f(-s,  s, -s);

        // Bottom
        glNormal3f(0, -1, 0);
        glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
        glTexCoord2f(1, 0); glVertex3f( s, -s, -s);
        glTexCoord2f(1, 1); glVertex3f( s, -s,  s);
        glTexCoord2f(0, 1); glVertex3f(-s, -s,  s);

        // Right
        glNormal3f(1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f( s, -s,  s);
        glTexCoord2f(1, 0); glVertex3f( s, -s, -s);
        glTexCoord2f(1, 1); glVertex3f( s,  s, -s);
        glTexCoord2f(0, 1); glVertex3f( s,  s,  s);

        // Left
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
        glTexCoord2f(1, 0); glVertex3f(-s, -s,  s);
        glTexCoord2f(1, 1); glVertex3f(-s,  s,  s);
        glTexCoord2f(0, 1); glVertex3f(-s,  s, -s);
    glEnd();

    glPopAttrib();
    glPopMatrix();
}

void draw_skybox(float size, u32 texture_id, vec3f player_pos, Color color) {
    // Inicializa o quadric na primeira chamada
    if (skyQuadric == NULL) {
        skyQuadric = gluNewQuadric();
        gluQuadricTexture(skyQuadric, GL_TRUE); 
        gluQuadricNormals(skyQuadric, GLU_SMOOTH);
    }

    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
    glTranslatef(player_pos.x, player_pos.y, player_pos.z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING); 
    glCullFace(GL_FRONT); 
    
    if (texture_id == (u32)-1 || texture_id == 0) {
        glDisable(GL_TEXTURE_2D);
        glColor4f(color.r, color.g, color.b, color.a);
    } else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glColor4f(1, 1, 1, 1);
    }

    gluSphere(skyQuadric, size, 40, 40);
    glDepthMask(GL_TRUE);
    glPopAttrib();
    glPopMatrix();
}

void draw_tree(vec3f position, float width, float height, u32 texture_id) {
    glPushMatrix();
    // Salva os atributos de enable (para o GL_ALPHA_TEST) e cor
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT); 

    // 1. Move para a base da árvore
    glTranslatef(position.x, position.y, position.z);

    
    glEnable(GL_ALPHA_TEST);
    // Só desenha pixels cuja transparência (alpha) seja maior que 50%
    glAlphaFunc(GL_GREATER, 0.5f); 

    // 3. Seta a textura e uma cor branca (para não "tingir" a textura)
    bind_texture(texture_id);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    float w = width * 0.5f;

    // 4. Desenha o primeiro plano (paralelo ao plano XY)
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f); // Normal apontando para +Z
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-w, 0, 0); // baixo-esquerda
        glTexCoord2f(1.0f, 0.0f); glVertex3f( w, 0, 0); // baixo-direita
        glTexCoord2f(1.0f, 1.0f); glVertex3f( w, height, 0); // cima-direita
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-w, height, 0); // cima-esquerda
    glEnd();

    // 5. Desenha o segundo plano (paralelo ao plano ZY)
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f); // Normal apontando para +X
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -w); // baixo-esquerda
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0,  w); // baixo-direita
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0, height,  w); // cima-direita
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, -w); // cima-esquerda
    glEnd();

    // 6. Restaura os estados que mudamos
    glDisable(GL_ALPHA_TEST);
    glPopAttrib(); 
    glPopMatrix();
}

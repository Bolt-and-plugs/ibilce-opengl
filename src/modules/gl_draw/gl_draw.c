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


void draw_model(obj* model, vec3f position, vec3f rotation, vec3f scale) {
  if (!model || model->mesh_groups->size == 0) return;

  glPushMatrix();
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_POLYGON_BIT);
  // 1. Transformações (Correto)
  glTranslatef(position.x, position.y, position.z);
  glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
  glScalef(scale.x, scale.y, scale.z);

  vec3f* vertices = (vec3f*)model->vertecies->data;
  vec2f* texcoords = (vec2f*)model->textures->data;
  vec3f* normals = (vec3f*)model->normals->data;

  MeshGroup* groups_array = (MeshGroup*)model->mesh_groups->data;


  for (int i = 0; i < model->mesh_groups->size; i++) {
    MeshGroup* group = &groups_array[i];
    bool has_textures = (group->material && group->material->texture_id > 0);
    face* faces = (face*)group->faces->data;

    if (has_textures) {
      glEnable(GL_TEXTURE_2D);
      bind_texture(group->material->texture_id);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
      glDisable(GL_TEXTURE_2D); 
      if (group->material) {
        glColor3f(group->material->diffuse.r / 255.0f,
                  group->material->diffuse.g / 255.0f,
                  group->material->diffuse.b / 255.0f);
      } else {
        glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
      }
    }

    glBegin(GL_TRIANGLES);
    for (int j = 0; j < group->faces->size; j++) {
      face* f = &faces[j];
      for (int k = 0; k < f->corner_count; k++) {
        int v_idx = f->corners[k].vertex_index - 1;
        int t_idx = f->corners[k].texture_index - 1;
        int n_idx = f->corners[k].normal_index - 1;

        if (n_idx >= 0) {
          glNormal3f(normals[n_idx].x, normals[n_idx].y, normals[n_idx].z);
        }
        if (t_idx >= 0) {
          glTexCoord2f(texcoords[t_idx].x, texcoords[t_idx].y);
        }
        if (v_idx >= 0) {
          glVertex3f(vertices[v_idx].x, vertices[v_idx].y, vertices[v_idx].z);
        }
      }
    }
    glEnd();
  } // Fim do loop de grupos

  glPopAttrib();
  glPopMatrix();
}

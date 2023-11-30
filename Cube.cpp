/****************************************************************************
 *                                                                          *
 * Simple example of texture mapping. This code uses DECAL mode so lighting *
 * is not used. You must use MODULATE mode along with light(s)              *
 *                                                                          *
 ****************************************************************************/
#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
	#include <glut/glut.h>
	#include <OpenGL/glu.h>
#else
	#include <gl/glut.h>
#endif

#include "stb_image.h"
#include "Cube.h"

Cube::Cube(int width, int height, int depth, const char *textureFile) {
  this->width = width;
  this->height = height;
  this->depth = depth;

  textureId = 0;
  this->loadTexture(textureFile);
}

void Cube::loadTexture(const char *filename) {
  int width, height, channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 3);

    if (!image) {
        printf("Texture loading failed!\n");
		return;
    }

    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Enable Modulate
}

void Cube::DrawCube(float centerX, float centerY, float centerZ) {
  glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->textureId);
  // Set up texture mapping.
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glPushMatrix();
    glTranslatef(centerX, centerY, centerZ);
    glScalef(this->width, this->height, this->depth);

    // top face of cube (Do not texture map top for buildings)
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glEnd();

    // right face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    // left face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    // bottom face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    // back face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f( -1.0f, 1.0f,  -1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, 1.0f,  -1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    // front face of cube
    glBegin(GL_QUADS);
    glTexCoord2f( 0.0, 0.0);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f( 0.0, 1.0);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glTexCoord2f( 1.0, 1.0);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glTexCoord2f( 1.0, 0.0);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    glEnd();

    glFlush();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}
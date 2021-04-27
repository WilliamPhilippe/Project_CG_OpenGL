#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>

#include "texture.h"
#include "material.h"
#include "solids.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

const GLfloat roomWidth = 20;
const GLfloat roomHeight = 10;
const GLfloat roomDepth = 1;

const glm::vec2 windowPosition = glm::vec2({ 6, 4 });
const glm::vec3 windowDimention = glm::vec3({ 7.5, 5, 0.2 });

glm::vec3 rgb(GLfloat r, GLfloat g, GLfloat b)
{
    return glm::vec3(r/255, g/255, b/255);
}

GLfloat* getNormal(glm::vec3 u, glm::vec3 v)
{
    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    return glm::value_ptr(normal);
}


void buildBoardTexture(Texture* texture, glm::vec3 color, glm::vec3 dimentions)
{
    GLfloat width = dimentions.x;
    GLfloat height = dimentions.y;
    GLfloat depth = dimentions.z;

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glColor3fv(glm::value_ptr(color));
    glPushMatrix();

        glBegin(GL_QUADS);

            // Front Face
            glNormal3fv(getNormal({ width, 0, depth }, { 0, height, depth }));
            glTexCoord2f(1.0, 1.0); 
            glVertex3f(0.0, 0.0, depth);
            glTexCoord2f(0.0, 1.0); 
            glVertex3f(width, 0.0, depth);
            glTexCoord2f(0.0, 0.0); 
            glVertex3f(width, height, depth);
            glTexCoord2f(1.0, 0.0); 
            glVertex3f(0.0, height, depth);

        glEnd();
        
        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);

            // RIGHT FACE
            glVertex3f(width, 0.0, 0.0);
            glVertex3f(width, height, 0.0);
            glVertex3f(width, height, depth);
            glVertex3f(width, 0.0, depth);

            //LEFT FACE
            glVertex3f(0.0, 0.0, depth);
            glVertex3f(0.0, height, depth);
            glVertex3f(0.0, height, 0.0);
            glVertex3f(0.0, 0.0, 0.0);

            //BOTTOM FACE
            glVertex3f(0.0, 0.0, depth);
            glVertex3f(width, 0.0, depth);
            glVertex3f(width, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);

        glEnd();

    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

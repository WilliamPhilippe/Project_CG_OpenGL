#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glm/glm.hpp"

void drawSolidRect(GLfloat vertices[8][3], int indices[4])
{
    glBegin(GL_QUADS);
        for (int i = 0; i < 4; ++i) {
            glVertex3fv(vertices[indices[i]]);
        }
    glEnd();
}

void drawSolidCuboid(GLfloat w, GLfloat h, GLfloat d)
{
    GLfloat vertices[8][3] = {
        {0, 0, 0}, /* 0 */
        {w, 0, 0}, /* 1 */
        {w, 0, d}, /* 2 */
        {0, 0, d}, /* 3 */
        {0, h, 0}, /* 4 */
        {w, h, 0}, /* 5 */
        {w, h, d}, /* 6 */
        {0, h, d}, /* 7 */
    };

    // Drawing front face
    glNormal3f(0, 0, -1);
    int frontVertices[] = { 0, 4, 5, 1 };
    drawSolidRect(vertices, frontVertices);

    // Drawing back face
    glNormal3f(0, 0, 1);
    int backVertices[] = { 3, 7, 6, 2 };
    drawSolidRect(vertices, backVertices);

    // Drawing left face
    glNormal3f(-1, 0, 0);
    int leftVertices[] = { 0, 3, 7, 4 };
    drawSolidRect(vertices, leftVertices);

    // Drawing right face
    glNormal3f(1, 0, 0);
    int rightVertices[] = { 1, 2, 6, 5 };
    drawSolidRect(vertices, rightVertices);

    // Drawing top face
    glNormal3f(0, 1, 0);
    int topVertices[] = { 4, 5, 6, 7 };
    drawSolidRect(vertices, topVertices);

    // Drawing bottom face
    glNormal3f(0, -1, 0);
    int bottomVertices[] = { 0, 1, 2, 3 };
    drawSolidRect(vertices, bottomVertices);
}
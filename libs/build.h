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

void drawCrosshair(int windowWidth, int windowHeight)
{
    glPushMatrix();
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0, 0, 0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    //horizontal line
    glVertex2i(windowWidth / 2 - 10, windowHeight / 2);
    glVertex2i(windowWidth / 2 + 10, windowHeight / 2);
    glEnd();
    //vertical line
    glBegin(GL_LINES);
    glVertex2i(windowWidth / 2, windowHeight / 2 + 10);
    glVertex2i(windowWidth / 2, windowHeight / 2 - 10);
    glEnd();

    glPopMatrix();
}

void displayText(int windowsWidth, int windowsHeight, std::string s) 
{
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glViewport(0, 0, windowsWidth, windowsHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowsWidth, windowsHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat initX = windowsWidth * 0.1;
    GLfloat initY = windowsHeight * 0.9;

    glRasterPos2i(initX, initY);

    void * font = GLUT_BITMAP_HELVETICA_18;
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glColor3f(0.0, 0.0, 0.0);
        glutBitmapCharacter(font, c);
    }

    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void buildWall(
    GLfloat width, GLfloat height, GLfloat depth, 
    GLfloat texCoordH = 2.0, GLfloat texCoordV = 2.0)
{
    glColor3f(1, 1, 1);
    // Front Face

    glNormal3f(0, 0, -1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(width, 0.0, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(0.0, height, 0.0);
    
    // Back Face

    glNormal3f(0, 0, 1);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(width, 0.0, depth);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, depth);
    glTexCoord2f(0.0, texCoordV); glVertex3f(0.0, height, depth);

    // Left Face

    glNormal3f(-1, 0, 0);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(0.0, 0.0, depth);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(0.0, height, depth);
    glTexCoord2f(0.0, texCoordV); glVertex3f(0.0, height, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);

    // Right Face

    glNormal3f(1, 0, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(width, 0.0, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(width, height, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(width, 0.0, depth);

    // Bottom Face

    glNormal3f(0, -1, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(width, 0.0, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, 0.0, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(0.0, 0.0, depth);
    
    // Top Face
    
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, height, 0.0);
    glTexCoord2f(0.0, texCoordV); glVertex3f(width, height, 0.0);
    glTexCoord2f(texCoordH, texCoordV); glVertex3f(width, height, depth);
    glTexCoord2f(texCoordH, 0.0); glVertex3f(0.0, height, depth);
}

void buildDoorFrame(Texture* texture) 
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();
        glTranslatef(roomWidth + roomDepth - 0.2, 0, -5);
        glRotatef(-90, 0, 1, 0);

        glBegin(GL_QUADS);
            buildWall(0.4, 8.0, roomDepth);
        glEnd();

        glTranslatef(5-0.4, 0.0, 0.0);
        glBegin(GL_QUADS);
            buildWall(0.4, 8.0, roomDepth);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(roomWidth + roomDepth - 0.2, 8.0, -5);
        glRotatef(-90, 0, 1, 0);

        glBegin(GL_QUADS);
            buildWall(5.0, 0.4, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildDoor(Texture* texture, GLfloat angle)
{
    GLfloat width = 4.2;
    GLfloat height = 8.0;
    GLfloat depth = roomDepth / 3;

    glPushMatrix();
        glTranslatef(roomWidth, 0, -0.4);
        glRotatef(90 + angle, 0, 1, 0);

        glColor3f(0.91, 0.9, 0.93);

        glEnable(GL_TEXTURE_2D);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texture->id);

        glBegin(GL_QUADS);

            // Front Face
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, height, 0 }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
            // Back Face
            glNormal3fv(getNormal({ width, 0, depth }, { 0, height, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, depth);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, depth);

        glEnd();
        
        glDisable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            // Left Face
            glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(0.0, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, depth);
            // Right Face
            glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, depth);
            // Bottom Face
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, 0.0, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, depth);
            // Top Face
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
            glTexCoord2f(0.0, 0.0); glVertex3f(0.0, height, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, height, depth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildFrontWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glPushMatrix();
        glTranslatef(0.0, 0.0, -20.0 - roomDepth);

        // Bottom part
        glBegin(GL_QUADS);
            buildWall(roomWidth, windowPosition.y, roomDepth);
        glEnd();

        // Top Part
        glPushMatrix();
            glTranslatef(0.0, windowPosition.y + windowDimention.y, 0.0);
            glBegin(GL_QUADS);
                buildWall(roomWidth, roomHeight - (windowPosition.y + windowDimention.y), roomDepth);
            glEnd();
        glPopMatrix();

         // Left part
        glPushMatrix();
            glTranslatef(0.0, windowPosition.y, 0.0);
            glBegin(GL_QUADS);
                buildWall(windowPosition.x, windowDimention.y, roomDepth);
            glEnd();
        glPopMatrix();
        
        // Right part
        glPushMatrix();
            glTranslatef(windowPosition.x + windowDimention.x, windowPosition.y, 0.0);
            glBegin(GL_QUADS);
                buildWall(roomWidth - (windowPosition.x + windowDimention.x), windowDimention.y, roomDepth);
            glEnd();
        glPopMatrix();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildBackWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glPushMatrix();
        glTranslatef(roomWidth + roomDepth, 0.0, roomDepth);
        glRotatef(180.0, 0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            buildWall(roomWidth + roomDepth, roomHeight, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildLeftWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();
        glTranslatef(-roomDepth, 0.0, 0.0);
        glRotatef(90.0, 0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            buildWall(roomWidth, roomHeight, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildRightWall(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glPushMatrix();
        glTranslatef(roomWidth + roomDepth, 0.0, -roomWidth);
        glRotatef(270.0, 0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            buildWall(roomWidth - 5, roomHeight, roomDepth);
        glEnd();

        glTranslatef(roomWidth - 5, 8.4, 0.0);
        
        glBegin(GL_QUADS);
            buildWall(5, roomHeight - 8.4, roomDepth);
        glEnd();
    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildFloor(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 2.0); glVertex3f(20.0, 0.0, 0.0);
    glTexCoord2f(2.0, 2.0); glVertex3f(20.0, 0.0, -20.0);
    glTexCoord2f(2.0, 0.0); glVertex3f(0.0, 0.0, -20.0);

    glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildRoof(Texture* texture)
{
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glNormal3fv(getNormal({ 0, 0, -20 }, { 20, 0, 0 }));
    glBegin(GL_QUADS);

        glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 10.0, 0.0);
        glTexCoord2f(0.0, 2.0); glVertex3f(20.0, 10.0, 0.0);
        glTexCoord2f(2.0, 2.0); glVertex3f(20.0, 10.0, -20.0);
        glTexCoord2f(2.0, 0.0); glVertex3f(0.0, 10.0, -20.0);

    glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildTable(bool alignLeft)
{
    GLfloat width = 7.f;
    GLfloat height = roomHeight / 3;
    GLfloat depth = 3.f;
    GLfloat thickness = 0.2f;

    GLfloat offset = alignLeft ? roomWidth - thickness : 0;

    glColor3f(1, 1, 1);
    glPushMatrix();
        glRotatef(-90.f, 0, 1, 0);
        glTranslatef(-roomWidth, 0, -roomWidth + (alignLeft ? roomWidth - width : 0));

        glBegin(GL_QUADS);
            buildWall(depth, height - thickness, thickness);
        glEnd();

    glPopMatrix();

    glPushMatrix();
        glRotatef(-90.f, 0, 1, 0);
        glTranslatef(-roomWidth, 0, -thickness + (alignLeft ? 0 : width - roomWidth));

        glBegin(GL_QUADS);
            buildWall(depth, height - thickness, thickness);
        glEnd();

    glPopMatrix();

    glPushMatrix();
        glRotatef(-90.f, 1, 0, 0);
        glTranslatef((alignLeft ? 0 : roomWidth - width), roomWidth - depth, height - thickness);

        glBegin(GL_QUADS);
            buildWall(7, depth, thickness);
        glEnd();

    glPopMatrix();
    glFlush();
}

void buildWindowGlass() 
{
    glPushMatrix();
        glm::vec3 innerWindowDimentions = glm::vec3({ 
            (windowDimention.x - (2 * windowDimention.z)) / 2,
            windowDimention.y - (2 * windowDimention.z),
            0.3
        });

        glColor3fv(glm::value_ptr(rgb(130, 130, 130)));
        drawSolidCuboid(0.2, innerWindowDimentions.y, innerWindowDimentions.z);

        glPushMatrix();
            glTranslatef(innerWindowDimentions.x, 0, 0);
            drawSolidCuboid(0.2, innerWindowDimentions.y, innerWindowDimentions.z);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.2, 0, 0);
            drawSolidCuboid(innerWindowDimentions.x - 0.2, 0.2, innerWindowDimentions.z);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.2, innerWindowDimentions.y - 0.2, 0);
            drawSolidCuboid(innerWindowDimentions.x - 0.2, 0.2, innerWindowDimentions.z);
        glPopMatrix();

        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        glEnable( GL_BLEND );
        glPushMatrix();

            glColor3f(0.8, 0.8, 0.8);
            glTranslatef(0.2, 0, 0);
            glBegin(GL_QUADS);
                glVertex3f(0, 0, 0);
                glVertex3f(innerWindowDimentions.x - 0.2, 0, 0.1);
                glVertex3f(innerWindowDimentions.x - 0.2, innerWindowDimentions.y, 0.1);
                glVertex3f(0, innerWindowDimentions.y, 0.1);
            glEnd();

        glPopMatrix();
        glDisable( GL_BLEND );

    glPopMatrix();
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
            glNormal3fv(getNormal({ width, 0, 0 }, { 0, height, 0 }));
            glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, 0.0, 0.0);
            glTexCoord2f(0.0, 0.0); glVertex3f(width, height, 0.0);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, height, 0.0);
            // Back Face
            glNormal3fv(getNormal({ width, 0, depth }, { 0, height, depth }));
            glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, depth);
            glTexCoord2f(0.0, 1.0); glVertex3f(width, 0.0, depth);
            glTexCoord2f(0.0, 0.0); glVertex3f(width, height, depth);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.0, height, depth);

        glEnd();
        
        glDisable(GL_TEXTURE_2D);

        // glBegin(GL_QUADS);
        //     // Left Face
        //     glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
        //     glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
        //     glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
        //     glTexCoord2f(1.0, 1.0); glVertex3f(0.0, height, depth);
        //     glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, depth);
        //     // Right Face
        //     glNormal3fv(getNormal({ 0, height, 0 }, { 0, 0, depth }));
        //     glTexCoord2f(0.0, 0.0); glVertex3f(width, 0.0, 0.0);
        //     glTexCoord2f(0.0, 1.0); glVertex3f(width, height, 0.0);
        //     glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
        //     glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, depth);
        //     // Bottom Face
        //     glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
        //     glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
        //     glTexCoord2f(0.0, 1.0); glVertex3f(width, 0.0, 0.0);
        //     glTexCoord2f(1.0, 1.0); glVertex3f(width, 0.0, depth);
        //     glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, depth);
        //     // Top Face
        //     glNormal3fv(getNormal({ width, 0, 0 }, { 0, 0, depth }));
        //     glTexCoord2f(0.0, 0.0); glVertex3f(0.0, height, 0.0);
        //     glTexCoord2f(0.0, 1.0); glVertex3f(width, height, 0.0);
        //     glTexCoord2f(1.0, 1.0); glVertex3f(width, height, depth);
        //     glTexCoord2f(1.0, 0.0); glVertex3f(0.0, height, depth);
        // glEnd();

    glPopMatrix();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void buildWindow(mat::Material chrome)
{
    glPushMatrix();
        chrome.active();
        glColor3fv(glm::value_ptr(rgb(110, 110, 110)));

        glTranslatef(windowPosition.x, windowPosition.y, -(roomWidth + roomDepth + 0.1));

        drawSolidCuboid(windowDimention.z, windowDimention.y, roomDepth + 0.2);

        glPushMatrix();
            glTranslatef(windowDimention.x - windowDimention.z, 0, 0);
            drawSolidCuboid(windowDimention.z, windowDimention.y, roomDepth + 0.2);
        glPopMatrix();

        glPushMatrix();
            drawSolidCuboid(windowDimention.x, windowDimention.z, roomDepth + 0.2);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, windowDimention.y - windowDimention.z, 0);
            drawSolidCuboid(windowDimention.x, windowDimention.z, roomDepth + 0.2);
        glPopMatrix();
    glPopMatrix();
}

void buildShelfWithBooks()
{
    glPushMatrix();
        glTranslatef(0, 3*roomHeight/4, -roomWidth);
        // Shelf
        drawSolidCuboid(windowPosition.x - 0.5, 0.2, 1.5);
        // Books
        glMaterialf(GL_FRONT, GL_SHININESS, 100);
        glTranslatef(0, 0.2, 0);
        glColor3fv(glm::value_ptr(rgb(150, 0, 0)));
        drawSolidCuboid(0.3, 1.5, 1.2);

        glTranslatef(0.3, 0, 0);
        glColor3fv(glm::value_ptr(rgb(10, 100, 150)));
        drawSolidCuboid(0.3, 1.3, 1.0);

        glTranslatef(0.3, 0, 0);
        glColor3fv(glm::value_ptr(rgb(150, 100, 0)));
        drawSolidCuboid(0.3, 1.2, 0.9);
    glPopMatrix();
}
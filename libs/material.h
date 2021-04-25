#pragma once

#include <GL/gl.h>
#include <vector>

namespace mat {
    class Material {
        public:
        GLfloat ambient_r;
        GLfloat ambient_g;
        GLfloat ambient_b;

        GLfloat diffuse_r;
        GLfloat diffuse_g;
        GLfloat diffuse_b;

        GLfloat specular_r;
        GLfloat specular_g;
        GLfloat specular_b;

        GLfloat high_shininess;

        Material() {
            
        }

        Material(std::vector<float> ambient, 
                std::vector<float> diffuse, 
                std::vector<float> specular, 
                float shininess) 
        {
            ambient_r = ambient[0]; ambient_g = ambient[1]; ambient_b = ambient[2];
            diffuse_r = diffuse[0]; diffuse_g = diffuse[1]; diffuse_b = diffuse[2];
            specular_r = specular[0]; specular_g = specular[1]; specular_b = specular[2];

            high_shininess = shininess;
        }

        Material(GLfloat ar, GLfloat ag, GLfloat ab, GLfloat dr, GLfloat dg, GLfloat db, GLfloat sr, GLfloat sg, GLfloat sb, GLfloat shininess){
            ambient_r=ar;   ambient_g=ag;   ambient_b=ab;
            diffuse_r=dr;   diffuse_g=dg;   diffuse_b=db;
            specular_r=sr;  specular_g=sg;  specular_b=sb;
            high_shininess=shininess;
        }

        void active(){
            GLfloat ambient[]={ambient_r, ambient_g, ambient_b, 1.0};
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
            GLfloat diffuse[]={diffuse_r, diffuse_g, diffuse_b};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
            GLfloat specular[]={specular_r, specular_g, specular_b};
            glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
            glMaterialf(GL_FRONT, GL_SHININESS, high_shininess);
        }

        void dye()
        {
            glColor3f(diffuse_r, diffuse_g, diffuse_b);
        }
    };
}
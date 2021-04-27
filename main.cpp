
#define GLEW_STATIC

#include <GL/glew.h>


#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <unistd.h>

using std::cout;

#define STB_IMAGE_IMPLEMENTATION
#include "./libs/stb_image.h"
#include "./libs/camera.h"
#include "./libs/build.h"
#include "./libs/texture.h"
#include "./libs/animation.h"
#include "./libs/solids.h"
#include "./libs/material.h"

#include "./libs/obj_loader.h"

#include<unistd.h>


#define BLACK 0.0f, 0.0f, 0.0f
#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define AZUL     0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0
#define CYAN     1.0, 0.0, 1.0
#define LARANJA  0.8, 0.6, 0.1
#define CINZA    0.6, 0.6, 0.6
#define PAREDE    0.87, 0.91, 0.92
#define PAREDE2    0.57, 0.61, 0.62
#define JANELA    0.407, 0.282, 0.098
#define MESA    0.77, 0.81, 0.92
#define LIGHTBLUE 0.678, 0.847, 0.902


struct Vec2 {
	float x, y;
};
typedef struct Vec2 Vec2;

struct Vec3 {
	float x, y, z;
};
typedef struct Vec3 Vec3;

struct Transform {
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};
typedef struct Transform Transform;

const float DEG2RAD = M_PI / 180.0f;
const float RAD2DEG = 180.0f / M_PI;

Vec2 WINDOW_SIZE = {1040, 720};
Vec2 WINDOW_CENTER = {320, 240};
float FOVY = 75.0f;
float ZNEAR = 10e-3;
float ZFAR = 10e3;
Transform CAM;
int KEYBOARD[128] = {0};
Vec2 MOTION;

void init_gl();
void display();
void idle();
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);
void reshape(int width, int height);
void draw_bedroom();
void draw_axis(int x, int y, int z);
void draw_grid(int n);
void display2();
int load_obj(const char* path, int type);

Vec3 forward(Transform* t);
Vec3 right(Transform* t);
Vec3 up(Transform* t);

// OBJECTS

objl::Loader tableLoader;
objl::MeshInfo tableMesh;
objl::MeshInfo tableSeatMesh;

objl::Loader bedLoader;
objl::MeshInfo bed1Mesh;
objl::MeshInfo bed2Mesh;

objl::Loader chairLoader;
objl::MeshInfo chair1Mesh;
objl::MeshInfo chair2Mesh;
objl::MeshInfo chair3Mesh;
objl::MeshInfo chair4Mesh;

objl::Loader pillowLoader;
objl::MeshInfo pillow1Mesh;
objl::MeshInfo pillow2Mesh;

objl::Loader bookLoader;
objl::MeshInfo book1Mesh;
objl::MeshInfo book2Mesh;

objl::Loader cupLoader;
objl::MeshInfo cup1Mesh;
objl::MeshInfo cup2Mesh;

objl::Loader shelfLoader;
objl::MeshInfo shelf1Mesh;
objl::MeshInfo shelf2Mesh;
objl::MeshInfo shelf3Mesh;

objl::Loader carpetLoader;
objl::MeshInfo carpet1Mesh;

objl::Loader fanLoader;
objl::MeshInfo fan1Mesh;
objl::MeshInfo fan2Mesh;
objl::MeshInfo fan3Mesh;
objl::MeshInfo fan4Mesh;
objl::MeshInfo fan5Mesh;

// TEXTURES

Texture vanGoghPaiting;
Texture doorTexture;
Texture carpetTexture;
Texture metalTexture;
Texture woodTexture;
Texture blanketTexture;
Texture windowTexture;

void loadTexture(const char* fileName, Texture* texture) 
{
    // Generate Texture
    glBindTexture(GL_TEXTURE_2D, texture->id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int nrChannels;

    texture->data = stbi_load(fileName, &texture->width, &texture->height, &nrChannels, 0);

    if (!texture->data) {
        cout << "Failed to load texture: " << fileName << "\n";
        exit(1);
    } 

}

void setupTexture(Texture* texture)
{
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                    GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, 
                    texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                    texture->data);                  
}

void setTextures() {
	loadTexture("./imgs/textures/paiting-van-gogh.png", &vanGoghPaiting);
	setupTexture(&vanGoghPaiting);

	loadTexture("./imgs/textures/door.png", &doorTexture);
	setupTexture(&doorTexture);

	loadTexture("./imgs/textures/carpet.png", &carpetTexture);
	setupTexture(&carpetTexture);

	loadTexture("./imgs/textures/iron.png", &metalTexture);
	setupTexture(&metalTexture);

	loadTexture("./imgs/textures/wood.png", &woodTexture);
	setupTexture(&woodTexture);

	loadTexture("./imgs/textures/blanket.png", &blanketTexture);
	setupTexture(&blanketTexture);

	loadTexture("./imgs/textures/window.png", &windowTexture);
	setupTexture(&windowTexture);
}

void init_gl() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	setTextures();

	tableMesh = tableLoader.LoadedMeshes[0].setup();
	tableSeatMesh = tableLoader.LoadedMeshes[1].setup();

	bed1Mesh = bedLoader.LoadedMeshes[0].setup();
	bed2Mesh = bedLoader.LoadedMeshes[1].setup();

	chair1Mesh = chairLoader.LoadedMeshes[1].setup();
	chair2Mesh = chairLoader.LoadedMeshes[2].setup();
	chair3Mesh = chairLoader.LoadedMeshes[3].setup();
	// chair4Mesh = chairLoader.LoadedMeshes[0].setup();

	pillow1Mesh = pillowLoader.LoadedMeshes[1].setup();
	pillow2Mesh = pillowLoader.LoadedMeshes[0].setup();

	book1Mesh = bookLoader.LoadedMeshes[0].setup();
	book2Mesh = bookLoader.LoadedMeshes[1].setup();

	cup1Mesh = cupLoader.LoadedMeshes[0].setup();
	cup2Mesh = cupLoader.LoadedMeshes[1].setup();

	shelf1Mesh = shelfLoader.LoadedMeshes[0].setup();
	shelf2Mesh = shelfLoader.LoadedMeshes[1].setup();
	shelf3Mesh = shelfLoader.LoadedMeshes[2].setup();

	carpet1Mesh = carpetLoader.LoadedMeshes[0].setup();

	fan1Mesh = fanLoader.LoadedMeshes[0].setup();
	fan2Mesh = fanLoader.LoadedMeshes[1].setup();
	fan3Mesh = fanLoader.LoadedMeshes[2].setup();
	fan4Mesh = fanLoader.LoadedMeshes[3].setup();
	fan5Mesh = fanLoader.LoadedMeshes[4].setup();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3 fwd = forward(&CAM);
	Vec3 u = up(&CAM);
	Vec3 eye = CAM.position;
	Vec3 center = {eye.x + fwd.x, eye.y + fwd.y, eye.z + fwd.z};

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, u.x, u.y, u.z); 

	draw_grid(10);
	draw_axis(1, 1, 1);
	draw_bedroom();

	glutSwapBuffers();
}

static GLfloat vertices[108]={
   /* vértices das paredes e teto */
   10.0,  0.0,   -10.0, /* 0 */
   10.0,  5.0,   -10.0, /* 1 */
  -10.0,  5.0,   -10.0, /* 2 */
  -10.0,  0.0,   -10.0, /* 3 */
  -10.0,  0.0,    10.0, /* 4 */
  -10.0,  5.0,    10.0, /* 5 */
   10.0,  5.0,   10.0, /* 6 */
   10.0,  0.0,    10.0, /* 7 */
	
	/* vértices do piso, um pouco abaixo do quadrangulado */
   10.0,  -0.0,  -10.0, /* 8 */
  -10.0,  -0.0,  -10.0, /* 9 */
   10.0,  -0.0,   10.0, /* 10 */
  -10.0,  -0.0,   10.0, /* 11 */

	/*vértices da moldura da porta*/
	8.0, 0.0, -9.999, /* 12 */
	8.0, 4.0, -9.999, /* 13 */
	5.0, 4.0, -9.999, /* 14 */
	5.0, 0.0, -9.999,  /* 15 */

	/*vértices janela*/
	0, 3.0, 2.0, /* 16 */
	0, 1.0, 2.0, /* 17 */
	0, 3.0, 0.0, /* 18 */
	0, 1.0, 0.0, /* 19 */

	/*vértices da porta que se movimenta*/
	0, 0.0, 0, /* 20 */
	0, 4.0, 0, /* 21 */
	-3.0, 4.0, 0, /* 22 */
	-3.0, 0.0, 0,  /* 23 */

	/* vertices mesa */
	7.0, 7.89, -4.5, /* 24 */
	7.0, 7.7, -4.5, /* 25 */
	7.00000, 7.89000, 4.50000, /* 26 */
	7.00000, 7.71000, 4.50000, /* 27 */
	7.00000, 7.89000, -4.50000, /* 28 */
	-7.00000, 7.71000, -4.50000, /* 29 */
	-7.00000, 7.89000, 4.50000, /* 30 */ 
	-7.00000, 7.71000, 4.50000,/* 31 */

	/*vértices janela moldura*/
	-9.999, 3.0, 5.0, /* 32 */
	-9.999, 1.0, 5.0, /* 33 */
	-9.999, 3.0, 3.0, /* 34 */
	-9.999, 1.0, 3.0, /* 35 */

};

static GLubyte trasIndices[] = {1,2,3,0};
static GLubyte portaIndices[] = {12,13,14,15};
static GLubyte portaMovIndices[] = {20,21,22,23};
static GLubyte portaMovTrazIndices[] = {23,22,21,20};
static GLubyte molduraJanelaIndices[] = {32,33,35,34};
static GLubyte janelaIndices[] = {16,17,19,18};
static GLubyte janelaTrazIndices[] = {18,19,17,16};
static GLubyte leftIndices[] = {4,3,2,5};
static GLubyte frenteIndices[] = {7,4,5,6};
static GLubyte rightIndices[] = {7,6,1,0};
static GLubyte tetoIndices[] = {5,2,1,6};
static GLubyte pisoIndices[] = {8,9,11,10};

static GLubyte mesa[] = {31,30,29,28,27,26,25,24};

static int eixoPortay, eixoJanelay, eixoVentiladorX = 0, velocidadeFan = 0;

void buildDoor(Texture *texture) {
	
	glPushMatrix();
			glTranslatef(8.0, 0, -9.998);
			glRotatef ((GLfloat) eixoPortay, 0.0, 1.0, 0.0);
		
			glColor3f(JANELA);
			glTranslatef(-3, 0, 0.001);
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    		glBindTexture(GL_TEXTURE_2D, texture->id);

				glBegin(GL_QUADS);
					// FRONT FACE
					glTexCoord2f(1.0, 1.0);
					glVertex3f(0.0, 0.0, 0);

					glTexCoord2f(0.0, 1.0);
					glVertex3f(3, 0.0, 0);

					glTexCoord2f(0.0, 0.0); 
					glVertex3f(3, 4, 0);

					glTexCoord2f(1.0, 0.0); 
					glVertex3f(0.0, 4, 0);

					// BACK FACE
					glTexCoord2f(1.0, 0.0); 
					glVertex3f(0.0, 4, 0);
					glTexCoord2f(0.0, 0.0); 
					glVertex3f(3, 4, 0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(3, 0.0, 0);
					glTexCoord2f(1.0, 1.0);
					glVertex3f(0.0, 0.0, 0);			
					
				glEnd();
      glDisable(GL_TEXTURE_2D);
			
	glPopMatrix();
}

void buildWindow() {
	glPushMatrix();
			glTranslatef(-9.998, 0, 3.0);
			glRotatef ((GLfloat) eixoJanelay, 0.0, 1.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    	glBindTexture(GL_TEXTURE_2D, windowTexture.id);
			glColor3f(JANELA);

			glBegin(GL_QUADS);
					// FRONT FACE
					glTexCoord2f(1.0, 1.0);
					glVertex3f(0.0, 3.0, 2.0);

					glTexCoord2f(0.0, 1.0);
					glVertex3f(0.0, 1.0, 2.0);

					glTexCoord2f(0.0, 0.0); 
					glVertex3f(0.0, 1.0, 0.0);

					glTexCoord2f(1.0, 0.0); 
					glVertex3f(0.0, 3.0, 0.0);

					// BACK FACE
					glTexCoord2f(1.0, 0.0); 
					glVertex3f(0.0, 3.0, 0.0);
					glTexCoord2f(0.0, 0.0); 
					glVertex3f(0.0, 1.0, 0.0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(0.0, 1.0, 2.0);
					glTexCoord2f(1.0, 1.0);
					glVertex3f(0.0, 3.0, 2.0);			
					
				glEnd();
	      glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void buildRoom() {
		glColor3f(BLACK);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, molduraJanelaIndices);

		glColor3f(BLACK);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, portaIndices);

    glColor3f(PAREDE);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, trasIndices);

    glColor3f(PAREDE2);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, leftIndices);

    glColor3f(PAREDE);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, frenteIndices);

		glColor3f(PAREDE2);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, rightIndices);

		glColor3f(LARANJA);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, tetoIndices);

		glColor3f(CINZA);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, pisoIndices);
}

void buildTable(Texture *metal, Texture *wood) {
	glPushMatrix();
			tableMesh.material.active();
			tableMesh.material.dye();
			glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);
      glTranslatef(0, 0, 9);
      glScalef(2, 2, 2);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, wood->id);
			glColor3f(JANELA);

			glVertexPointer(3, GL_FLOAT, 0, &tableMesh.vertices_pointers[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &tableSeatMesh.vertices_tex_coords[0]);
      glNormalPointer(GL_FLOAT, 0, &tableMesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, tableMesh.indices_pointers.size(), GL_UNSIGNED_INT, &tableMesh.indices_pointers[0]);
			glDisable(GL_TEXTURE_2D);

			glPushMatrix();
            tableSeatMesh.material.active();
            tableSeatMesh.material.dye();

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);

						glEnable(GL_TEXTURE_2D);
						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
						glBindTexture(GL_TEXTURE_2D, metal->id);

						glColor3f(MESA);

            glVertexPointer(3, GL_FLOAT, 0, &tableSeatMesh.vertices_pointers[0]);
			      glTexCoordPointer(2, GL_FLOAT, 0, &tableSeatMesh.vertices_tex_coords[0]);
            glNormalPointer(GL_FLOAT, 0, &tableSeatMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, tableSeatMesh.indices_pointers.size(), GL_UNSIGNED_INT, &tableSeatMesh.indices_pointers[0]);
				    glDisable(GL_TEXTURE_2D);
        glPopMatrix();
  glPopMatrix();

	glPushMatrix();
			book1Mesh.material.active();
			book1Mesh.material.dye();
      glScalef(0.02, 0.02, 0.02);
      glTranslatef(420, 80, 0);
			glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glColor3f(RED);

			glVertexPointer(3, GL_FLOAT, 0, &book1Mesh.vertices_pointers[0]);
      glNormalPointer(GL_FLOAT, 0, &book1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, book1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &book1Mesh.indices_pointers[0]);

			glPushMatrix();
            book2Mesh.material.active();
            book2Mesh.material.dye();
						glColor3f(MESA);

            glVertexPointer(3, GL_FLOAT, 0, &book2Mesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &book2Mesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, book2Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &book2Mesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

	glPushMatrix();
			cup1Mesh.material.active();
			cup1Mesh.material.dye();
      glScalef(0.04, 0.04, 0.04);
      glTranslatef(208, 41, -5);
			glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glColor3f(BLACK);

			glVertexPointer(3, GL_FLOAT, 0, &cup1Mesh.vertices_pointers[0]);
      glNormalPointer(GL_FLOAT, 0, &cup1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, cup1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &cup1Mesh.indices_pointers[0]);

			glPushMatrix();
            cup2Mesh.material.active();
            cup2Mesh.material.dye();
						glColor3f(AZUL);

            glVertexPointer(3, GL_FLOAT, 0, &cup2Mesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &cup2Mesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, cup2Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &cup2Mesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();

		glPushMatrix();
			chair1Mesh.material.active();
			chair1Mesh.material.dye();
			glRotatef ((GLfloat) -90, 0.0, 1.0, 0.0);
      glScalef(0.05, 0.05, 0.05);
      glTranslatef(0, 0, -120);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glColor3f(MESA);

			glVertexPointer(3, GL_FLOAT, 0, &chair1Mesh.vertices_pointers[0]);
      glNormalPointer(GL_FLOAT, 0, &chair1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, chair1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &chair1Mesh.indices_pointers[0]);

			glPushMatrix();
            chair2Mesh.material.active();
            chair2Mesh.material.dye();
						glColor3f(MESA);

            glVertexPointer(3, GL_FLOAT, 0, &chair2Mesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &chair2Mesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, chair2Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &chair2Mesh.indices_pointers[0]);

      glPopMatrix();

			glPushMatrix();
							chair3Mesh.material.active();
							chair3Mesh.material.dye();
							glColor3f(PAREDE2);

							glVertexPointer(3, GL_FLOAT, 0, &chair3Mesh.vertices_pointers[0]);
							glNormalPointer(GL_FLOAT, 0, &chair3Mesh.vertices_normals[0]);
							glDrawElements(GL_TRIANGLES, chair3Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &chair3Mesh.indices_pointers[0]);

			glPopMatrix();
    glPopMatrix();

}

void buildBed(Texture *wood, Texture *blanket) {

		glPushMatrix();
			bed2Mesh.material.active();
			bed2Mesh.material.dye();
      glScalef(0.05, 0.05, 0.05);
      glTranslatef(-160, 0,200);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      glBindTexture(GL_TEXTURE_2D, wood->id);
			glColor3f(MESA);

			glVertexPointer(3, GL_FLOAT, 0, &bed2Mesh.vertices_pointers[0]);
      glTexCoordPointer(2, GL_FLOAT, 0, &bed2Mesh.vertices_tex_coords[0]);
      glNormalPointer(GL_FLOAT, 0, &bed2Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, bed2Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &bed2Mesh.indices_pointers[0]);
      glDisable(GL_TEXTURE_2D);
			
    glPopMatrix();

		
		glPushMatrix();
			pillow1Mesh.material.active();
			pillow1Mesh.material.dye();
			// glRotatef ((GLfloat) -90, 0.0, 1.0, 0.0);
      glScalef(0.03, 0.03, 0.03);
      glTranslatef(-260, 38, 250);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      glBindTexture(GL_TEXTURE_2D, blanket->id);
			glColor3f(AMARELO);

			glVertexPointer(3, GL_FLOAT, 0, &pillow1Mesh.vertices_pointers[0]);
      glTexCoordPointer(2, GL_FLOAT, 0, &pillow1Mesh.vertices_tex_coords[0]);
      glNormalPointer(GL_FLOAT, 0, &pillow1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, pillow1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &pillow1Mesh.indices_pointers[0]);
      glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void buildShelf() {
glPushMatrix();
			shelf1Mesh.material.active();
			shelf1Mesh.material.dye();
			// glRotatef ((GLfloat) 90, 1.0, 0, 0.0);
      glScalef(2, 1.6, 2);
      glTranslatef(-4.7, 0, 3);
      glTranslatef(0, 0.5,0);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, woodTexture.id);
			glColor3f(AZUL);

			glVertexPointer(3, GL_FLOAT, 0, &shelf1Mesh.vertices_pointers[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &shelf1Mesh.vertices_tex_coords[0]);
      glNormalPointer(GL_FLOAT, 0, &shelf1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, shelf1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &shelf1Mesh.indices_pointers[0]);
			glDisable(GL_TEXTURE_2D);

			glPushMatrix();
							shelf2Mesh.material.active();
							shelf2Mesh.material.dye();
							glColor3f(JANELA);

							glVertexPointer(3, GL_FLOAT, 0, &shelf2Mesh.vertices_pointers[0]);
							glNormalPointer(GL_FLOAT, 0, &shelf2Mesh.vertices_normals[0]);
							glDrawElements(GL_TRIANGLES, shelf2Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &shelf2Mesh.indices_pointers[0]);

			glPopMatrix();

			glPushMatrix();
							shelf3Mesh.material.active();
							shelf3Mesh.material.dye();

							glEnableClientState(GL_TEXTURE_COORD_ARRAY);

							glEnable(GL_TEXTURE_2D);
							glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
							glBindTexture(GL_TEXTURE_2D, woodTexture.id);
							glColor3f(AZUL);

							glVertexPointer(3, GL_FLOAT, 0, &shelf3Mesh.vertices_pointers[0]);
							glTexCoordPointer(2, GL_FLOAT, 0, &shelf3Mesh.vertices_tex_coords[0]);
							glNormalPointer(GL_FLOAT, 0, &shelf3Mesh.vertices_normals[0]);
							glDrawElements(GL_TRIANGLES, shelf3Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &shelf3Mesh.indices_pointers[0]);
							glDisable(GL_TEXTURE_2D);

			glPopMatrix();
    glPopMatrix();

	glPushMatrix();
			glScalef(0.032, 0.032, 0.032);
			glTranslatef(-295, 57, 185);



		glPushMatrix();
			fan1Mesh.material.active();
			fan1Mesh.material.dye();
			eixoVentiladorX = (eixoVentiladorX + velocidadeFan) % 360;
			glRotatef ((GLfloat) eixoVentiladorX, 1.0, 0, 0.0);

			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
			glColor3f(LIGHTBLUE);

			glVertexPointer(3, GL_FLOAT, 0, &fan1Mesh.vertices_pointers[0]);
      glNormalPointer(GL_FLOAT, 0, &fan1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, fan1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &fan1Mesh.indices_pointers[0]);
		glPopMatrix();

		glPushMatrix();
							fan2Mesh.material.active();
							fan2Mesh.material.dye();
							glColor3f(JANELA);

							glVertexPointer(3, GL_FLOAT, 0, &fan2Mesh.vertices_pointers[0]);
							glNormalPointer(GL_FLOAT, 0, &fan2Mesh.vertices_normals[0]);
							glDrawElements(GL_TRIANGLES, fan2Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &fan2Mesh.indices_pointers[0]);

		glPopMatrix();

		glPushMatrix();
							fan3Mesh.material.active();
							fan3Mesh.material.dye();
							glColor3f(AZUL);

							glVertexPointer(3, GL_FLOAT, 0, &fan3Mesh.vertices_pointers[0]);
							glNormalPointer(GL_FLOAT, 0, &fan3Mesh.vertices_normals[0]);
							glDrawElements(GL_TRIANGLES, fan3Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &fan3Mesh.indices_pointers[0]);

		glPopMatrix();

		glPushMatrix();
						fan4Mesh.material.active();
						fan4Mesh.material.dye();
						glColor3f(JANELA);

						glVertexPointer(3, GL_FLOAT, 0, &fan4Mesh.vertices_pointers[0]);
						glNormalPointer(GL_FLOAT, 0, &fan4Mesh.vertices_normals[0]);
						glDrawElements(GL_TRIANGLES, fan4Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &fan4Mesh.indices_pointers[0]);

		glPopMatrix();

		glPushMatrix();
						fan5Mesh.material.active();
						fan5Mesh.material.dye();
						glColor3f(RED);

						glVertexPointer(3, GL_FLOAT, 0, &fan5Mesh.vertices_pointers[0]);
						glNormalPointer(GL_FLOAT, 0, &fan5Mesh.vertices_normals[0]);
						glDrawElements(GL_TRIANGLES, fan5Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &fan5Mesh.indices_pointers[0]);

		glPopMatrix();
    glPopMatrix();

}

void buildCarpet(Texture *texture) {
	glPushMatrix();
			carpet1Mesh.material.active();
			carpet1Mesh.material.dye();
			glTranslatef(0,0.001, 0);


			glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      glBindTexture(GL_TEXTURE_2D, texture->id);

			glColor3f(LIGHTBLUE);

			glVertexPointer(3, GL_FLOAT, 0, &carpet1Mesh.vertices_pointers[0]);
      glTexCoordPointer(2, GL_FLOAT, 0, &carpet1Mesh.vertices_tex_coords[0]);

      glNormalPointer(GL_FLOAT, 0, &carpet1Mesh.vertices_normals[0]);
      glDrawElements(GL_TRIANGLES, carpet1Mesh.indices_pointers.size(), GL_UNSIGNED_INT, &carpet1Mesh.indices_pointers[0]);
      glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void buildBoardVanGogh(){
		glPushMatrix();
			glRotatef ((GLfloat) -90, 0, 1.0, 0.0);
			glTranslatef(-6,1.7,-10);

			buildBoardTexture(&vanGoghPaiting, rgb(155, 125, 155), {5,3,0.1});
		glPopMatrix();
}

void draw_bedroom() {
    glClear(GL_DEPTH_BUFFER_BIT);    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);


		buildDoor(&doorTexture);
		buildWindow();
		buildRoom();
		buildTable(&metalTexture, &woodTexture);
		buildBed(&woodTexture, &blanketTexture);
		buildShelf();
		buildCarpet(&carpetTexture);
		buildBoardVanGogh();		
}

void idle() {
	/*// Forward movement */
	int move_forward = KEYBOARD['w'] - KEYBOARD['s'];
	Vec3 fwd = forward(&CAM);
	
	fwd.x *= move_forward;
	/*fwd.y = 0.0f; // Projects fwd in the xz plane */
	fwd.z *= move_forward;

	/*// Lateral movement */
	int move_right = KEYBOARD['d'] - KEYBOARD['a'];
	Vec3 rgt = right(&CAM);

	rgt.x *= move_right;
	rgt.z *= move_right;
	
	CAM.position.x += 0.1f * (fwd.x + rgt.x);
	CAM.position.z += 0.1f * (fwd.z + rgt.z);

	glutPostRedisplay();
}

void motion(int x, int y) { 
	static int wrap = 0;
	Vec2 delta;
	
    if (!wrap) {
        delta.x = x - WINDOW_CENTER.x;
        delta.y = y - WINDOW_CENTER.y;

		CAM.rotation.x += delta.y > 0 ? 2.0f : (delta.y < 0 ? -2.0f : 0.0f);
		CAM.rotation.y -= delta.x > 0 ? 2.0f : (delta.x < 0 ? -2.0f : 0.0f);

        wrap = 1;
        glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);
    } else {
        wrap = 0;
    }
}

void keyboard(unsigned char key, int x, int y){
	if(key == 27)
		glutLeaveMainLoop();

	switch (key) {
  case 'a':
    /*printf("%d, %d\n",x,y);*/
    break;
  case 'p':
    eixoPortay = (eixoPortay + 5) % 360;
		if(eixoPortay > 180) eixoPortay = 180;
    glutPostRedisplay();
    break;
  case 'P':
    eixoPortay = (eixoPortay - 5) % 360;
		if(eixoPortay < 0) eixoPortay = 0;
    glutPostRedisplay();
    break;
	case 'j':
    eixoJanelay = (eixoJanelay + 5) % 360;
		if(eixoJanelay > 180) eixoJanelay = 180;
    glutPostRedisplay();
    break;
  case 'J':
    eixoJanelay = (eixoJanelay - 5) % 360;
		if(eixoJanelay < 0) eixoJanelay = 0;
    glutPostRedisplay();
    break;
	case 'f':
    velocidadeFan = velocidadeFan + 3;
		if(velocidadeFan > 12) velocidadeFan = 12;
    glutPostRedisplay();
    break;
	case 'F':
    velocidadeFan = velocidadeFan - 3;
		if(velocidadeFan < -12) velocidadeFan = -12;
    glutPostRedisplay();
    break;
	}

	KEYBOARD[tolower(key)] = 1;
}

void keyboard_up(unsigned char key, int x, int y){
	KEYBOARD[tolower(key)] = 0;
}

void reshape(int width, int height) {
	float aspect = (float) width / (float) height;
	WINDOW_SIZE.x = width;
	WINDOW_SIZE.y = height;
	WINDOW_CENTER.x = width / 2;
	WINDOW_CENTER.y = height / 2;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, aspect, ZNEAR, ZFAR);
}

/*// Drawing utils */
void draw_axis(int x, int y, int z) {
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	if(x) {
		glColor3f(RED);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(ZFAR, 0.0f, 0.0f);
	}
	if(y) {
		glColor3f(GREEN);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, ZFAR, 0.0f);
	}
	if(z) {
		glColor3f(BLUE);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, ZFAR);
	}
	
	glEnd();
	glLineWidth(1.0f);
}

void draw_grid(int n) {
	int i;

	glBegin(GL_LINES);
	glColor3f(WHITE);
	for(i = -n;i < n;i++){
		float d = (float) i;
		/*// Parallel to x-axis  */
		glVertex3f(-n, 0.0f, d);
		glVertex3f(n, 0.0f, d);
		/*// Parallel to z-axis */
		glVertex3f(d, 0.0f, -n);
		glVertex3f(d, 0.0f, n);
	}
	glEnd();
}
/*// Math utils */

Vec3 forward(Transform* t) {
	Vec3 v;
	float a = t->rotation.x * DEG2RAD;
	float b = t->rotation.y * DEG2RAD;
	float c = t->rotation.z * DEG2RAD;

	v.x = -(sin(c) * sin(a) + cos(c) * sin(b) * cos(a));
	v.y = -(-cos(c) * sin(a) + sin(c) * sin(b) * cos(a));
	v.z = -(cos(b) * cos(a));

	return v;
}

Vec3 up(Transform* t) {
	Vec3 v;
	float a = t->rotation.x * DEG2RAD;
	float b = t->rotation.y * DEG2RAD;
	float c = t->rotation.z * DEG2RAD;

	v.x = -sin(c) * cos(a) + cos(c) * sin(b) * sin(a);
	v.y = cos(c) * cos(a) + sin(c) * sin(b) * sin(a);
	v.z = cos(b) * sin(a);
	
	return v;
}

Vec3 right(Transform* t) {
	Vec3 v;
	float a = t->rotation.x * DEG2RAD;
	float b = t->rotation.y * DEG2RAD;
	float c = t->rotation.z * DEG2RAD;

	v.x = cos(c) * cos(b);
	v.y = sin(c) * cos(b);
	v.z = -sin(b);

	return v;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Room");
	glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);	

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutPassiveMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutReshapeFunc(reshape);

	tableLoader.LoadFile("./obj/table/table.obj");
	bedLoader.LoadFile("./obj/bed/bed.obj");
	chairLoader.LoadFile("./obj/chair/chair.obj");
	pillowLoader.LoadFile("./obj/pillow/pillow.obj");
	bookLoader.LoadFile("./obj/book/book.obj");
	cupLoader.LoadFile("./obj/glass/glass.obj");
	shelfLoader.LoadFile("./obj/shelf/shelf.obj");
	carpetLoader.LoadFile("./obj/carpet/carpet.obj");
	fanLoader.LoadFile("./obj/fan/fan.obj");

	unsigned int microsecond = 1000000;



	init_gl();

	CAM.position = (Vec3) {0.0f, 2.0f, 0.0f};
	CAM.rotation = (Vec3) {0.0f, 0.0f, 0.0f};

	glutMainLoop();

	return 0;
}
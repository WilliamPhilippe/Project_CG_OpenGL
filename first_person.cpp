#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#define BLACK 0.0f, 0.0f, 0.0f
#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define AZUL     0.0, 0.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0
#define AMARELO  1.0, 1.0, 0.0
#define VERDE    0.0, 1.0, 0.0
#define CYAN     1.0, 0.0, 1.0
#define LARANJA  0.8, 0.6, 0.1
#define ROSEO    0.7, 0.1, 0.6
#define CINZA    0.6, 0.6, 0.6

#define ORIGIN 0.0f, 0.0f, 0.0f
#define RIGHT 1.0f, 0.0f, 0.0f
#define UP 0.0f, 1.0f, 0.0f
#define FORWARD 0.0f, 0.0f, -1.0f

using std::cout;

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

Vec2 WINDOW_SIZE = {640, 480};
Vec2 WINDOW_CENTER = {320, 240};
float FOVY = 75.0f;
float ZNEAR = 10e-3;
float ZFAR = 10e3;
Transform CAM;
int KEYBOARD[128] = {0};
Vec2 MOTION;

char ch='1';

#define MAX_VERTICES 5000
int TABLE_VERTEX_COUNT;
Vec3 TABLE_VERTICES[MAX_VERTICES];
Vec3 TABLE_NORMALS[MAX_VERTICES];
Vec2 TABLE_TEX_COORDS[MAX_VERTICES];

int BED_VERTEX_COUNT;
Vec3 BED_VERTICES[MAX_VERTICES];
Vec3 BED_NORMALS[MAX_VERTICES];
Vec2 BED_TEX_COORDS[MAX_VERTICES];

int CHAIR_VERTEX_COUNT;
Vec3 CHAIR_VERTICES[MAX_VERTICES];
Vec3 CHAIR_NORMALS[MAX_VERTICES];
Vec2 CHAIR_TEX_COORDS[MAX_VERTICES];

int PILLOW_VERTEX_COUNT;
Vec3 PILLOW_VERTICES[MAX_VERTICES];
Vec3 PILLOW_NORMALS[MAX_VERTICES];
Vec2 PILLOW_TEX_COORDS[MAX_VERTICES];

void init_gl();
void display();
void idle();
void motion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);
void reshape(int width, int height);
void draw_walls();
void draw_axis(int x, int y, int z);
void draw_grid(int n);
void display2();
int load_obj(const char* path, int type);


Vec3 forward(Transform* t);
Vec3 right(Transform* t);
Vec3 up(Transform* t);


int main(int argc, char** argv) {
	glutInit(&argc, argv);

	// if(!load_obj("table-1.obj", 1 /*table*/)) {
	// 	perror("Erro ao abrir table");
	// 	return -1;
	// }

	if(!load_obj("bed-2.obj", 1 /*BED*/)) {
		perror("Erro ao abrir BED");
		return -1;
	}

	// if(!load_obj("pillow.obj", 3 /*pillow*/)) {
	// 	perror("Erro ao abrir pillow");
	// 	return -1;
	// }

	// if(!load_obj("chair.obj", 4 /*chair*/)) {
	// 	perror("Erro ao abrir chair");
	// 	return -1;
	// }

	glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Hue");
	glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);	

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutPassiveMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutReshapeFunc(reshape);

	init_gl();

	CAM.position = (Vec3) {0.0f, 2.0f, 0.0f};
	CAM.rotation = (Vec3) {-90.0f, 0.0f, 0.0f};

	
	
	glutMainLoop();

	return 0;
}



int load_obj(const char* path, int type) {
	FILE* fp = fopen(path, "r");

	if(!fp)
		return 0;

	char buffer[512] = "";
	int vertex_count = 0;
	int normal_count = 0;
	int tex_coord_count = 0;
	Vec3 vertices[MAX_VERTICES];
	Vec3 normals[MAX_VERTICES];
	Vec2 tex_coords[MAX_VERTICES];

	while(fgets(buffer, 512, fp)){
		if(buffer[0] == '#') /*/ Comment */
			continue;
		
		char* token = strtok(buffer, " ");
	

		if(strcmp(token, "v") == 0){
			
			vertices[vertex_count].x = atof(strtok(NULL, " "));
			vertices[vertex_count].y = atof(strtok(NULL, " "));
			vertices[vertex_count].z = atof(strtok(NULL, " "));
			vertex_count++;
		}
		else if(strcmp(token, "vn") == 0) {
			
			normals[normal_count].x = atof(strtok(NULL, " "));
			normals[normal_count].y = atof(strtok(NULL, " "));
			normals[normal_count].z = atof(strtok(NULL, " "));
			normal_count++;
		}
		else if(strcmp(token, "vt") == 0) {

			tex_coords[tex_coord_count].x = atof(strtok(NULL, " "));
			tex_coords[tex_coord_count].y = -atof(strtok(NULL, " "));
			tex_coord_count++;

		}
		else if(strcmp(token, "f") == 0) {
			int i;
			for(i = 0;i < 3;i++){
				
				switch (type)
				{
				case 1:
					TABLE_VERTICES[TABLE_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
					TABLE_TEX_COORDS[TABLE_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
					TABLE_NORMALS[TABLE_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
					TABLE_VERTEX_COUNT++;
					break;
				case 2:
					BED_VERTICES[BED_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
					BED_TEX_COORDS[BED_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
					BED_NORMALS[BED_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
					BED_VERTEX_COUNT++;
					break;
				case 3:
					PILLOW_VERTICES[PILLOW_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
					PILLOW_TEX_COORDS[PILLOW_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
					PILLOW_NORMALS[PILLOW_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
					PILLOW_VERTEX_COUNT++;
					break;
				case 4:
					CHAIR_VERTICES[CHAIR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
					CHAIR_TEX_COORDS[CHAIR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
					CHAIR_NORMALS[CHAIR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
					CHAIR_VERTEX_COUNT++;
					break;
				
				default:
					break;
				}
				
			}
		}
	}

	fclose(fp);

	return 1;
}

// void display2() {
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// 	gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
// 	float t = 1.0f * glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	
// 	int i;
// 	glBegin(GL_POLYGON);
// 	for(i = 0;i < VERTEX_COUNT;i++){
// 		glNormal3f(NORMALS[i].x, NORMALS[i].y, NORMALS[i].z);
// 		glVertex3f(VERTICES[i].x, VERTICES[i].y, VERTICES[i].z);
// 	}
// 	glEnd();

// 	glutSwapBuffers();
// }

void init_gl() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
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
	draw_walls();

	

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

static int eixoPortay, eixoJanelay;

void draw_walls() {
    glClear(GL_DEPTH_BUFFER_BIT);    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);


		glPushMatrix();
		glTranslatef(8.0, 0, -9.998);
		glRotatef ((GLfloat) eixoPortay, 0.0, 1.0, 0.0);
		
		glColor3f(RED);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, portaMovIndices);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, portaMovTrazIndices);

		glPopMatrix();

		glPushMatrix();
		glTranslatef(-9.998, 0, 3.0);
		glRotatef ((GLfloat) eixoJanelay, 0.0, 1.0, 0.0);

		glColor3f(AMARELO);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, janelaIndices);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, janelaTrazIndices);

		glPopMatrix();


		glColor3f(BLACK);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, molduraJanelaIndices);

		glColor3f(BLACK);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, portaIndices);

    glColor3f(BLUE);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, trasIndices);

    glColor3f(RED);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, leftIndices);

    glColor3f(AMARELO);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, frenteIndices);

		glColor3f(VERDE);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, rightIndices);

		glColor3f(LARANJA);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, tetoIndices);

		// glColor3f(CINZA);
    // glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, pisoIndices);

		glColor3f(RED);
		glDrawElements(GL_POLYGON, TABLE_VERTEX_COUNT, GL_UNSIGNED_BYTE, TABLE_VERTICES);

		int i;
		glBegin(GL_TRIANGLES);
		for(i = 0;i < TABLE_VERTEX_COUNT;i++){
			glNormal3f(TABLE_NORMALS[i].x, TABLE_NORMALS[i].y, TABLE_NORMALS[i].z);
			glVertex3f(TABLE_VERTICES[i].x*2 + 5.0, TABLE_VERTICES[i].y*2, TABLE_VERTICES[i].z*2 + 9.0);
		}

		glEnd();

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

		CAM.rotation.x += delta.y > 0 ? 1.0f : (delta.y < 0 ? -1.0f : 0.0f);
		CAM.rotation.y -= delta.x > 0 ? 1.0f : (delta.x < 0 ? -1.0f : 0.0f);

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
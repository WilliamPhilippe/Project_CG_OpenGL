#include <math.h>
#include <ctype.h>
#include <stdio.h>


#include <GL/gl.h>
#include <GL/freeglut.h>


/*// Colors */

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


/*// Math */

#define ORIGIN 0.0f, 0.0f, 0.0f
#define RIGHT 1.0f, 0.0f, 0.0f
#define UP 0.0f, 1.0f, 0.0f
#define FORWARD 0.0f, 0.0f, -1.0f


/*// Data types */

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


/*// Constants */

const float DEG2RAD = M_PI / 180.0f;
const float RAD2DEG = 180.0f / M_PI;


/*// Global variables */

Vec2 WINDOW_SIZE = {640, 480};
Vec2 WINDOW_CENTER = {320, 240};
float FOVY = 75.0f;
float ZNEAR = 10e-3;
float ZFAR = 10e3;
Transform CAM;
int KEYBOARD[128] = {0};
Vec2 MOTION;


void init_gl();


/*// Callbacks */

void display();

void idle();

void motion(int x, int y);

void keyboard(unsigned char key, int x, int y);

void keyboard_up(unsigned char key, int x, int y);

void reshape(int width, int height);


/*// Drawing utils */

void draw_axis(int x, int y, int z);

void draw_grid(int n);


/*// Math utils */

Vec3 forward(Transform* t);

Vec3 right(Transform* t);

Vec3 up(Transform* t);


int main(int argc, char** argv) {
	glutInit(&argc, argv);

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


void init_gl() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}


/*// Callbacks */

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*// View matrix */
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

static GLfloat vertices[72]={
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
	8.0, 0.0, -9.99, /* 12 */
	8.0, 4.0, -9.99, /* 13 */
	5.0, 4.0, -9.99, /* 14 */
	5.0, 0.0, -9.99,  /* 15 */

	/*vértices janela*/
	-9.99, 3.0, 5.0, /* 16 */
	-9.99, 1.0, 5.0, /* 17 */
	-9.99, 3.0, 3.0, /* 18 */
	-9.99, 1.0, 3.0, /* 19 */

	/*vértices da porta que se movimenta*/
	8.0, 0.0, -9.99, /* 20 */
	8.0, 4.0, -9.99, /* 21 */
	5.0, 4.0, -9.99, /* 22 */
	5.0, 0.0, -9.99,  /* 23 */
};

static GLubyte trasIndices[] = {1,2,3,0};
static GLubyte portaIndices[] = {12,13,14,15};
static GLubyte portaMovIndices[] = {20,21,22,23};
static GLubyte janelaIndeces[] = {16,17,19,18};
static GLubyte leftIndices[] = {4,3,2,5};
static GLubyte frenteIndices[] = {7,4,5,6};
static GLubyte rightIndices[] = {7,6,1,0};
static GLubyte tetoIndices[] = {5,2,1,6};
static GLubyte pisoIndices[] = {8,9,11,10};






static int eixoy, eixox;


void draw_walls() {
    glClear(GL_DEPTH_BUFFER_BIT);    
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);


		glPushMatrix();
		glRotatef ((GLfloat) eixoy, 0.0, 1.0, 0.0);
  	glRotatef ((GLfloat) eixox, 1.0, 0.0, 0.0);

		glColor3f(RED);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, portaMovIndices);
		glPopMatrix();
		
		
		
		glColor3f(BLACK);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, janelaIndeces);

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

		/*glColor3f(CINZA);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, pisoIndices);*/

		


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
    printf("%d, %d\n",x,y);
    break;
  case 'y':
    eixoy = (eixoy + 5) % 360;
    glutPostRedisplay();
    break;
  case 'Y':
    eixoy = (eixoy - 5) % 360;
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
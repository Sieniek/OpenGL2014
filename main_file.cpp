#include <iostream>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ball.h"

using namespace glm;

struct Element{
	Ball *object;
	Element *next = NULL;
};

struct Camera{
	float xRotation, yRotation, zRotation;
};
struct Screen{
	int width, height;
};

mat4 m, v, p;

//List of Elements
Element *list;

//Pink ball
Ball cursor = *new Ball();

//screen settings (auto-change in p-matrix)
Screen screen = {1200, 300};

//Camera rotation
Camera camera = {0, 0, 0};

void addToList(Ball *b){
	Element *newE = new Element;
	newE->object = b;
	newE->next = list;

	list = newE;
}

void keyDownHandler(int c, int x, int y){
}

void keyUpHandler(int c, int x, int y){	
}

void keyDownHandler(unsigned char c, int x, int y){
	//Camera rotation
			 if(c == 'q') camera.zRotation += 1;
	else if(c == 'e') camera.zRotation -= 1;
	else if(c == 'a') camera.xRotation += 1;
	else if(c == 'd') camera.xRotation -= 1;
	else if(c == 'w') camera.yRotation += 1;
	else if(c == 's') camera.yRotation -= 1;
	
	//Moving the cursor - pink ball
	else if(c == 'y') cursor.r += 0.025;
	else if(c == 'h') cursor.r -= 0.025;
	else if(c == 'l') cursor.x += 0.1;
	else if(c == 'j') cursor.x -= 0.1;
	else if(c == 'o') cursor.y += 0.1;
	else if(c == 'u') cursor.y -= 0.1;
	else if(c == 'i') cursor.z += 0.1;
	else if(c == 'k') cursor.z -= 0.1;
	//Add Pink Ball as Green Ball
	else if(c == ' ') addToList(new Ball(cursor.x, cursor.y, cursor.z, cursor.r, glutGet(GLUT_ELAPSED_TIME)));
	
	glutPostRedisplay();
	
}

void keyUpHandler(unsigned char c, int x, int y){	
}

void refresh(void){
	Element *e = list;
	
	while(e != NULL){
		e->object->update(glutGet(GLUT_ELAPSED_TIME));
		e = e -> next;
	}
	glutPostRedisplay();
}
inline void groundDraw(){
	glColor3d(0.2, 0.2, 0.2);
	for(float i = -20; i < 20; i++)
		for(float j = -20; j < 20; j++){
			glBegin(GL_QUADS);                      // Draw A Quad
        glVertex3f(i, j, f(i, j));              // Top Left
        glVertex3f( i + 0.95, j, f(i + 0.95, j));              // Top Right
        glVertex3f( i + 0.95,j + 0.95, f(i + 0.95, j + 0.95));              // Bottom Right
        glVertex3f(i,j + 0.95, f(i, j + 0.95));              // Bottom Left
   	 glEnd(); 
   }
}
void displayFrame(void) {

	glClearColor(0.0f, 0.3984375f, 0.796875f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//v-matrix settings
	v = lookAt(
		vec3(0.0f, 10.0f, 10.0f),
		vec3(0, 0, 1),
		vec3(0, 0, 1)//to jest wazne: patrzac od gory mamy kartezjanski ukl wsp: x i y, z jest wysokoscia!!!
		);
	
	//X/Y/Z-axis rotation
	v = rotate(v, camera.zRotation, vec3(.0f, 0.0f, 1.0f));
	v = rotate(v, camera.yRotation, vec3(.0f, 1.0f, .0f));
	v = rotate(v, camera.xRotation, vec3(1.0f, .0f, .0f));
	//p-matrix settings
	p = perspective(50.0f, screen.width / screen.height * 1.0f, 1.0f, 50.0f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(p));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(v * m));
	
	//ground draw
	groundDraw();
	//rysowanie obiektow
	glColor3d(1.0, 0.0, 1.0);
	glLoadMatrixf(value_ptr(v * translate(mat4(1.0f), vec3(cursor.x,cursor.y,cursor.z))));
	glutSolidSphere(cursor.r, 15, 13);
	
	glColor3d(0.0, 1.0, .0);
	Element e = *list;
	
	while(1){
		Ball b = *e.object;
		
		mat4 n = translate(mat4(1.0f), vec3(b.x, b.y, b.z));
		glLoadMatrixf(value_ptr(v * n));
		glutSolidSphere(b.r, 15, 13);
		
		if(!e.next) break;
		
		e = *e.next;
	}
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	
	addToList(new Ball(-3, 2, 4, 0.2));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(screen.width, screen.height);
  glutInitWindowPosition(40, 40);
  glutCreateWindow("Program Zaliczeniowy"); 
	//my code
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  
	//refresh-ing all physics
	glutIdleFunc(refresh);
  
	//obsluga klawiatury
	glutSpecialFunc(keyDownHandler);
	glutSpecialUpFunc(keyUpHandler);
	glutKeyboardFunc(keyDownHandler);
	glutKeyboardUpFunc(keyUpHandler);
	
	
	//Rysowanie	   
	glutDisplayFunc(displayFrame);

  glutMainLoop();
  return 0;
}

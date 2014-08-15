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
Element *list;

mat4 m,v,p;

Ball cursor = *new Ball();
//screen settings (auto-change in p-matrix)
int widthScreen = 1200, heightScreen = 300;


int xr=0,yr=0,zr = 0;
int x=0,y=0,z = 0;

void addToList(Ball *b){
	Element *newE = new Element;
	newE->object = b;
	newE->next = list;
	
	list = newE;
}

void keyDownHandler(int c, int x, int y){
	
}

void keyUpHandler(int c, int x, int y){
	switch(c){
		default:
			break;
	}
	glutPostRedisplay();
}
void keyDownHandler(unsigned char c, int x, int y){
	//Camera rotation
			 if(c == 'q') zr += 1;
	else if(c == 'e') zr -= 1;
	else if(c == 'a') xr += 1;
	else if(c == 'd') xr -= 1;
	else if(c == 'w') yr += 1;
	else if(c == 's') yr -= 1;
	
	//Moving the cursor - pink ball
	else if(c == 'y') cursor.r += 0.025;
	else if(c == 'h') cursor.r -= 0.025;
	else if(c == 'l') cursor.x += 0.1;
	else if(c == 'j') cursor.x -= 0.1;
	else if(c == 'o') cursor.y += 0.1;
	else if(c == 'u') cursor.y -= 0.1;
	else if(c == 'i') cursor.z += 0.1;
	else if(c == 'k') cursor.z -= 0.1;
	else if(c == ' ') addToList(new Ball(cursor.x, cursor.y, cursor.z, cursor.r, glutGet(GLUT_ELAPSED_TIME)));
	
	glutPostRedisplay();
	
}

void keyUpHandler(unsigned char c, int x, int y){
	
	switch(c){
		default:
			break;
	}
	glutPostRedisplay();
	
}

void refresh(void){
	Element *e = list;
	
	while(e != NULL){
			e->object->update(glutGet(GLUT_ELAPSED_TIME));
		e = e -> next;
	}
	glutPostRedisplay();
}
void displayFrame(void) {
	glClearColor(0.0f, 0.3984375f, 0.796875f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//v-matrix settings
	v = lookAt(
		vec3(1.0f, 5.0f, 7.0f),
		vec3(0, 0, 1),
		vec3(0, 0, 1)//to jest wazne: patrzac od gory mamy kartezjanski ukl wsp: x i y, z jest wysokoscia!!!
		);
	
	//X/Y/Z-axis rotation
	v = rotate(v, zr * 1.0f, vec3(.0f, 0.0f, 1.0f));
	v = rotate(v, yr * 1.0f, vec3(.0f, 1.0f, .0f));
	v = rotate(v, xr * 1.0f, vec3(1.0f, .0f, .0f));
	//p-matrix settings
	p = perspective(50.0f, widthScreen / heightScreen * 1.0f, 1.0f, 50.0f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(p));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(v * m));
	
	//ground draw
	glColor3d(0.2, 0.2, 0.2);
	mat4 o = scale(mat4(1.0f), vec3(100,100,0.01));
	glLoadMatrixf(value_ptr(v * o));
	glutSolidCube(1.0f);
	
	glColor3d(1.0, 1.0, 1.0);
	for(float i = -10; i < 12; i+=1)
		for(float j = -10; j < 20; j+=1){
			mat4 m = translate(mat4(1.0f), vec3(i, j, f(i, j)));
			glLoadMatrixf(value_ptr(v * m));
			glutSolidSphere(0.4,  4, 4);
		}
	//rysowanie obiektow
	
	glColor3d(1.0, .0, 1.0);
	
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
  glutInitWindowSize(widthScreen, heightScreen);
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

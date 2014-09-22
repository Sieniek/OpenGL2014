#include <iostream>
#include <vector>
//#include <GL/freeglut.h>//??
#include <ode/ode.h>
#include <string>
#include <cstdlib>
#include <ctime>

#include "tga.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "surface.h"

#define OBJ_COUNT 78 // BAD, BAD, BAD !!! sizeof, vectors count?

using namespace std;
using namespace glm;

enum ObjectType {BALL, PLANE};// maybe 2 vectors[]: 1 for Balls + 1 for Planes(planes will build up the surface)

struct Object {//class + constructor!!!
	dBodyID body;
	dGeomID geom;
	dMass mass;
	ObjectType type;
	float color[4];
};

struct DisplaySettings{
	int width, height;
};

// position
glm::vec3 position = glm::vec3( 100, 700, 400 );
// horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float FoV = 60.0f;
 
float speed = 1.0f; // 3 units / second
float mouseSpeed = 0.005f;
float zoomSpeed = 3.0f;

float geomTexCoords2[]={
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f
};

int deltaTime = 0;
int currentTime = 0;
int lastTime = 0;

float force = 5000.0f;

int disaperePoint = 5000;

float distanceFromCamera = 20.0f;

int xpos, ypos;

//////// TEXTURES

GLuint tex;
TGAImg img;

////////

void DrawObject(Object& ob);
static void nearCallback(void *data, dGeomID o1, dGeomID o2);
void Draw();
void Initialize();
void Timer(int iUnused);
void ballsCreate();
void surfaceCreate();
void createOneBall();


float terrainFunc(float x, float y){
	return x*x/200 + y*y/200;
}

DisplaySettings display = {800, 400};

dWorldID world;
dSpaceID space;
dJointGroupID cgroup;

bool mouseButton[] = {0,0,0,0};
bool keyPressed[] = {0,0,0,0};

mat4 v,p;

vector<Object> objects;

int object_count = 0;
//keyboard handlers
void keyDownHandler(int c, int x, int y){

}

void keyUpHandler(int c, int x, int y){

}

void MouseRoutine(int x, int y){	
	if(mouseButton[0] == 1) {
		xpos = x;
		ypos = y;
	}
}

void mouseButtonFunc(int button, int state, int x, int y) {	
	if (button == 0 && state != GLUT_UP) {
		mouseButton[0] = 1;	
	} else if(mouseButton[0] == 1 && state == GLUT_UP && button == 0) {
		mouseButton[0] = 0;	
		xpos = x;
		ypos = y;
	}
	if (button == GLUT_RIGHT_BUTTON && state != GLUT_UP) {
		mouseButton[1] = 1;
		createOneBall();
	} else if(mouseButton[1] == 1 && state == GLUT_UP && button == 0) {
		mouseButton[1] = 0;
	}
	if (button == 3 && state != GLUT_UP) {
		if(FoV > 30.0f)FoV-=zoomSpeed;	
	} else if(button == 4 && state != GLUT_UP) {
		if(FoV < 120.0f)FoV+=zoomSpeed;
	}
}

void createOneBall(){
	Object ball;
	ball.body = dBodyCreate(world);
	ball.type = BALL;
	dMassSetZero(&ball.mass);
	dMassSetSphereTotal(&ball.mass, 1, 10);
	dBodySetMass(ball.body, &ball.mass);

	ball.geom = dCreateSphere(space, 10);
	dGeomSetBody(ball.geom, ball.body);
	dGeomSetData(ball.geom, (void*)"ball");

	glm::vec3 direction(
    	cos(verticalAngle) * sin(horizontalAngle),
    	sin(verticalAngle),
    	cos(verticalAngle) * cos(horizontalAngle)
	);

	dGeomSetPosition(ball.geom, position.x + distanceFromCamera * direction.x, position.y + distanceFromCamera * direction.y, position.z + distanceFromCamera * direction.z);

	ball.color[0] = rand()%1000 * 0.001;
	ball.color[1] = rand()%1000 * 0.001;
	ball.color[2] = rand()%1000 * 0.001;
	ball.color[3] = 1.0f;	

	dBodyAddForce(ball.body, force*direction.x, force*direction.y, force*direction.z);

	objects.push_back(ball);
	object_count++;
}

void keyDownHandler(unsigned char c, int x, int y){
    if(c == 'w') {
    	keyPressed[0] = 1;
    } else if(c == 'a') {
    	keyPressed[1] = 1;
    } else if(c == 's') {
    	keyPressed[2] = 1;
    } else if(c == 'd') {
    	keyPressed[3] = 1;
    } else if(c == 27) {
    	dWorldDestroy(world);
  		dCloseODE();
    	exit(0);
    }

}

void keyUpHandler(unsigned char c, int x, int y){
	if(c == 'w') {
    	keyPressed[0] = 0;
    } else if(c == 'a') {
    	keyPressed[1] = 0;
    } else if(c == 's') {
    	keyPressed[2] = 0;
    } else if(c == 'd') {
    	keyPressed[3] = 0;
    }
}
//*************
static void nearCallback(void *data, dGeomID o1, dGeomID o2) {
	const int N = 120;
	dContact contact[N];

	for (int i=0; i < N; i++) {
		contact[i].surface.mode = dContactBounce;
		contact[i].surface.mu = dInfinity;
		//contact[i].surface.mu2 = 5000;//??
		contact[i].surface.bounce = 0.4;
		contact[i].surface.bounce_vel = 0.0;
	}
	int n =  dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));//it has to be replace if too many objects appear on stage (is O(N^2) - complex)

	for (int i = 0; i < n; i++) {
		dJointID c = dJointCreateContact(world, cgroup, &contact[i]);
		dJointAttach(c, dGeomGetBody(o1), dGeomGetBody(o2));
	}

}
void Draw() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	deltaTime = currentTime - lastTime;

	lastTime = currentTime;

	if (mouseButton[0]) {
		horizontalAngle += mouseSpeed * deltaTime/500.0f * float(display.width/2 - xpos );
		verticalAngle   += mouseSpeed * deltaTime/500.0f * float(display.height/2 - ypos );
	}	

	glm::vec3 direction(
    	cos(verticalAngle) * sin(horizontalAngle),
    	sin(verticalAngle),
    	cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
	    sin(horizontalAngle - 3.14f/2.0f),
	    0,
	    cos(horizontalAngle - 3.14f/2.0f)
	);

	if(keyPressed[0]) {
		position += direction * (deltaTime/1.0f) * speed; 
    } else if(keyPressed[1]) {
    	position -= right * (deltaTime/1.0f) * speed;
    } else if(keyPressed[2]) {
    	position -= direction * (deltaTime/1.0f) * speed;
    } else if(keyPressed[3]) {
    	position +=    right * (deltaTime/1.0f) * speed;
    }

	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//???????
	
	p = perspective(FoV, 1.0f * display.width / display.height , 40.0f, 5000.0f);

	v = lookAt( position,// y is a height
										position + direction,
										up);
	
	glLoadMatrixf(value_ptr(p));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(v));

	dSpaceCollide(space, 0, &nearCallback);

	dWorldQuickStep(world, 0.05);//much better with step = 0.02 als 0.1

	dJointGroupEmpty(cgroup);
	
	glClearColor(0.0f, 0.3984375f, 0.796875f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	int c = 0;

  for (int i=0; i<object_count; i++) {
  	const dReal *realP = dBodyGetPosition(objects[i].body);
  	if(objects[i].type!=PLANE && sqrt(realP[0]*realP[0] + realP[1]*realP[1] + realP[2]*realP[2]) > disaperePoint){
  		printf("Deleted object %d at %f %f %f\n", objects[i].type, realP[0], realP[1], realP[2]);
  		objects.erase(objects.begin() + i);  		
  		c++;
  	}
  	else
  		DrawObject(objects[i]);
  }

  object_count -= c;
  glFlush();
  glutSwapBuffers();
}

void Initialize() {
	glEnable(GL_NORMALIZE);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	
	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {1.0, 1.0, 1.0, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);	
	
	GLfloat qaLightPosition[]	= {100.0, 500.0, 100.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
	glEnable(GL_LIGHT0);

	char *fileName = "brick2c.tga";

	if (img.Load(fileName) == IMG_OK) {
		glGenTextures(1,&tex); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D,tex); //Przetwarzaj uchwyt tex

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		if (img.GetBPP()==24) //Obrazek 24bit
			glTexImage2D(GL_TEXTURE_2D,0,3,img.GetWidth(),img.GetHeight(),0, GL_RGB,GL_UNSIGNED_BYTE,img.GetImg());
		else if (img.GetBPP()==32)
			//Obrazek 32bit
			glTexImage2D(GL_TEXTURE_2D,0,4,img.GetWidth(),img.GetHeight(),0, GL_RGBA,GL_UNSIGNED_BYTE,img.GetImg());
		else {
			//Obrazek 16 albo 8 bit, takimi się nie przejmujemy
		}
	} else {
		exit(666);
	}

}
void Timer(int iUnused) {
    glutPostRedisplay();
    glutTimerFunc(1, Timer, 0);//??
}

/*****************/
void DrawObject(Object& ob){
	// materials setup per object basis
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ob.color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ob.color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ob.color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ob.color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
	
	if (ob.type == BALL){
		
		//glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
		
		const dReal *realP = dBodyGetPosition(ob.body);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr( v
		 * translate(mat4(1.0f), vec3(realP[0], realP[1], realP[2]))
		));

		glutSolidSphere(dGeomSphereGetRadius(ob.geom), 20, 20);
		

	} else{
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glShadeModel(GL_FLAT);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(v ));//*translate(mat4(1.0f),vec3(-200,-200,-500))* scale(mat4(1.0f), vec3(100.0f,10.0f,100.0f))));
		glBindTexture(GL_TEXTURE_2D,tex);
	
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
		glVertexPointer(3, GL_FLOAT, 0, Vertices);

		glTexCoordPointer( 2, GL_FLOAT, 0, geomTexCoords);
		glNormalPointer(GL_FLOAT, 0, Normals);
		//glColorPointer(3, GL_FLOAT, 0, Colors);
		//glDrawArrays(GL_LINES, 0, VertexCount);
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, gl_Indices);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}

}

void ballsCreate(){
	for (int i=0;i<OBJ_COUNT;i++) {
			Object ball;
			ball.body = dBodyCreate(world);
			ball.type = BALL;
			dMassSetZero(&ball.mass);
			dMassSetSphereTotal(&ball.mass, 1, 10);
			dBodySetMass(ball.body, &ball.mass);

			ball.geom = dCreateSphere(space, 10);
			dGeomSetBody(ball.geom, ball.body);
			dGeomSetData(ball.geom, (void*)"ball");
			dGeomSetPosition(ball.geom, rand()%400 -200.0f, 200.0 +i * 30.0, rand() % 200 - 100.0f);

			ball.color[0] = rand()%1000 * 0.001;
			ball.color[1] = rand()%1000 * 0.001;
			ball.color[2] = rand()%1000 * 0.001;
			ball.color[3] = 1.0f;

			objects.push_back(ball);
		}
		object_count = OBJ_COUNT;//to change
}
void surfaceCreate(){//to do: change plane to a custom surface 
	Object plane;
	plane.type = PLANE;
	
	plane.color[0]=0.6;
	plane.color[1]=0.3;
	plane.color[2]=0.0;
	plane.color[3]=1;

	dTriMeshDataID new_tmdata = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(new_tmdata, &Vertices[0], 3 * sizeof(float), VertexCount, 
				&Indices[0], IndexCount, 3 * sizeof(dTriIndex));


	plane.geom = dCreateTriMesh(space, new_tmdata, 0, 0, 0);
	objects.push_back(plane);
	object_count++;
}

dWorldID worldInit(){
	world = dWorldCreate();
  dWorldSetGravity(world, 0.0, -9.81, 0.0);
  dWorldSetERP(world, 0.9);
  dWorldSetCFM(world, 1e-4);
  dWorldSetLinearDamping(world, 0.00001);
  dWorldSetAngularDamping(world, 0.005);
  dWorldSetMaxAngularSpeed(world, 200);
  dWorldSetContactMaxCorrectingVel(world, 0.1);
  dWorldSetContactSurfaceLayer(world, 0.1);
  
  return world;
}

int main(int argc, char** argv) {
	srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(display.width, display.height);
  glutInitWindowPosition(200, 200);

  glutCreateWindow("OpenGL2.0 + ODE: Balls Physics");
  Initialize();
  dInitODE();

  world = worldInit();

  space = dSimpleSpaceCreate(0);
  cgroup = dJointGroupCreate(0);
  
  //keyboard handlers
	glutSpecialFunc(keyDownHandler);
	glutSpecialUpFunc(keyUpHandler);
	glutKeyboardFunc(keyDownHandler);
	glutKeyboardUpFunc(keyUpHandler);

	glutMotionFunc(MouseRoutine);
	glutWarpPointer(display.width/2,display.height/2);
	glutMouseFunc(mouseButtonFunc);

  
  // make some balls
  ballsCreate();
  //Create a surface
  surfaceCreate();
  
  glutDisplayFunc(Draw);
  Timer(0);
  
  glutMainLoop();
  dWorldDestroy(world);

  dCloseODE();
  glDeleteTextures(1,&tex);
  return 0;
}

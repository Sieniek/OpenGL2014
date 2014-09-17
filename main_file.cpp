#include <iostream>
#include <vector>
#include <GL/freeglut.h>//??
#include <ode/ode.h>
#include <string>
#include <cstdlib>
#include <ctime>

#include <GL/gl.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "surface.h"

#define OBJ_COUNT 78 // BAD, BAD, BAD !!! sizeof, vectors count?

using namespace std;
using namespace glm;
//* CONECTION: OpenGL + ODE*//
// 1 methods change vision(OpenGL) and physics(ODE)
enum ObjectType {BALL, PLANE};// maybe 2 vectors[]: 1 for Balls + 1 for Planes(planes will build up the surface)
/* to do!!!
class Object2{
	private:
	float color[3];
	public:
	dBodyID body;
	dGeomID geom;
	dMass mass;
	ObjectType type;
	
	
	void setColor(float r, float g, float b, float alpha){
		color[0] = r; 
		color[1] = g;
		color[2] = b;
		color[3] = alpha;
	}
	inline void setColor(){
			setColor(rand() % 1000 * 0.001, rand() % 1000 * 0.001, rand() % 1000 * 0.001, 1.0f);
	}
	Object2(ObjectType obj_type, dReal position[3], dReal rotation[3]){
		//body = dBodyCreate(world);
		type = obj_type;
		dGeomSetPosition(geom, position[0], position[1], position[2]);
	}		
	void init(){
		dMassSetZero(&a.mass);
		dMassSetSphereTotal(&a.mass, 1, 10);
		dBodySetMass(a.body, &a.mass);
		dGeomSetBody(a.geom, a.body);
		dGeomSetData(a.geom, (void*)"ball");
		dGeomSetPosition(a.geom, rand()%20 -10.0f, -200.0 -i*30.0, rand()%20 -10.0f);
	}
};

a.body = dBodyCreate(world);
			dMassSetZero(&a.mass);
			dMassSetSphereTotal(&a.mass, 1, 10);
			dBodySetMass(a.body, &a.mass);

			dGeomSetBody(a.geom, a.body);
			dGeomSetData(a.geom, (void*)"ball");
*/

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
float initialFoV = 45.0f;
 
float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

int deltaTime = 0;
int currentTime = 0;
int lastTime = 0;

int xpos, ypos;

void DrawObject(Object& ob);
static void nearCallback(void *data, dGeomID o1, dGeomID o2);
void Draw();
void Initialize();
void Timer(int iUnused);
void ballsCreate();
void surfaceCreate();
/** CUSTOM TRI–MESH **/
//const int VertexCount = 9;
//const int IndexCount = 8 * 3;

float terrainFunc(float x, float y){
	return x*x/200 + y*y/200;
}
/*
//how to??
float* generateVertices(int width, int lenght){
	float Vertices[20];
	index = 0
	for(int i = width / -2; i < width / 2; i++)
		for(int j = lenght / -2; j < lenght / 2; j++){
			Vertices[index++] = i;
			Vertices[index++] = j;
			Vertices[index++] = terrainFunc(i, j);
		}
	return Vertices;
}
int** generateIndices(int* Vertices, int index){
	int odeVertices[index/3][3];
	for(int i = 0; i < index / 3; i++)
		for(int j = 0; j < 3; j++)
			odeVertices[i][j] = Vertices[3 * i + j];		
			
	return odeVertices;
}*/
/*
const int VertexCount = 9;
const int IndexCount = 8 * 3;

float Vertices[VertexCount * 3] = {
	 -500,   500, 500,
	 		0,   500, 500,
	 	500, 	 500, 500,
	 -500,	 500,		0,
	    0,	 	 0,		0,
	 	500,	 500,		0,
	 -500,	 500,-500,
	  	0,	 500,-500,
	  500,	 500,-500
};
float Normals[] = {
	0,0.5,-0.5,
	0,0.5,-0.5,
	-0.5,0.5,0,
	-0.5,0.5,0,
	0,0.5,0.5,
	0,0.5,0.5,
	0.5,0.5,0,
	0.5,0.5,0,
	0.5,0.5,0,
};
unsigned int gl_Indices[IndexCount] = {//GL Indieces
	4,0,1,
	4,1,2,
	4,2,5,
	4,5,8,
	4,8,7,
	4,7,6,
	4,6,3,
	4,3,0
};
float Colors[] = {//Open GL material
	0.5, 0.5, 0.5,
	0.1, 0.5, 0.5,
	0.1, 0.5, 0.5,
	0.1, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
	0.5, 0.5, 0.5
};
dTriIndex Indices[IndexCount / 3][3] = {
	{4,0,1},
	{4,1,2},
	{4,2,5},
	{4,5,8},
	{4,8,7},
	{4,7,6},
	{4,6,3},
	{4,3,0}
};
*/
/** END TRI–MESH **/
DisplaySettings display = {800, 400};

dWorldID world;
dSpaceID space;
dJointGroupID cgroup;

bool mouseButton[] = {0,0};
bool keyPressed[] = {0,0,0,0};

mat4 v,p;

vector<Object> objects;

int object_count = 0;
//keyboard handlers
void keyDownHandler(int c, int x, int y){
}

void keyUpHandler(int c, int x, int y){
  glFlush();
  glutSwapBuffers();
}

void MouseRoutine(int x, int y){
	xpos = x;
	ypos = y;
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
  //glFlush();
  //glutSwapBuffers();
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

	horizontalAngle += mouseSpeed * deltaTime/500.0f * float(display.width/2 - xpos );
	verticalAngle   += mouseSpeed * deltaTime/500.0f * float(display.height/2 - ypos );

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
	
	p = perspective(90.0f, 1.0f * display.width / display.height , 40.0f, 5000.0f);

	v = lookAt( position,// y is a height
										position + direction,
										up);
	
	glLoadMatrixf(value_ptr(p));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(v));

	dSpaceCollide(space, 0, &nearCallback);

	dWorldQuickStep(world, 0.03);//much better with step = 0.02 als 0.1

	dJointGroupEmpty(cgroup);
	
	glClearColor(0.0f, 0.3984375f, 0.796875f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
  for (int i=0; i<object_count; i++) {
  	DrawObject(objects[i]);
  }
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
			
		glShadeModel(GL_FLAT);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(value_ptr(v ));//*translate(mat4(1.0f),vec3(-200,-200,-500))* scale(mat4(1.0f), vec3(100.0f,10.0f,100.0f))));
	
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
	
		glVertexPointer(3, GL_FLOAT, 0, Vertices);
		glNormalPointer(GL_FLOAT, 0, Normals);
		//glColorPointer(3, GL_FLOAT, 0, Colors);
		//glDrawArrays(GL_LINES, 0, VertexCount);
		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, gl_Indices);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
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

	glutPassiveMotionFunc(MouseRoutine);
	glutWarpPointer(display.width/2,display.height/2);
  
  // make some balls
  ballsCreate();
  //Create a surface
  surfaceCreate();
  
  glutDisplayFunc(Draw);
  Timer(0);
  
  glutMainLoop();
  dWorldDestroy(world);

  dCloseODE();
  return 0;
}

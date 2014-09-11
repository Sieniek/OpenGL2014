#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <ode/ode.h>
#include <string>
#include <cstdlib>
#include <ctime>


#include <GL/gl.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

#define OBJ_COUNT 78


// Heightfield ############################################################################################

#define	DEGTORAD			0.01745329251994329577f				//!< PI / 180.0, convert degrees to radians

#define DENSITY (5.0)		// density of all objects
#define GPB 3			// maximum number of geometries per body

#define HFIELD_WSTEP			15			// Vertex count along edge >= 2
#define HFIELD_DSTEP			31

#define HFIELD_WIDTH			REAL( 4.0 )
#define HFIELD_DEPTH			REAL( 8.0 )

#define HFIELD_WSAMP			( HFIELD_WIDTH / ( HFIELD_WSTEP-1 ) )
#define HFIELD_DSAMP			( HFIELD_DEPTH / ( HFIELD_DSTEP-1 ) )

dReal planes[]= // planes for a cube
  {
    1.0f ,0.0f ,0.0f ,0.25f,
    0.0f ,1.0f ,0.0f ,0.25f,
    0.0f ,0.0f ,1.0f ,0.25f,
    0.0f ,0.0f ,-1.0f,0.25f,
    0.0f ,-1.0f,0.0f ,0.25f,
    -1.0f,0.0f ,0.0f ,0.25f
    /*
    1.0f ,0.0f ,0.0f ,2.0f,
    0.0f ,1.0f ,0.0f ,1.0f,
    0.0f ,0.0f ,1.0f ,1.0f,
    0.0f ,0.0f ,-1.0f,1.0f,
    0.0f ,-1.0f,0.0f ,1.0f,
    -1.0f,0.0f ,0.0f ,0.0f
    */
  };
const unsigned int planecount=6;

dReal points[]= // points for a cube
  {
    0.25f,0.25f,0.25f,  //  point 0
    -0.25f,0.25f,0.25f, //  point 1

    0.25f,-0.25f,0.25f, //  point 2
    -0.25f,-0.25f,0.25f,//  point 3

    0.25f,0.25f,-0.25f, //  point 4
    -0.25f,0.25f,-0.25f,//  point 5

    0.25f,-0.25f,-0.25f,//  point 6
    -0.25f,-0.25f,-0.25f,// point 7
  };
const unsigned int pointcount=8;
unsigned int polygons[] = //Polygons for a cube (6 squares)
  {
    4,0,2,6,4, // positive X
    4,1,0,4,5, // positive Y
    4,0,1,3,2, // positive Z
    4,3,1,5,7, // negative X
    4,2,3,7,6, // negative Y
    4,5,4,6,7, // negative Z
  };
//----> Convex Object

// select correct drawing functions

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#define dsDrawConvex dsDrawConvexD
#define dsDrawTriangle dsDrawTriangleD
#endif


// some constants

#define NUM 100			// max number of objects
#define DENSITY (5.0)		// density of all objects
#define GPB 3			// maximum number of geometries per body
#define MAX_CONTACTS 64		// maximum number of contact points per body


// dynamics and collision objects

struct MyObject {
  dBodyID body;			// the body
  dGeomID geom[GPB];		// geometries representing this body

  // Trimesh only - double buffered matrices for 'last transform' setup
  dReal matrix_dblbuff[ 16 * 2 ];
  int last_matrix_index;
};

// Our heightfield geom
dGeomID gheight;
////////////////

dGeomID TriMesh1;
dGeomID TriMesh2;

//#############################################################################################


dWorldID world;
dBodyID ball_body;
dSpaceID space;
dGeomID ball_geom;
dGeomID plane_geom;



dMass ball_mass;
dJointGroupID cgroup;

float angle=0.0;

enum ObjectType {BALL, PLANE};

struct Object {
	dBodyID body;
	dGeomID geom;
	dMass mass;
	ObjectType type;
	float color[4];
};
vector<Object> objects;


static void nearCallback(void *data, dGeomID o1, dGeomID o2) {
	const int N = 120;
	dContact contact[N];

	for (int i=0;i<N;i++) {
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 5000;
		contact[i].surface.bounce = 1.0;
		contact[i].surface.bounce_vel = 0.0;
	}
	int n =  dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));

	for (int i = 0; i < n; i++) {
		dJointID c = dJointCreateContact(world,cgroup,&contact[i]);
		dJointAttach (c,dGeomGetBody(o1),dGeomGetBody(o2));
	}

}

dReal heightfield_callback( void* pUserData, int x, int z )
{
	dReal fx = ( ((dReal)x) - ( HFIELD_WSTEP-1 )/2 ) / (dReal)( HFIELD_WSTEP-1 );
	dReal fz = ( ((dReal)z) - ( HFIELD_DSTEP-1 )/2 ) / (dReal)( HFIELD_DSTEP-1 );

	// Create an interesting 'hump' shape
	dReal h = REAL( 1.0 ) + ( REAL( -16.0 ) * ( fx*fx*fx + fz*fz*fz ) );

	return h;
}

void DrawObject(Object& ob){//??????? banned functions
	// materials setup per object basis
	glMaterialfv(GL_FRONT, GL_AMBIENT, ob.color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ob.color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ob.color);
	if (ob.type == BALL){
	
			glMatrixMode(GL_MODELVIEW);
			
			glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
			const dReal *realP = dBodyGetPosition(ob.body);
	
			glLoadMatrixf(value_ptr(translate(mat4(1.0f), vec3(realP[0], realP[1], realP[2]))));
	
			glutSolidSphere(dGeomSphereGetRadius(ob.geom) , 20,20);

	} else if (ob.type == PLANE) {//any other objects?
		//clear from banned func
			dVector3 v;
			dGeomBoxGetLengths(ob.geom, v);
			dReal *x = (dReal *)&v;
			
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(value_ptr(scale(mat4(1.0f), vec3(x[0],x[1],x[2]))));
			
			
			glutSolidCube(1);
	}

}

void Draw() {
	dSpaceCollide(space,0,&nearCallback);

	dWorldQuickStep (world,0.01);

	dJointGroupEmpty(cgroup);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);//???????
	glLoadIdentity();
	// rotates local axis around y
	//glRotatef(angle, 0.0,1.0,0.0);
	if (angle>360.0) {
        angle=0.0;
    }
    angle+=1;
    glNormal3f(0.5, 0.5, 0.1);
    for (int i=0;i<OBJ_COUNT;i++) {
    	DrawObject(objects[i]);
    }

    /* drawing heightfield - TODO
  	/*
  	/*
  	/*
  	/*

  	const dReal* pReal = dGeomGetPosition( gheight );

	const dReal* RReal = dGeomGetRotation( gheight );

	// Set ox and oz to zero for DHEIGHTFIELD_CORNER_ORIGIN mode.
	int ox = (int) ( -HFIELD_WIDTH/2 );
	int oz = (int) ( -HFIELD_DEPTH/2 );

	  for ( int i = 0; i < HFIELD_WSTEP - 1; ++i ){
	  	for ( int j = 0; j < HFIELD_DSTEP - 1; ++j ){
				dReal a[3], b[3], c[3], d[3];

				a[ 0 ] = ox + ( i ) * HFIELD_WSAMP;
				a[ 1 ] = heightfield_callback( NULL, i, j );
				a[ 2 ] = oz + ( j ) * HFIELD_DSAMP;

				b[ 0 ] = ox + ( i + 1 ) * HFIELD_WSAMP;
				b[ 1 ] = heightfield_callback( NULL, i + 1, j );
				b[ 2 ] = oz + ( j ) * HFIELD_DSAMP;

				c[ 0 ] = ox + ( i ) * HFIELD_WSAMP;
				c[ 1 ] = heightfield_callback( NULL, i, j + 1 );
				c[ 2 ] = oz + ( j + 1 ) * HFIELD_DSAMP;

				d[ 0 ] = ox + ( i + 1 ) * HFIELD_WSAMP;
				d[ 1 ] = heightfield_callback( NULL, i + 1, j + 1 );
				d[ 2 ] = oz + ( j + 1 ) * HFIELD_DSAMP;

				dsDrawTriangle( pReal, RReal, a, c, b, 1 ); // TODO - change to triangle OpenGL drawing function
				dsDrawTriangle( pReal, RReal, b, c, d, 1 );	// TODO - change to triangle OpenGL drawing function
		}
	  }	
	 */

    glFlush();//nessesary??
    glutSwapBuffers();
}

void Initialize() {
	glClearColor(0.8, 0.9, 0.8, 0.0);
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


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-200.0, 200.0, 200.0, -200.0, 0.0, 1000.0);

	// when near is too low(i.e. 0), flickering occurs
	gluPerspective(45.0f,1.0f,40.0f,10000.0f);//???????

	gluLookAt (500.0, -400.0, 500, 0.0, 0.0, 0, 0,-0.5, 0);//???????

	GLfloat qaLightPosition[]	= {100.0, -600.0, -100.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
	glEnable(GL_LIGHT0);

}
void Timer(int iUnused) {
    glutPostRedisplay();
    glutTimerFunc(1, Timer, 0);
}

int main(int argc, char** argv) {
	srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(200, 200);

  glutCreateWindow("test");
  Initialize();
  dInitODE();

  world = dWorldCreate();
  dWorldSetGravity(world, 0.0, 9.81, 0.0);
  dWorldSetERP(world, 0.9);
  dWorldSetCFM(world, 1e-4);
  dWorldSetLinearDamping(world, 0.00001);
  dWorldSetAngularDamping(world, 0.005);
  dWorldSetMaxAngularSpeed(world, 200);
  dWorldSetContactMaxCorrectingVel(world,0.1);
  dWorldSetContactSurfaceLayer(world,0.1);

  space = dSimpleSpaceCreate(0);
  cgroup = dJointGroupCreate(0);
  // make some balls
  for (int i=0;i<OBJ_COUNT-1;i++) {
  	Object a;
  	a.body = dBodyCreate(world);
  	a.type = BALL;
  	dMassSetZero(&a.mass);
  	dMassSetSphereTotal(&a.mass, 1, 10);
  	dBodySetMass(a.body, &a.mass);

  	a.geom = dCreateSphere(space, 10);
  	dGeomSetBody(a.geom, a.body);
  	dGeomSetData(a.geom, (void*)"ball");
  	dGeomSetPosition(a.geom, rand()%20 -10.0f, -200.0 -i*30.0, rand()%20 -10.0f);

  	a.color[0] = rand()%1000 * 0.001;
  	a.color[1] = rand()%1000 * 0.001;
  	a.color[2] = rand()%1000 * 0.001;
  	a.color[3] = 1.0f;

  	objects.push_back(a);
  }
  Object plane;//here to change basic plane to custom surface created by math func
  plane.type = PLANE;
  plane.geom = dCreateBox (space, 500.0, 5, 500.0);

  dCreatePlane( space, 0, 0, 1, 0 ); // TODO - check what does that do

  /*	########### HEIGHTFIELD - TODO ############

  // our heightfield floor

	dHeightfieldDataID heightid = dGeomHeightfieldDataCreate();

	// Create an finite heightfield.
	dGeomHeightfieldDataBuildCallback( heightid, NULL, heightfield_callback,
		HFIELD_WIDTH, HFIELD_DEPTH, HFIELD_WSTEP, HFIELD_DSTEP,
		REAL( 1.0 ), REAL( 0.0 ), REAL( 0.0 ), 0 );

	// Give some very bounds which, while conservative,
	// makes AABB computation more accurate than +/-INF.
	dGeomHeightfieldDataSetBounds( heightid, REAL( -4.0 ), REAL( +6.0 ) );

	gheight = dCreateHeightfield( space, heightid, 1 );

	dVector3 pos;
	pos[ 0 ] = 0;
	pos[ 1 ] = 0;
	pos[ 2 ] = 0;

	// Rotate so Z is up, not Y (which is the default orientation)
	dMatrix3 R;
	dRSetIdentity( R );
	dRFromAxisAndAngle( R, 1, 0, 0, DEGTORAD * 90 );

	// Place it.
	dGeomSetRotation( gheight, R );
	dGeomSetPosition( gheight, pos[0], pos[1], pos[2] );

  */

  plane.color[0]=0.6;
  plane.color[1]=0.3;
  plane.color[2]=0.0;
  plane.color[3]=1.0;

	objects.push_back(plane);

  glutDisplayFunc(Draw);
  Timer(0);

  glutMainLoop();
  dWorldDestroy(world);
  dSpaceDestroy (space);

  /*	######## HEIGHTFIELD DESTROY - TODO ###########

  dGeomHeightfieldDataDestroy( heightid );

  */

  dCloseODE();
  return 0;
}

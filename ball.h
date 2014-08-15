#ifndef BALL_H
#define BALL_H

#include <math.h>


#include <GL/gl.h>
#include <GL/glut.h>

#define PI 3.14159265
#define G 9.80665

class Ball{//moze tylko struct?
	public://->private + get/set-ters
	float x,y,z;
	float m,ek,ep;
	float rx, ry, rz;
	float r;
	int lastTimeUpdate;
	float v[3];//velocity in 3D
	float a[3];//accelaration in 3D
	
	Ball();
	Ball(float x1, float y1, float z1, float r1, int time);
	Ball(float x1, float y1, float z1, float r1);
	void update(float time);
	void changeDirection();
};
//f: some math function on 2 variables: x, y; return z
float f(float a, float b);
float f1(float a, float b);
float distance(float a, float b);
float distance(float a, float b, float c);
int getSign(float f);
#endif

#include "ball.h"
#include <iostream>

//f: some math function on 2 variables: x, y; return z
float f(float a, float b){
 	return a*a/20 + b*b/20;
}
float f1(float a, float b){
 	return a/10 + b/10;
}


Ball::Ball(){
	x=y=z=0;
	m=ek=ep=0;
	r=rx=ry=rz=1;
	lastTimeUpdate = 0;
	v[0] = v[1] = v[2] = 0;
	a[0] = a[1] = 0;
	a[2] = -G;  
}

Ball::Ball(float x1, float y1, float z1, float r1, int time){
	x=x1;
	y=y1;
	z=z1;
	r=r1;
	
	m=ek=ep=0;
	rx=ry=rz=1;
	lastTimeUpdate = time;
	v[0] = v[1] = v[2] = 0;
	a[0] = a[1] = 0.0f;
	a[2] = -G;  
}
Ball::Ball(float x1, float y1, float z1, float r1){
	x=x1;
	y=y1;
	z=z1;
	r=r1;
	
	m=ek=ep=0;
	rx=ry=rz=1;
	lastTimeUpdate = 0;
	v[0] = v[1] = v[2] = 0;
	a[0] = a[1] = 0.0f;
	a[2] = -G;  
}
float distance(float a, float b){
	return sqrt(a*a+b*b);
}
float distance(float a, float b, float c){
	a = sqrt(a*a+b*b);
	return sqrt(a*a+c*c) + 1e-30;
}
int getSign(float f){
	return f < 0 ? -1 : 1;
}
void Ball::update(float time){
		if(a[2] < 0.1 && a[2] > -0.1){
		
			//std::cout << "END " <<"\n";
		return;//in future also direct[0,1] with AND
		}
		
		float interval =  (time - lastTimeUpdate) / 1000.0f;
		lastTimeUpdate  = time;
		if(interval > 100)interval = 100;
		v[0] += interval * a[0];  
		v[1] += interval * a[1];
		v[2] += interval * a[2];
		
		x	+= v[0] * interval;// delta s = a * t ^ 2 / 2
		y	+= v[1] * interval;
		z += v[2] * interval;
			//std::cout << "a0 -> "<<a[0] <<"\n";
			//std::cout << "v0 -> "<<v[0] <<"\n";
		
		if(z - r <= f(x, y) && v[2] <= 0){//pilka spada i ma sie odbic
			// div by 0 
			float alpha = acos(a[0]/distance(a[0], a[2]));//angle btwn Ball-vector and x/y axis
			float gamma = acos(x/distance(x,f(x,y)));//angle btwn Surface-vector and x/y axis
			float beta = alpha - gamma;
			
			//std::cout << "Beta : " <<beta <<"\n";
			//std::cout << "Gamma -> "<<gamma <<"\n";
			//std::cout << "Alpha >>" << alpha <<"\n";
			a[0] = 10*  -sin(beta) / PI;
			//if( alpha < PI/2 )v[0] *= -1;
			//****************************************************
			
			alpha = acos(a[1]/distance(a[1], a[2]));//angle btwn Ball-vector and x/y axis
			gamma = acos(y/distance(y,f(x,y)));//angle btwn Surface-vector and x/y axis
			//gamma = gamma ? gamma : PI / 2;
			beta = alpha - gamma;//
			
			std::cout << "Beta : " <<beta <<"\n";
			std::cout << "Gamma -> "<<gamma <<"\n";
			std::cout << "Alpha >>" << alpha <<"\n";
			a[1] = 10 * -sin(beta) / PI;
			//****************************************************
			
		
			v[2] *= -1;
			if(v[2] < 0.1){a[2] = v[2] = 0;} // piłka zatrzymuje się na stale (nie uwzgledniam kolizji z innymi kulkami)
			
		}else if(v[2] <= 0 && a[2] >= 0){//pilka w gorze zatrzymuje sie i zaczyna spadac
			v[2] = 0;
		}
}

#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <math.h>

// Global Variables
#define M 10
#define L 10
#define Ht .5
#define minNumHits 12
#define PI 3.14159265
#define LAND_VAL 9999.0


extern int h, w;
extern int width;
extern int length;
extern float scale;
extern float maxu, minu;
extern float maxv, minv;
extern float transx, transy;
extern int Idata[800][600][4];



void readData(char **argv);

class Vertex{
	float x, y;
	float u,v;
public:
	Vertex();
	Vertex(float x_coor, float y_coor, float u_val, float v_val);
	float getX();
	float getY();
	float getU();
	float getV();
	void setU(float u_val);
	void setV(float v_val);
	void setX(float x_coor);
	void setY(float y_coor);
};
#endif

/*
 * VectorField.cpp
 * 
 * Copyright 2015 Sanat <sanat@sanat-HP-Pavilion-dv6-Notebook-PC>
 *  
 */

#include "VectorField.h"

int val;
int h, w;
int width = 60;
int length = 90;
int pix_x, pix_y;
int png_width, png_height;

float scale = 1;
float maxu = -40, minu = 1000;
float maxv = -40, minv = 1000;
float transx = 0, transy = 0;

extern Vertex vertices[90][60];

/*
 * readData: read velocity field data from a file
 * 
 */
void readData(char **argv)
{
	char ch;
	int i,j;
	float x = 0.0, y=0, u=0, v=0;
	std::ifstream ifile;
	ifile.open(argv[1]);
	
	for(i=0; i < width; i++)
		for(j=0;j < length;j++)
		{
			ifile>>x>>ch>>y>>ch>>u>>ch>>v;
			vertices[j][i].setX(x);
			vertices[j][i].setY(y);
			vertices[j][i].setU(u);
			vertices[j][i].setV(v);
			if (u != LAND_VAL){
				if (maxu < u)
					maxu = u;
				if (minu > u)
					minu = u;
			}
			if (v != LAND_VAL){
				if (maxv < v)
					maxv = v;
				if (minv > v)
					minv = v;
			}
		}
	ifile.close();
}

/*
 * Vertex class - represents each point in the grid
 */

// Vertex constructor
Vertex::Vertex(){
	x = 0.0;
	y = 0.0;
	u = 0.0;
	v = 0.0;
}
// Vertex constructor
Vertex::Vertex(float x_coor, float y_coor, float u_val, float v_val){
	x = x_coor;
	y = y_coor;
	u = u_val;
	v =	v_val;
}

float Vertex::getX(){
	return x;
}

void Vertex::setX(float x_coor){
	x = x_coor;
}

float Vertex::getY(){
	return y;
}

void Vertex::setY(float y_coor){
	y = y_coor;
}

float Vertex::getU(){
	return u;
}

void Vertex::setU(float u_val){
	u = u_val;
}

float Vertex::getV(){
	return v;
}

void Vertex::setV(float v_val){
	v = v_val;
}

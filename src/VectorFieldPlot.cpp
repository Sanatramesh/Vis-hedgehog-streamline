/*
 * VectorFieldPlot.cpp
 * 
 * Copyright 2015 Sanat <sanat@sanat-HP-Pavilion-dv6-Notebook-PC>
 *  
 */

#include "VectorField.h"

Vertex vertices[90][60];
int flag = 0;
int numValid;
int hitdata[800][600];
int Idata[800][600][4];
int I1[4], T[4];
int curx, cury;
int validpt = 0;
float I[4];
float transVertex[3][2];
float plotu = 0.0, plotv = 0.0;

/*
 * initRendering: Setting up glut window and view
 * 
 */
void initRendering() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	::w = glutGet( GLUT_WINDOW_WIDTH );
    ::h = glutGet( GLUT_WINDOW_HEIGHT );
	gluOrtho2D(25, 125.0, -35.0, 35.0);
	glViewport(0,0, w, h );
}

/*
 * handleResize: Setting up glut window and view after window resize
 * 
 */
void handleResize(int weight, int height){
	if (height  == 0)
		height = 1;
	w = weight;
	h = height;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluOrtho2D(25, 125.0, -35.0, 35.0);
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, weight, height);
}

/*
 * interpolateUV: Bilinear interpolation for the values of u and v at (x, y) in the grid
 * 
 */ 
void interpolateUV(float x, float y){
	int lowerx = (int)(x - 29.5), lowery = (int)(y + 29.5);
	
	if ((lowerx < 90 && lowerx >= 0 && lowery < 60 && lowery >= 0)){
		
		// Interpolate U in the grid at (x,y)
		if (vertices[lowerx][lowery].getU()!= LAND_VAL && vertices[lowerx+1][lowery].getU()!=LAND_VAL &&
			vertices[lowerx][lowery+1].getU()!=LAND_VAL && vertices[lowerx+1][lowery+1].getU()!=LAND_VAL)
		plotu = (vertices[lowerx+1][lowery].getX() - x)*(vertices[lowerx][lowery+1].getY() - y)*vertices[lowerx][lowery].getU() +
				(x - vertices[lowerx][lowery].getX())*(vertices[lowerx][lowery+1].getY() - y)*vertices[lowerx+1][lowery].getU() +
				(vertices[lowerx+1][lowery].getX() - x)*(y - vertices[lowerx][lowery].getY())*vertices[lowerx][lowery+1].getU() +
				(x - vertices[lowerx][lowery].getX())*(y - vertices[lowerx][lowery].getY())*vertices[lowerx+1][lowery+1].getU();				
		else
			plotu = 0;
		
		// Interpolate U in the grid at (x,y)
		if (vertices[lowerx][lowery].getV()!=LAND_VAL && vertices[lowerx+1][lowery].getV()!=LAND_VAL &&
			vertices[lowerx][lowery+1].getV()!=LAND_VAL && vertices[lowerx+1][lowery+1].getV()!=LAND_VAL)
		plotv = (vertices[lowerx+1][lowery].getX() - x)*(vertices[lowerx][lowery+1].getY() - y)*vertices[lowerx][lowery].getV() +
				(x - vertices[lowerx][lowery].getX())*(vertices[lowerx][lowery+1].getY() - y)*vertices[lowerx+1][lowery].getV() +
				(vertices[lowerx+1][lowery].getX() - x)*(y - vertices[lowerx][lowery].getY())*vertices[lowerx][lowery+1].getV() +
				(x - vertices[lowerx][lowery].getX())*(y - vertices[lowerx][lowery].getY())*vertices[lowerx+1][lowery+1].getV();
		else
			plotv = 0;
	}
	else{
		plotu = 0;
		plotv = 0;
	}
}

/*
 * drawHedgePlot: Draw the hedgehog plot of the velocity field
 * 
 */
void drawHedgePlot(){
	//Clear information from last draw
	int i,j;
	float angle;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	
	glLoadIdentity();
	glScalef(scale, scale, scale);
	glColor3f(0, 0, 1);
	
	for (i=0;i<length-1;i++){
		for(j=0;j<width-1;j++){
			
			if ((vertices[i][j].getU() != LAND_VAL && vertices[i][j].getV() != LAND_VAL) &&
				(vertices[i+1][j].getU() != LAND_VAL && vertices[i+1][j].getV() != LAND_VAL) &&
				(vertices[i+1][j+1].getU() != LAND_VAL && vertices[i+1][j+1].getV() != LAND_VAL) &&
				(vertices[i][j+1].getU() != LAND_VAL && vertices[i][j+1].getV() != LAND_VAL)){
				
				angle = vertices[i][j].getV()/vertices[i][j].getU();
				angle = (vertices[i][j].getV()+vertices[i+1][j+1].getV()+vertices[i+1][j].getV()+vertices[i][j+1].getV())/
						(vertices[i][j].getU()+vertices[i+1][j+1].getU()+vertices[i+1][j].getU()+vertices[i][j+1].getU());
				
				glPushMatrix();
				glTranslatef(vertices[i][j].getX(),vertices[i][j].getY(),0);
				glRotatef(atanf(angle)*180/PI,0,0,1);
				glBegin(GL_QUADS);
				glColor3f(1 - (angle+15)/30,0,0);
				glVertex2f(-0.15, -0.15);
				glVertex2f(0, 0);
				glVertex2f(0.15, -0.15);
				glVertex2f(0, 0.6);
				glEnd(); //End quadrilateral coordinates
				glPopMatrix();
			}
		}
	}
	glFlush();
}

/*
 * drawStreamLines: Draw the hedgehog plot of the velocity field
 * 
 */
void drawStreamLines(){
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glPushMatrix();
	glScalef(scale, scale, 0);
	int numSeeds = 20000;
	int i,j, k;
	float x, y, x_idx, y_idx, nextx, nexty, dh, k1[2], k2[2];
	dh = 5;
	srand (time(NULL));
	std::cout<<"Stream Lines"<<std::endl;
	
	for ( i=0; i<numSeeds; i+=1){
			x_idx = rand()%90;
			y_idx = rand()%60;
			x = vertices[(int)x_idx][(int)y_idx].getX();
			y = vertices[(int)x_idx][(int)y_idx].getY();
			
			if (vertices[(int)x_idx][(int)y_idx].getU() == LAND_VAL && 
				vertices[(int)x_idx][(int)y_idx].getV() == LAND_VAL){
				continue;
			}
			
			for ( j=0; j<600; j++){ 
				interpolateUV(x, y);
				if (plotu == 0 && plotv == 0)
					break;
				k1[0] = plotu*dh/maxu;
				k1[1] = plotv*dh/maxv;
				interpolateUV(x+k1[0], y+k1[1]);
				if (plotu == 0 && plotv == 0)
					break;
				k2[0] = plotu*dh/maxu;
				k2[1] = plotv*dh/maxv;
				nextx = x + (k1[0] + k2[0])/2;
				nexty = y + (k1[1] + k2[1])/2;
				glBegin(GL_LINES);
				glColor3f(j/600.0, j/600.0, 1 - 2*j/600.0);
				glVertex2f(x, y);
				glVertex2f(nextx, nexty);
				glEnd(); //End quadrilateral coordinates
				x = nextx; y = nexty;
			}
	}
	glPopMatrix();
	glFlush();
}

/*
 * drawScene: Render the visualization(Hedgehog or streamlines).
 * 
 */
void drawScene(){
	if (flag)
		drawHedgePlot();
	else
		drawStreamLines();
}

/*
 * keyboard: add keys for interaction
 */
void keyboard(unsigned char key, int x, int y){
	if (key == char(27) || key == 'q'){
		std::cout<<"Quiting ..."<<std::endl;
		exit(-1);
	}
	else if (key  == 'z'){
		scale += 0.1;
	}
	else if (key == 'x'){
		scale -= 0.1;
	}
	else if (key == 'c')
		flag = (flag+1)%2;
	glutPostRedisplay();
}

/*
 * SpecialKeys: add special keys for interaction
 */
void SpecialKeys(int key, int x, int y){
	if (key == GLUT_KEY_LEFT){
		transx+=0.1;
	}
	else if (key == GLUT_KEY_UP){
		transy-=0.1;
	}
	else if (key == GLUT_KEY_RIGHT){
		transx-=0.1;
	}
	else if (key == GLUT_KEY_DOWN){
		transy+=0.1;
	}
	glutPostRedisplay();
}

/*
 * main: initialize glut window, keyboard, mouse, etc and start Mainloop
 */
int main(int argc, char **argv){

	if (argc <= 1){
		std::cout<<argc<<"Usage: ./vectorfield <file with path>"<<std::endl; 
		exit(-1);
	}
	readData(argv);
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("Color Map");
	initRendering(); //Initialize rendering
	//Set handler function for drawing
	glutDisplayFunc(drawScene);
	//Set handler function for window resize
	glutReshapeFunc(handleResize);
	//Set handler function for keypress
	glutKeyboardFunc(keyboard);
	//Set handler function for Special keypress
	glutSpecialFunc(SpecialKeys);
	//glutMouseFunc(mousekeys);
	glutMainLoop(); //Start the main loop.
	return 0; //This line is never reached glutMainLoop doesn't return.
}

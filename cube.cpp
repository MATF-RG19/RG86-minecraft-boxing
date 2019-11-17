#include "cube.hpp"
#include <math.h>
#include <iostream>



cube :: cube(double width, double height, double depth){
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->hit = false;
	this->movement = false; 	
}

void cube :: draw(){

    glPushMatrix();

    

    if(hit){
    	glTranslatef(Xcenter_of_rotation , Ycenter_of_rotation + 0.1, Zcenter_of_rotation+0.1);
        glRotatef(rotate_for , 1, 0, 0);
        glTranslatef(-Xcenter_of_rotation, -Ycenter_of_rotation-0.1, -Zcenter_of_rotation-0.1);
    }

    if(movement){
    	Xcenter_of_rotation += increase_X;
    	Ycenter_of_rotation += increase_Y;
    	Zcenter_of_rotation += increase_Z;
    	//glTranslatef(increase_X/translate_for, increase_Y/translate_for, increase_Z/translate_for);
    	Xcenter = Xcenter + increase_X ;
    	Ycenter = Ycenter + increase_Y ;
    	Zcenter = Zcenter + increase_Z ;
    }
   	 
	glColor3f(R, G, B);
    glTranslatef(Xcenter, Ycenter, Zcenter);
    glScalef(width, height, depth);
    
    glutSolidCube(1);

    glLineWidth(0.5);
    glColor3f(1, 1, 1);
    glutWireCube(1);

    glPopMatrix();
    
}

void cube :: cube_set_translate(double increase_X,double increase_Y,double increase_Z){
	this->increase_X = increase_X;
	this->increase_Y = increase_Y;
	this->increase_Z = increase_Z;
}

void cube :: cube_set_lh_hit(bool b){
	this->hit = b;
}

void cube :: cube_set_rotate(double Xcenter_of_rotation,double Ycenter_of_rotation, double Zcenter_of_rotation){

	this->Xcenter_of_rotation = Xcenter_of_rotation;
	this->Ycenter_of_rotation = Ycenter_of_rotation;
	this->Zcenter_of_rotation = Zcenter_of_rotation;

}

void cube :: cube_set_color(double R, double G, double B){
	this->R = R;
	this->G = G;
	this->B = B;
}
void cube :: cube_set_center(double Xcenter,double Ycenter,double Zcenter){
	this->Xcenter = Xcenter;
	this->Ycenter = Ycenter;
	this->Zcenter = Zcenter;
}

void cube :: cube_set_value(double width,double height,double depth){
	this->width = width;
	this->height = height;
	this->depth = depth;
}

cube :: ~cube(){};
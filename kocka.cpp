#include "kocka.hpp"
#include <iostream>


kocka :: kocka(double width,double heigth,double depth){
	this->width = width;
	this->height = height;
	this->depth = depth;
}

void kocka :: draw(){

	glColor3f(1, 0, 0);
    glScalef(this->width/2, this->height/2, this->depth/2);
    glTranslatef(0.1, 0.1, 0.1);
    glutSolidCube(1);

}

kocka :: ~kocka(){
	this->width = 1;
	this->height = 1;
	this->depth = 1;
};
kocka :: kocka(){};
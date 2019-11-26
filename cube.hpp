#include <stdlib.h>
#include <GL/glut.h>

class cube
{
public:
	//konstruktori
	cube(double width=1,double height=1,double depth=1);
	
	//set funkcije
	void cube_set_value(double width,double height,double depth);
	void cube_set_color(double R, double G, double B);
	void cube_set_center(double Xcenter,double Ycenter,double Zcenter);
	void cube_set_rotate(double Xcenter_of_rotation,double Ycenter_of_rotation, double Zcenter_of_rotation);
	void cube_set_translate(double increase_X,double increase_Y,double increase_Z);
	void cube_set_lh_hit(bool b);

	//get funkcije

	//ostale funkcije
	void draw();

	//destruktori
	~cube();

	//rotacije za udarac
	double Xcenter_of_rotation, Ycenter_of_rotation, Zcenter_of_rotation;
	double rotate_for;
	bool rotate_end = false;

	//translacije za kretanje
	double increase_X, increase_Y, increase_Z;
	double translate_for = 5;
	
	
	//uslovi
	bool hit;
	bool movement;


	//znacajne promenljive za iscrtavanje
	double Xcenter, Ycenter, Zcenter;
	double R,G,B;
	double width;
	double height;
	double depth;
};
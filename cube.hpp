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


	void rotate_me(double angle);

	//ostale funkcije
	void draw();

	//destruktori
	~cube();

	//rotacije za udarac i usmeravanje
	double Xcenter_of_rotation, Ycenter_of_rotation, Zcenter_of_rotation;
	double global_Xcenter, global_Ycenter, global_Zcenter;
	double rotate_for = 0;
	double active_angle = 0;
	bool rotate_end = false;
	double angle_of_rotation = 0;
	double direction_vector_X = 0;
	double direction_vector_Y = 0;
	double direction_vector_Z = 0;

	//translacije za kretanje
	double increase_X, increase_Y, increase_Z;
	double translate_for = 5;
	
	
	//uslovi
	bool hit;
	bool movement;
	bool guard = false;


	//znacajne promenljive za iscrtavanje
	double Xcenter, Ycenter, Zcenter;
	double previous_Xcenter, previous_Ycenter, previous_Zcenter;
	double R,G,B;
	double width;
	double height;
	double depth;
	double angle;
	double X_axes = 1, Z_axes = 0;
};
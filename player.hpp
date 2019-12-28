#include <stdlib.h>
#include <GL/glut.h>
#include "cube.hpp"

class player
{
public:
	//konstruktori
	player();

	//funkcije
	void draw_player();

	void move_up();
	void move_down();
	void move_left();
	void move_right();
	void punch_move(double x, double y);

	//funkcije za zavrsetak igrice
	void sky_launch();

	void translate_player();
	void redirect(double angle, double global_Xcenter, double global_Ycenter, double global_Zcenter);
	
	void guard_me(bool protect);

	void set_axes(double x, double z);

	//destruktori
	~player();

	//za pravljenje boksera koriscene su idealne razmere tela u odnosu na glavu
	//za glavu postavimo kocku neke duzine stranice
	//duzina tela = 3 glave, sirina tela = 2.5 glava, dubina tela = malo vece od glave
	//duzina noge = 3.5 glava, sirina noge = 1 glava, dubina noge = dubina tela
	//duzina ruke = 3.7 glava, sirina ruke < 0.5 glava, dubina ruke < dubina tela
	//figura(visina) idealnog coveka sastoji se iz 7.5 glava
	cube head;
	cube body;
	cube left_foot;
	cube right_foot;
	cube left_hand;
	cube right_hand;

	double health;


};
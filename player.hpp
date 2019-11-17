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
	
	double player_center;
};
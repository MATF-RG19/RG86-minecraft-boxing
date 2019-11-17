#include "player.hpp"

player :: player(){

    //glava
    head.cube_set_value(0.2, 0.2, 0.2);
    head.cube_set_color(1.0, 0, 0);
    head.cube_set_center(1.7, 1.85, 1.7);

    //telo
    body.cube_set_value(0.5 , 0.6, 0.3);
    body.cube_set_color(0, 1.0, 0);
    body.cube_set_center(1.7, 1.45, 1.7);
    
    //noge
    left_foot.cube_set_value(0.2, 0.7, 0.3);
    left_foot.cube_set_color(0.95, 0.95, 0);
    left_foot.cube_set_center(1.85, 0.8, 1.7);
    left_foot.cube_set_rotate(1.65, 1.02 , 1.55);

    right_foot.cube_set_value(0.2, 0.7, 0.3);
    right_foot.cube_set_color(0.95, 0.95, 0);
    right_foot.cube_set_center(1.55, 0.8, 1.7);
    right_foot.cube_set_rotate(1.57, 1.02 , 1.55);
    
    //ruke
    left_hand.cube_set_value(0.12, 0.7, 0.2);
    left_hand.cube_set_color(0, 0, 0);
    left_hand.cube_set_center(2.01, 1.4, 1.7);
    left_hand.cube_set_rotate(1.72, 1.52 , 1.55);

    right_hand.cube_set_value(0.12, 0.7, 0.2);
    right_hand.cube_set_color(0, 0, 0);
    right_hand.cube_set_center(1.39, 1.4, 1.7);
    right_hand.cube_set_rotate(1.52, 1.52 , 1.55);

    player_center = 1.7;;

}

void player :: draw_player(){

    head.draw();
    body.draw();
    left_foot.draw();
    right_foot.draw();
    left_hand.draw();
    right_hand.draw();

}

void player :: move_up(){
    head.cube_set_translate(-0.023, 0, -0.023);
    body.cube_set_translate(-0.023, 0, -0.023);
    left_foot.cube_set_translate(-0.023, 0, -0.023);
    right_foot.cube_set_translate(-0.023, 0, -0.023);
    left_hand.cube_set_translate(-0.023, 0, -0.023);
    right_hand.cube_set_translate(-0.023, 0, -0.023);
}

void  player :: move_down(){
    head.cube_set_translate(0.023, 0, 0.023);
    body.cube_set_translate(0.023, 0, 0.023);
    left_foot.cube_set_translate(0.023, 0, 0.023);
    right_foot.cube_set_translate(0.023, 0, 0.023);
    left_hand.cube_set_translate(0.023, 0, 0.023);
    right_hand.cube_set_translate(0.023, 0, 0.023);

}

void player :: move_left(){
    head.cube_set_translate(-0.023, 0, 0.023);
    body.cube_set_translate(-0.023, 0, 0.023);
    left_foot.cube_set_translate(-0.023, 0, 0.023);
    right_foot.cube_set_translate(-0.023, 0, 0.023);
    left_hand.cube_set_translate(-0.023, 0, 0.023);
    right_hand.cube_set_translate(-0.023, 0, 0.023);
}

void player :: move_right(){
    head.cube_set_translate(0.023, 0, -0.023);
    body.cube_set_translate(0.023, 0, -0.023);
    left_foot.cube_set_translate(0.023, 0, -0.023);
    right_foot.cube_set_translate(0.023, 0, -0.023);
    left_hand.cube_set_translate(0.023, 0, -0.023);
    right_hand.cube_set_translate(0.023, 0, -0.023);
}


player :: ~player(){};
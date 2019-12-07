#include "player.hpp"

player :: player(){

    //glava
    head.cube_set_value(0.2, 0.2, 0.2);
    head.cube_set_color(1.0, 0, 0);
    head.cube_set_center(0.7, 1.85, 0.7);

    //telo
    body.cube_set_value(0.5 , 0.6, 0.3);
    body.cube_set_color(0, 1.0, 0);
    body.cube_set_center(0.7, 1.45, 0.7);
    
    //noge
    left_foot.cube_set_value(0.2, 0.7, 0.3);
    left_foot.cube_set_color(0.95, 0.95, 0);
    left_foot.cube_set_center(0.85, 0.8, 0.7);
    left_foot.cube_set_rotate(0.65, 1.02 , 0.55);

    right_foot.cube_set_value(0.2, 0.7, 0.3);
    right_foot.cube_set_color(0.95, 0.95, 0);
    right_foot.cube_set_center(0.55, 0.8, 0.7);
    right_foot.cube_set_rotate(0.57, 1.02 , 0.55);
    
    //ruke
    left_hand.cube_set_value(0.12, 0.7, 0.2);
    left_hand.cube_set_color(0, 0, 0);
    left_hand.cube_set_center(1.01, 1.4, 0.7);
    left_hand.cube_set_rotate(1.72, 1.52 , 0.55);

    right_hand.cube_set_value(0.12, 0.7, 0.2);
    right_hand.cube_set_color(0, 0, 0);
    right_hand.cube_set_center(0.39, 1.4, 0.7);
    right_hand.cube_set_rotate(0.52, 1.52 , 0.55);

    player_center = 1.7;

}

void player :: draw_player(){

    head.draw();
    body.draw();
    left_foot.draw();
    right_foot.draw();
    left_hand.draw();
    right_hand.draw();

}

void player :: translate_player(){

        head.cube_set_value(0.2, 0.2, 0.2);
        head.cube_set_color(0.95, 0.95, 0);
        head.cube_set_center(0.7, 1.85, 1.7);

        //telo
        body.cube_set_value(0.5 , 0.6, 0.3);
        body.cube_set_color(1.0, 0, 0);
        body.cube_set_center(0.7, 1.45, 1.7);
        
        //noge
        left_foot.cube_set_value(0.2, 0.7, 0.3);
        left_foot.cube_set_color(0, 1, 0);
        left_foot.cube_set_center(0.85, 0.8, 1.7);
        left_foot.cube_set_rotate(1.65, 1.02 , 1.55);

        right_foot.cube_set_value(0.2, 0.7, 0.3);
        right_foot.cube_set_color(0, 1, 0);
        right_foot.cube_set_center(0.55, 0.8, 1.7);
        right_foot.cube_set_rotate(1.57, 1.02 , 1.55);
        
        //ruke
        left_hand.cube_set_value(0.12, 0.7, 0.2);
        left_hand.cube_set_color(0, 0, 0);
        left_hand.cube_set_center(1.01, 1.4, 1.7);
        left_hand.cube_set_rotate(1.72, 1.52 , 1.55);

        right_hand.cube_set_value(0.12, 0.7, 0.2);
        right_hand.cube_set_color(0, 0, 0);
        right_hand.cube_set_center(0.39, 1.4, 1.7);
        right_hand.cube_set_rotate(1.52, 1.52 , 1.55);

        player_center = 1.7;
    

}

void player :: redirect(double angle,double global_Xcenter, double global_Ycenter, double global_Zcenter){

    head.active_angle += angle;
    body.active_angle += angle;
    left_foot.active_angle += angle;
    right_foot.active_angle += angle;
    left_hand.active_angle += angle;
    right_hand.active_angle += angle;

    head.global_Xcenter = global_Xcenter;
    head.global_Ycenter = global_Ycenter;
    head.global_Zcenter = global_Zcenter;

    body.global_Xcenter = global_Xcenter;
    body.global_Ycenter = global_Ycenter;
    body.global_Zcenter = global_Zcenter;

    left_foot.global_Xcenter = global_Xcenter;
    left_foot.global_Ycenter = global_Ycenter;
    left_foot.global_Zcenter = global_Zcenter;

    right_foot.global_Xcenter = global_Xcenter;
    right_foot.global_Ycenter = global_Ycenter;
    right_foot.global_Zcenter = global_Zcenter;

    left_hand.global_Xcenter = global_Xcenter;
    left_hand.global_Ycenter = global_Ycenter;
    left_hand.global_Zcenter = global_Zcenter;

    right_hand.global_Xcenter = global_Xcenter;
    right_hand.global_Ycenter = global_Ycenter;
    right_hand.global_Zcenter = global_Zcenter;

}

void player :: guard_me(bool protect){

    this->left_hand.guard = protect;
    this->right_hand.guard = protect;

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
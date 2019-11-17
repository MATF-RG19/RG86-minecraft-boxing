#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include "player.hpp"

//dimenzije prozora
static int window_width, window_height;


static int left_hand_punch;
static int right_hand_punch; 
static int left_foot_move;
static int right_foot_move;
static int move_up;
static int move_down;
static int move_left;
static int move_right; 


//podesavanja tajmera
#define TIMER_ID 0
#define TIMER_INTERVAL 20


static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);

player p1;

int main(int argc, char **argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    
    glutInitWindowSize(840, 1200);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    //events
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);

    
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}


static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}




static void on_display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glViewport(0, 0, window_width, window_height);

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 50);

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            6.5, 6.5, 6.5,
            0, 0.2, 0,
            0, 1, 0
        );


   /* cube k1;
    k1.cube_set_value(3, 3, 3);
    k1.cube_set_color(0,1,0);
    k1.cube_set_center(0.8,0.8,0.8);
    k1.draw();
 */
/*
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(5,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,5,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,5);
    glEnd();
*/
    
    p1.draw_player();


    glutSwapBuffers();
}



static void on_timer_hand(int value)
{
    
    if (value != TIMER_ID)
        return;


    //udarac leve ruke
    if(p1.left_hand.rotate_for >= -120 && !p1.left_hand.rotate_end){
        p1.left_hand.rotate_for -= 10;
        if(p1.left_hand.rotate_for == -120)
            p1.left_hand.rotate_end = true;
    }
    else if(p1.left_hand.rotate_for <0){
        p1.left_hand.rotate_for += 10;
    }
    else{
        p1.left_hand.hit = false;
        p1.left_hand.rotate_for = 0;
        left_hand_punch = 0;
        p1.left_hand.rotate_end= false; 
    }

      if(p1.right_hand.rotate_for >= -120 && !p1.right_hand.rotate_end){
        p1.right_hand.rotate_for -= 10;
        if(p1.right_hand.rotate_for == -120)
            p1.right_hand.rotate_end = true;
    }
    else if(p1.right_hand.rotate_for <0){
        p1.right_hand.rotate_for += 10;
    }
    else{
        p1.right_hand.hit = false;
        p1.right_hand.rotate_for = 0;
        right_hand_punch = 0;
        p1.right_hand.rotate_end= false; 
    }




    glutPostRedisplay();

    if (left_hand_punch || right_hand_punch) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
    }
}
/*
static void on_timer_right_hand(int value)
{
    
    if (value != TIMER_ID)
        return;


    //udarac desne ruke
    if(p1.right_hand.rotate_for >= -120 && !p1.right_hand.rotate_end){
        p1.right_hand.rotate_for -= 10;
        if(p1.right_hand.rotate_for == -120)
            p1.right_hand.rotate_end = true;
    }
    else if(p1.right_hand.rotate_for <0){
        p1.right_hand.rotate_for += 10;
    }
    else{
        p1.right_hand.hit = false;
        p1.right_hand.rotate_for = 0;
        right_hand_punch = 0;
        p1.right_hand.rotate_end= false; 
    }


    glutPostRedisplay();

    if (right_hand_punch) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_right_hand, TIMER_ID);
    }
}
*/

static void on_timer_movement(int value)
{
    
    if (value != TIMER_ID)
        return;


    if(p1.right_hand.translate_for > 0 && (move_up || move_down || move_right || move_left)){
        p1.right_hand.translate_for -= 0.5;
        p1.left_hand.translate_for -= 0.5;
        p1.right_foot.translate_for -= 0.5;
        p1.left_foot.translate_for -= 0.5;
        p1.body.translate_for -= 0.5;
        p1.head.translate_for -= 0.5;
    }
    else{
        p1.head.movement = false;
        p1.body.movement = false;
        p1.left_foot.movement = false;
        p1.right_foot.movement = false;
        p1.left_hand.movement = false;
        p1.right_hand.movement = false;

        p1.right_hand.translate_for = 5;
        p1.left_hand.translate_for = 5;
        p1.right_foot.translate_for = 5;
        p1.left_foot.translate_for = 5;
        p1.body.translate_for = 5;
        p1.head.translate_for = 5;
        move_up = 0; 
        move_down = 0;
        move_right = 0;
        move_left = 0 ;
    }
    
    glutPostRedisplay();

    if (move_up || move_down || move_right || move_left) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
    }
}

static void on_timer_movement_legs(int value)
{
    
    if (value != TIMER_ID)
        return;

    if(p1.left_foot.rotate_for >= -50 && !p1.left_foot.rotate_end){
        p1.left_foot.rotate_for -= 5;
        if(p1.left_foot.rotate_for == -50)
            p1.left_foot.rotate_end = true;
    }
    else if(p1.left_foot.rotate_for <0){
        p1.left_foot.rotate_for += 5;
    }
    else{
        p1.left_foot.hit = false;
        p1.left_foot.rotate_for = 0;
        p1.left_foot.rotate_end= false; 
        left_foot_move = 0;
    }

    if(p1.right_foot.rotate_for <= 50 && !p1.right_foot.rotate_end){
        p1.right_foot.rotate_for += 5;
        if(p1.right_foot.rotate_for == 50)
            p1.right_foot.rotate_end = true;
    }
    else if(p1.right_foot.rotate_for >0){
        p1.right_foot.rotate_for -= 5;
    }
    else{
        p1.right_foot.hit = false;
        p1.right_foot.rotate_for = 0;
        p1.right_foot.rotate_end= false; 
        right_foot_move = 0;
    }
    
    glutPostRedisplay();

    if (left_foot_move || right_foot_move) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
    }
}


static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'g':
    case 'G':
        /* Pokrece se animacija. */
        if (!left_hand_punch) {
            p1.left_hand.hit = true;
            left_hand_punch = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
        }
        break;

    case 'h':
    case 'H':
        /* Pokrece se animacija.*/ 
        if (!right_hand_punch) {
            p1.right_hand.hit = true;
            right_hand_punch = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
        }
        break;


    case 'w':
    case 'W':
        /* Pokrece se animacija.*/ 
        if (!move_up) {
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            p1.move_up();
            move_up = 1;
            p1.head.movement = true;
            p1.body.movement = true;
            p1.left_foot.movement = true;
            p1.right_foot.movement = true;
            p1.left_hand.movement = true;
            p1.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
        }
        break;    
    
    case 's':
    case 'S':
        if (!move_down) {
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            p1.move_down();
            move_down = 1;
            p1.head.movement = true;
            p1.body.movement = true;
            p1.left_foot.movement = true;
            p1.right_foot.movement = true;
            p1.left_hand.movement = true;
            p1.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
        }
        break;

    case 'd':
    case 'D':
        /* Pokrece se animacija.*/ 
        if (!move_right) {
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            p1.move_right();
            move_right = 1;
            p1.head.movement = true;
            p1.body.movement = true;
            p1.left_foot.movement = true;
            p1.right_foot.movement = true;
            p1.left_hand.movement = true;
            p1.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
        }
        break;  

    case 'a':
    case 'A':
        /* Pokrece se animacija.*/ 
        if (!move_left) {
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            p1.move_left();
            move_left = 1;
            p1.head.movement = true;
            p1.body.movement = true;
            p1.left_foot.movement = true;
            p1.right_foot.movement = true;
            p1.left_hand.movement = true;
            p1.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
        }
        break; 


    }
}
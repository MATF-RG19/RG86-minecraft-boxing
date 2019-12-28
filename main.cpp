/***********Projekat Minecraft-boxing**********************
	Literatura:
		->kodovi sa casova iz racunarske grafike
		->internet:
			-https://community.khronos.org/ (poput man strana)
			-https://stackoverflow.com/
			-https://learnopengl.com/
			-https://docs.microsoft.com/en-us/windows/win32/opengl/gl-functions (spisak funkcija opengl-a)
***********************************************************/
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <time.h>
#include "player.hpp"
#include "image.h"
#include <unistd.h>

//dimenzije prozora
static int window_width, window_height;

#define FILENAME0 "textura.bmp"
// Identifikatori tekstura.
static GLuint names[5];

//centar ringa, za sada ne treba
double ring_centerX = 1.25;
double ring_centerZ = 1.25;

//kljucna promenljiva u projektu... sva usmeravanja se vrse prema njoj
double angle;

//parametri za kraj igrice
double animation_parameter = 0.0;
int animation_ongoing;
int who_should_dance;


void set_view();
void victory_dance();
void draw_spotlights();
void draw_power_bar();
void draw_ring();
void end_game();
void show_game_over();

//prvi igrac
player p1;
static int left_hand_punch;
static int right_hand_punch; 
static int left_foot_move;
static int right_foot_move;
static int move_up;
static int move_down;
static int move_left;
static int move_right; 
static int guard;

//drugi igrac
player p2;
static int left_hand_punch2;
static int right_hand_punch2; 
static int left_foot_move2;
static int right_foot_move2;
static int move_up2;
static int move_down2;
static int move_left2;
static int move_right2; 
static int guard2;


//podesavanja tajmera
#define TIMER_ID 0
#define TIMER_INTERVAL 15

//callback funkcije
static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_Upkeyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void glutSpecialInput(int key, int x, int y);

//funkcija za inicijalizaciju tekstura
static void initialize(void);

//funkcija za zavrsetak igrice
static void finish_him(int wich_player);

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);


    glutInitWindowSize(840, 1200);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    //dogadjaji
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_Upkeyboard);
    glutSpecialFunc(glutSpecialInput);
    //glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    
    
    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);

    //postavljanje svetlosti koje je globalno za sve objekte
    //kasnije ce svaki objekat dobiti koeficijente kako bi odbijao odgovarajucu boju
    GLfloat light_position[] = { 30, 150, -220, 0 };
    GLfloat light_ambient[] = { 0, 0, 0, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 1, 1, 1, 1 };

    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };

    
    //ukljucivanje svetla
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    p2.translate_player();
    p2.draw_player();

    initialize();

    //fullscreen mode
    glutFullScreen();
    glutMainLoop();

    return 0;
}


//funkcija po uzoru na funkciju inicijalizacije sa vezbi
static void initialize(void)
{
    //instanciranje objekta tipa image
    Image * image;

    glClearColor(0, 0, 0, 0);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    
    //inicijalizacija slike    
    image = image_init(0, 0);

    //trazimo da nam se obezbedi 1 id i smesti u niz names kako bismo koristili teksturu
    //niz se nalazi na ovom mestu jer sam planirao da ubacim 2 teksture, od toga sam
    //za sada odustao pa i nije neophodan niz, mogla je promenljiva
    glGenTextures(5, names);

    //ovde je bio problem konverzije string tipa u char*
    //verovatno zbog toga sto je biblioteka image pisana u c-u
    //kako bi radilo u image.c sam promenio da drugi argument fje image_read bude
    //konstantan char*
    //a onda sam na internetu pronasao metodu c_str() nad objektima tipa string
    std::string s = "tekstura.bmp";
    image_read(image, s.c_str());

    
    //podesavanja teksture
    //pocinjemo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //zavrsavamo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, 0);



    std::string s2 = "tekstura5.bmp";
    image_read(image, s2.c_str());
 
    //podesavanja teksture
    //pocinjemo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //zavrsavamo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, 0);

    std::string s4 = "tekstura3.bmp";
    image_read(image, s4.c_str());
 
    //podesavanja teksture
    //pocinjemo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //zavrsavamo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, 0);

    std::string s3 = "tekstura2.bmp";
    image_read(image, s3.c_str());
 
    //podesavanja teksture
    //pocinjemo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //zavrsavamo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, 0);

    std::string sg = "game_over.bmp";
    image_read(image, sg.c_str());
 
    //podesavanja teksture
    //pocinjemo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, names[4]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    //zavrsavamo rad nad teksturom
    glBindTexture(GL_TEXTURE_2D, 0);


    //uklanjamo dinamicki alociran objekat
    image_done(image);

}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}



void draw_spotlights(){

    //tribine
    // levi reflektor
    glBindTexture(GL_TEXTURE_2D, names[0]);
        glBegin(GL_QUADS);
            glNormal3f(0,1,0);

            glTexCoord2f(0.1, 0);
            glVertex3f(-5, 0, 8.3);
 
            glTexCoord2f(1,0);
            glVertex3f(-5, 0, -5);

            glTexCoord2f(1,0.95);
            glVertex3f(-6, 6, -5);

            glTexCoord2f(0.1,0.95);
            glVertex3f(-6, 6, 6);
        glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    //desni reflektor
    glBindTexture(GL_TEXTURE_2D, names[0]);
        glBegin(GL_QUADS);
            glTexCoord2f(1,0);
            glVertex3f(-7, 0, -5);

            glTexCoord2f(0.1,0);
            glVertex3f(7, 0, -5);

            glTexCoord2f(0,0.95);
            glVertex3f(7, 6, -6);

            glTexCoord2f(1,0.95);
            glVertex3f(-7, 6, -6);
        glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);


}

void draw_power_bar(){

	//globalna clipping ravan koja ce prekriti status barove kad se igrica zavrsi
	double clipping_plane3[] = {-1, 0, 0, -100000};
    glClipPlane(GL_CLIP_PLANE2, clipping_plane3);
    
    if(animation_parameter>0)
		glEnable(GL_CLIP_PLANE2);

    //postavljanje power-barova
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	//ucitavamo matricu identiteta jer gledamo kao da je nezavisan deo igrice
    	glLoadIdentity();
    	glMatrixMode(GL_PROJECTION);
    	glPushMatrix();
    		glLoadIdentity();
    		//prebacujemo se na 2D crtanje preko celog ekrana

			//kliping ravan za levi status-bar
			double dl = 450*450/636.4;
		    double clipping_plane[] = {-1/sqrt(2), -1/sqrt(2), 0, dl - p2.health};
		    
		    glClipPlane(GL_CLIP_PLANE0, clipping_plane);
    		glEnable(GL_CLIP_PLANE0);
    	
    		gluOrtho2D(0,window_width,0,window_height);
    		//glavna linija snage
		    glBindTexture(GL_TEXTURE_2D, names[3]);
		        glBegin(GL_POLYGON);
		            glNormal3f(0,0,1);
		            glTexCoord2f(0.1,0.4);
		            glVertex2f(100,60);

		            glTexCoord2f(1,0.4);
		            glVertex2f(400,60);

		            glTexCoord2f(1,0.8);
		            glVertex2f(400,85);

		            glTexCoord2f(0.1,0.8);
		            glVertex2f(100,85);
		        glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);

		    //sporedna linija snage
		    glBindTexture(GL_TEXTURE_2D, names[1]);
		        glBegin(GL_POLYGON);
		            glNormal3f(0,0,1);
		            glTexCoord2f(0.1,0.4);
		            glVertex2f(100,50);

		            glTexCoord2f(1,0.4);
		            glVertex2f(400,50);

		            glTexCoord2f(1,0.8);
		            glVertex2f(400,60);

		            glTexCoord2f(0.1,0.8);
		            glVertex2f(100,60);
		        glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);

		    glDisable(GL_CLIP_PLANE0);
		    //oblik sa strane
		    glBindTexture(GL_TEXTURE_2D, names[3]);
		        glBegin(GL_POLYGON);
		            glNormal3f(0,0,1);

		            glTexCoord2f(0.4,0);
		            glVertex2f(35,30);

		            glTexCoord2f(0.6,0);
		            glVertex2f(70,30);

		            glTexCoord2f(0.8,0.4);
		            glVertex2f(100,50);
		            
		            glTexCoord2f(0.8,0.6);
		            glVertex2f(100,85);

		            glTexCoord2f(0.6,0.8);
		            glVertex2f(70,105);

		            glTexCoord2f(0.4,0.8);
		            glVertex2f(35,105);

		        glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //desni power-bar
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	//ucitavamo matricu identiteta jer gledamo kao da je nezavisan deo igrice
    	glLoadIdentity();
    	glMatrixMode(GL_PROJECTION);
    	glPushMatrix();
    		glLoadIdentity();
    		//prebacujemo se na 2D crtanje preko celog ekrana

    		double dd = (-window_width +450) / sqrt(2);
    		double clipping_plane2[] = {1/sqrt(2), -1/sqrt(2), 0, dd-p1.health};
    		glClipPlane(GL_CLIP_PLANE1, clipping_plane2);
    		
    		glEnable(GL_CLIP_PLANE1);
    		gluOrtho2D(0,window_width,0,window_height);
    		//glavna linija snage
		    glBindTexture(GL_TEXTURE_2D, names[2]);
		        glBegin(GL_POLYGON);
		            glNormal3f(0,0,1);
		            glTexCoord2f(0.1,0.4);
		            glVertex2f(window_width-100,60);

		            glTexCoord2f(1,0.4);
		            glVertex2f(window_width-400,60);

		            glTexCoord2f(1,0.8);
		            glVertex2f(window_width-400,85);

		            glTexCoord2f(0.1,0.8);
		            glVertex2f(window_width-100,85);
		        glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);

		    //sporedna linija snage
		    glBindTexture(GL_TEXTURE_2D, names[1]);
		        glBegin(GL_POLYGON);
		            glNormal3f(0,0,1);
		            glTexCoord2f(0.1,0.4);
		            glVertex2f(window_width-100,50);

		            glTexCoord2f(1,0.4);
		            glVertex2f(window_width-400,50);

		            glTexCoord2f(1,0.8);
		            glVertex2f(window_width-400,60);

		            glTexCoord2f(0.1,0.8);
		            glVertex2f(window_width-100,60);
		        glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);
		    glDisable(GL_CLIP_PLANE1);

		    //oblik sa strane
		    glBindTexture(GL_TEXTURE_2D, names[2]);
		        glBegin(GL_POLYGON);
		            glNormal3f(0,0,1);

		            glTexCoord2f(0.4,0);
		            glVertex2f(window_width-35,30);

		            glTexCoord2f(0.6,0);
		            glVertex2f(window_width-70,30);

		            glTexCoord2f(0.8,0.4);
		            glVertex2f(window_width-100,50);
		            
		            glTexCoord2f(0.8,0.6);
		            glVertex2f(window_width-100,85);

		            glTexCoord2f(0.6,0.8);
		            glVertex2f(window_width-70,105);

		            glTexCoord2f(0.4,0.8);
		            glVertex2f(window_width-35,105);

		        glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    glPopMatrix();

     if(animation_parameter>0)
		glDisable(GL_CLIP_PLANE2);

}


void draw_ring(){

	//plava svetlost se najvise odbija od rina
    GLfloat ambient_coeffs[] = { 0.3, 0.6, 0.95, 1 };
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 1, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 30;

    //crvena svetlost se najvise odbija od stubova
    GLfloat ambient_coeffs1[] = { 0.9, 0.2, 0.1, 1 };
    GLfloat diffuse_coeffs1[] = { 1, 0.2, 0.2, 1 };
    GLfloat specular_coeffs1[] = { 1, 1, 1, 1 };

    //bela boja se najvise odbija od konopaca
    GLfloat ambient_coeffs2[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat diffuse_coeffs2[] = { 1, 0.9, 0.9, 1 };
    GLfloat specular_coeffs2[] = { 1, 1, 1, 1 };

    //crna boja za pod
    GLfloat ambient_coeffsC[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat diffuse_coeffsC[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffsC[] = { 1, 1, 1, 1 };

    

    //ring
    glPushMatrix();
        glColor3f(0, 0, 1);
        glScalef(7, 1, 7);
        glTranslatef(0.1, 0.1, 0.1);

        //primena materijala na ring
        glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
        glMaterialf(GL_FRONT,  GL_SHININESS, shininess);

        glutSolidCube(1);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs2);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs2);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs2);
        glMaterialf(GL_FRONT,  GL_SHININESS, shininess);

        glColor3f(1, 1, 1);
        glutWireCube(1);
     glPopMatrix();

     //primena materijala na sve stubove
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs1);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs1);
    glMaterialf(GL_FRONT,  GL_SHININESS, shininess);



     //stubovi ringa
     glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(-2.65, 1.2,-2.65);
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(1);
        glColor3f(1, 0, 1);

        glutWireCube(1);     
     glPopMatrix();

     glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(4.12, 1.2,-2.65);
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(1);
        glColor3f(1, 0, 1);
        glutWireCube(1);     
     glPopMatrix();

     glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(4.12, 1.2,4.12);
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(1);     
        glColor3f(1, 0, 1);
        glutWireCube(1);
     glPopMatrix();


     glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(-2.65, 1.2,4.12);
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(1);
        glColor3f(1, 0, 1);
        glutWireCube(1);     
     glPopMatrix();

    //primena materijala na sve konopce
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs2);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs2);
    glMaterialf(GL_FRONT,  GL_SHININESS, shininess);

     //konopci
    glPushMatrix();
        glLineWidth(5);
        glBegin(GL_LINES);
        glColor3f(1,1,1);

        //gornji konopci
        glVertex3f(-2.65, 1.75,4.12);
        glVertex3f(4.12, 1.75,4.12);

        glVertex3f(-2.65, 1.75,4.12);
        glVertex3f(-2.65, 1.75,-2.65);

        glVertex3f(-2.65, 1.75,-2.65);
        glVertex3f(4.12, 1.75,-2.65);


        glVertex3f(4.12, 1.75,-2.65);
        glVertex3f(4.12, 1.75,4.12);

        //donji konopci
        glVertex3f(-2.65, 1.25,4.12);
        glVertex3f(4.12, 1.25,4.12);

        glVertex3f(-2.65, 1.25,4.12);
        glVertex3f(-2.65, 1.25,-2.65);

        glVertex3f(-2.65, 1.25,-2.65);
        glVertex3f(4.12, 1.25,-2.65);


        glVertex3f(4.12, 1.25,-2.65);
        glVertex3f(4.12, 1.25,4.12);

        glEnd();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffsC);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffsC);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffsC);
    glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
    

}

void end_game(){

	glClearColor(0.0, 0.0, 0.0, 0.0);
	    
	    glMatrixMode(GL_MODELVIEW);
    	glPushMatrix();
    	
    	glLoadIdentity();
    	glMatrixMode(GL_PROJECTION);
    	glPushMatrix();
    		glLoadIdentity();
    		
    		gluOrtho2D(0,window_width,0,window_height);
	    	glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	    	glDisable(GL_LIGHT0);
	    	glDisable(GL_LIGHTING);
	      		
	      	double blend = (animation_parameter-200.0)/200.0;
	      	if(blend > 1)
	      		blend = 1;
			glColor4f(0.05,0.05,0.05,0.0+blend);
	        glBegin(GL_POLYGON);
	            glVertex2f(0,0);

	            glVertex2f(window_width,0);

	            glVertex2f(window_width, window_height);

	            glVertex2f(0, window_height);
	    	glEnd();

	    	glDisable(GL_BLEND);
	    	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
        

}

void show_game_over(){

	glClearColor(0.0, 0.0, 0.0, 0.0);
	    
	    glMatrixMode(GL_MODELVIEW);
    	glPushMatrix();
    	
    	glLoadIdentity();
    	glMatrixMode(GL_PROJECTION);
    	glPushMatrix();
    		glLoadIdentity();
    		
    		gluOrtho2D(0,window_width,0,window_height);
	    	glDisable(GL_LIGHT0);
	    	glDisable(GL_LIGHTING);
	      	
	      	glBindTexture(GL_TEXTURE_2D, names[4]);
	        glBegin(GL_POLYGON);
	        	glTexCoord2f(0.07,0);
	            glVertex2f(0,0);

	            glTexCoord2f(1,0);
	            glVertex2f(window_width,0);

	            glTexCoord2f(1,1);
	            glVertex2f(window_width, window_height);

	            glTexCoord2f(0.07,1);
	            glVertex2f(0, window_height);
	    	
	    	glEnd();
	      	glBindTexture(GL_TEXTURE_2D, 0);

	    	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
        
}


void set_view(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, window_width, window_height);

    
    //podesavanje perspektive
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 50);


    //postavljanje kamere
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            6.5, 6.5, 6.5,
            0, 0.2, 0,
            0, 1, 0
        );

}


static void on_display(void)
{
    
    set_view();

    // postavljmo iscrtavanja stvari na koje se ne primenjuje osvetljenje
    draw_spotlights();
    draw_power_bar();
    
    //nakon iscrtavanja stvari na koje se ne primenjuje osvetljenje, uklucujemo osvetljenje i crtamo
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    draw_ring();

    //iscrtavanje igraca
    p1.draw_player();
    p2.draw_player();

    //ako je neko pobedio pokrenut je tajmer za kraj, pa je animation_parameter pomereno preko 0
    if(animation_parameter >0 and (who_should_dance == 1 or who_should_dance == 2))
    	victory_dance();

    //zatamljenje
    if(animation_parameter > 200 and animation_parameter <450)
    	end_game();

    //iscrtavanje game over teksture
    if(animation_parameter > 350)
    	show_game_over();

    glutSwapBuffers();
}

void victory_dance(){

	if(who_should_dance == 1){
		//printf("I should dance");
		p1.body.Ycenter += sin(animation_parameter/6)/20;
		p1.head.Ycenter += sin(animation_parameter/6)/20;
		p1.left_hand.Ycenter += sin(animation_parameter/6)/20;		
		p1.right_hand.Ycenter += sin(animation_parameter/6)/20;
		p1.left_foot.Ycenter += sin(animation_parameter/6)/20;
		p1.right_foot.Ycenter += sin(animation_parameter/6)/20;

	}else{
		p2.body.Ycenter += sin(animation_parameter/6)/20;
		p2.head.Ycenter += sin(animation_parameter/6)/20;
		p2.left_hand.Ycenter += sin(animation_parameter/6)/20;		
		p2.right_hand.Ycenter += sin(animation_parameter/6)/20;
		p2.left_foot.Ycenter += sin(animation_parameter/6)/20;
		p2.right_foot.Ycenter += sin(animation_parameter/6)/20;

	}

}


//zavrsni tajmer
//pokrece animacioni parametar koji obavlja animacije za kraj igrice
static void on_timer_end(int value)
{
    
    if (value != TIMER_ID)
        return;

    animation_parameter += 1.0;

    glutPostRedisplay();
    if(animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_end, TIMER_ID);
    }

}


static void on_timer_hand(int value)
{
    
    if (value != TIMER_ID)
        return;

    double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

    srand(time(NULL));


    //udarac leve ruke
    //ruci kazemo da se rotira za odredjeni deo. Kako je pritiskom na dugme
    //promenljiva hit stavljena na true, prilikom iscrtavanja se proverava njena
    //vrednost. Kako je ona true, vrsi se rotacija za rotate_for deo
    //da bi ruka stala sa rotacijom u nekom trenutku imamo rotate_end promenljivu
    if(p1.left_hand.rotate_for >= -120 && !p1.left_hand.rotate_end){
        p1.left_hand.rotate_for -= 10;
        

        //detekcija udarca
        /**************************************************************
        naravno ako protivnik drzi gard mozemo da ga udaramo ali mu ne 
        nanosimo stetu. Takodje podrazumeva se da neki udarac moze da 
        "zaluta kroz gard i to se desava retko i nezavisno od toga koji
        je to pokusaj po redu bio(dakle rand())"
        ***************************************************************/

        if(rand()%10 == 1 or rand()%10 == 5){
	        	if(real_distance <0.4 and p2.left_hand.guard 
	        		and p1.left_hand.rotate_for == -20 and !animation_parameter){
	        	p1.left_hand.rotate_end =true;
	        	p2.health +=5;
	        	if(p2.health >=210)
	        		finish_him(2);
	    	}
        }

        //udarci su razvrstani po snazi u zavisnosti koliko smo blizu prisli
        //protivniku
        //najjaci udarac je kad smo bas blizu protivniku
        if(real_distance <0.4 and real_distance and !p2.left_hand.guard and 
        	p1.left_hand.rotate_for == -20 and !animation_parameter){
        	p1.left_hand.rotate_end =true;
        	p2.health +=15;
        	if(p2.health >=210)
	        		finish_him(2);       
    	}

    	//srednji udarac je kad smo na nekom optimalnom rastojanju
        if(real_distance <0.55 and real_distance > 0.4 and !p2.left_hand.guard and 
        	p1.left_hand.rotate_for == -60 and !animation_parameter){
        	p1.left_hand.rotate_end =true;
        	p2.health +=10;
        	if(p2.health >=210)
	        		finish_him(2);
    	}

    	//ako protivnika jedva mozemo da "okrznemo" smanjujemo mu malo snage
        if(real_distance <0.75 and real_distance > 0.55 and !p2.left_hand.guard and 
        	p1.left_hand.rotate_for == -90 and !animation_parameter){
        	p2.health +=5;
        	if(p2.health >=210)
	        		finish_him(2);
    	}



        //ako je rotacija dosla do kraja treba stati
        if(p1.left_hand.rotate_for == -120)
            p1.left_hand.rotate_end = true;
    }//vracamo ruku nazad
    else if(p1.left_hand.rotate_for <0){
        p1.left_hand.rotate_for += 10;
    }//resetujemo odgovarajuce vrednosti kako bi se pokret mogao ponoviti
    else{
        p1.left_hand.hit = false;
        p1.left_hand.rotate_for = 0;
        left_hand_punch = 0;
        p1.left_hand.rotate_end= false; 
    }

    //isto kao za left_hand :)
    if(p1.right_hand.rotate_for >= -120 && !p1.right_hand.rotate_end){
        p1.right_hand.rotate_for -= 10;


        //slucajan udarac kroz blok
        if(rand()%10 == 1 or rand()%10 == 5){
	        	if(real_distance <0.4 and p2.left_hand.guard
	        	 and p1.right_hand.rotate_for == -20){
	        	p1.right_hand.rotate_end =true;
	        	p2.health +=5;
	        	if(p2.health >=210)
	        		finish_him(2);
	    	}	
        }


        //detekcija udarca
        if(real_distance <0.4 and real_distance and !p2.left_hand.guard and 
        	p1.right_hand.rotate_for == -20 and !animation_parameter){
        	p1.right_hand.rotate_end =true;
        	p2.health +=15;
        	if(p2.health >=210)
	        		finish_him(2);
    	}

        if(real_distance <0.55 and real_distance > 0.4 and !p2.left_hand.guard and 
        	p1.right_hand.rotate_for == -60 and !animation_parameter){
        	p1.right_hand.rotate_end =true;
        	p2.health +=10;
        	if(p2.health >=210)
	        		finish_him(2);
    	}
        if(real_distance <0.75 and real_distance > 0.55 and !p2.left_hand.guard and 
        	p1.right_hand.rotate_for == -90 and !animation_parameter){
        	p2.health +=5;
        	if(p2.health >=210)
	        		finish_him(2);
    	}
        

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

//sve isto vazi za udarac drugog igraca
static void on_timer_hand2(int value)
{
    
    if (value != TIMER_ID)
        return;

    double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));


    srand(time(NULL));

    //udarac leve ruke
    //ruci kazemo da se rotira za odredjeni deo. Kako je pritiskom na dugme
    //promenljiva hit stavljena na true, prilikom iscrtavanja se proverava njena
    //vrednost. Kako je ona true, vrsi se rotacija za rotate_for deo
    //da bi ruka stala sa rotacijom u nekom trenutku imamo rotate_end promenljivu
    if(p2.left_hand.rotate_for >= -120 && !p2.left_hand.rotate_end){
        p2.left_hand.rotate_for -= 10;

        //slucajan udarac kroz blok
        if(rand()%10 == 1 or rand()%10 == 5){
	        	if(real_distance <0.4 and p1.left_hand.guard
	        	and p2.left_hand.rotate_for == -20){
	        	p2.left_hand.rotate_end =true;
	        	p1.health +=5;
	        	if(p1.health >=210)
	        		finish_him(1);
	    	}	
        }

        //detekcija udarca
        if(real_distance <0.4 and real_distance and !p1.left_hand.guard and 
        	p2.left_hand.rotate_for == -20 and !animation_parameter){
        	p2.left_hand.rotate_end =true;
        	p1.health +=15;
        	if(p1.health >=210)
	        		finish_him(1);
    	}

        if(real_distance <0.55 and real_distance > 0.4 and !p1.left_hand.guard and 
        	p2.left_hand.rotate_for == -60 and !animation_parameter){
        	p2.left_hand.rotate_end =true;
        	p1.health +=10;
        	if(p1.health >=210)
	        		finish_him(1);
    	}
        if(real_distance <0.75 and real_distance > 0.55 and !p1.left_hand.guard and 
        	p2.left_hand.rotate_for == -90 and !animation_parameter){
        	p1.health +=5;
        	if(p1.health >=210)
	        		finish_him(1);
	        }
        

        //ako je rotacija dosla do kraja treba stati
        if(p2.left_hand.rotate_for == -120)
            p2.left_hand.rotate_end = true;
    }//vracamo ruku nazad
    else if(p2.left_hand.rotate_for <0){
        p2.left_hand.rotate_for += 10;
    }//resetujemo odgovarajuce vrednosti kako bi se pokret mogao ponoviti
    else{
        p2.left_hand.hit = false;
        p2.left_hand.rotate_for = 0;
        left_hand_punch2 = 0;
        p2.left_hand.rotate_end= false; 
    }

    //isto kao za left_hand :)
    if(p2.right_hand.rotate_for >= -120 && !p2.right_hand.rotate_end){
        p2.right_hand.rotate_for -= 10;

        //slucajan udarac kroz blok
        if(rand()%10 == 1 or rand()%10 == 5){
	        	if(real_distance <0.4 and !p1.left_hand.guard and 
	        	p2.right_hand.rotate_for == -20){
	        	p2.right_hand.rotate_end =true;
	        	p1.health +=5;
	        	if(p1.health >=210)
	        		finish_him(1);
	    	}	
        }

        //detekcija udarca
        if(real_distance <0.4 and real_distance and !p1.left_hand.guard and 
        	p2.right_hand.rotate_for == -20 and !animation_parameter){
        	p2.right_hand.rotate_end =true;
        	p1.health +=15;
        	if(p1.health >=210)
	        		finish_him(1);
    	}

        if(real_distance <0.55 and real_distance > 0.4 and !p1.left_hand.guard and 
        	p2.right_hand.rotate_for == -60 and !animation_parameter){
        	p2.right_hand.rotate_end =true;
        	p1.health +=10;
        	if(p1.health >=210)
	        		finish_him(1);
    	}
        if(real_distance <0.75 and real_distance > 0.55 and !p1.left_hand.guard and 
        	p2.right_hand.rotate_for == -90 and !animation_parameter){
        	p1.health +=5;
        	if(p1.health >=210)
	        		finish_him(1);
    	}

        //ako je ruka dosla do kraja rotacije i treba da se vrati
        if(p2.right_hand.rotate_for == -120)
            p2.right_hand.rotate_end = true;
    }
    else if(p2.right_hand.rotate_for <0){
        p2.right_hand.rotate_for += 10;
    }
    else{
        p2.right_hand.hit = false;
        p2.right_hand.rotate_for = 0;
        right_hand_punch2 = 0;
        p2.right_hand.rotate_end= false; 
    }

    glutPostRedisplay();

    if (left_hand_punch2 || right_hand_punch2) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_hand2, TIMER_ID);
    }
}


static void on_timer_movement(int value)
{
    
    if (value != TIMER_ID)
        return;


    //racunanje ugla za koji treba rotirati igraca na osnovu skalarnog proizvoda
    //racuna se u odnosu na pocetni vektor 0,0,1
    double x = p2.body.Xcenter-p1.body.Xcenter;
    double z = p2.body.Zcenter - p1.body.Zcenter;

    angle = acos(z/(sqrt(x*x + z*z)));


    //odredjivanje orijentacije i prebacivanje iz radijana u stepene
    if(x > 0)
        angle =  180.0 * angle/M_PI;
    else
        angle = - 180.0 * angle/M_PI;



    //posto se svi delovi tela pomeraju zajedno, uzimam bilo koji deo za proveru
    //ovde sam uzeo desnu ruku i proveravam koliko je jos rotacije preostalo da se
    //izvrsi tako sto rotacija ide od 5 pa se smanjuje do 0 cime se postize efekat
    //glatkog pokreta. Dakle ako je pomeranje aktivirano za neku od starna 
    if(p1.right_hand.translate_for > 0 && (move_up || move_down || move_right || move_left)){
        //translate_for se sustinski kao promenljiva ne primenjuje ni u jednoj funkciji
        //njena uloga je da "broji" koliko je ostalo do kraja translacije
        p1.right_hand.translate_for -= 0.5;
        p1.left_hand.translate_for -= 0.5;
        p1.right_foot.translate_for -= 0.5;
        p1.left_foot.translate_for -= 0.5;
        p1.body.translate_for -= 0.5;
        p1.head.translate_for -= 0.5;
        p1.redirect(angle, p1.body.Xcenter, p1.body.Ycenter, p1.body.Zcenter);
        p2.redirect(angle, p2.body.Xcenter, p2.body.Ycenter, p2.body.Zcenter);
    }//kazemo da kretanje prestane
    else{
        p1.head.movement = false;
        p1.body.movement = false;
        p1.left_foot.movement = false;
        p1.right_foot.movement = false;
        p1.left_hand.movement = false;
        p1.right_hand.movement = false;

        //kako bi bilo moguce ponovo se kretati, vrednost "brojaca" translate_for
        //se resetuje na stare vrednosti
        p1.right_hand.translate_for = 5;
        p1.left_hand.translate_for = 5;
        p1.right_foot.translate_for = 5;
        p1.left_foot.translate_for = 5;
        p1.body.translate_for = 5;
        p1.head.translate_for = 5;
        //omogucujemo da se ponovo mogu aktivirati pokreti
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

    //objektu p1, njegovoj levoj nozi, saopstili smo da joj je neophodno postaviti
    //vrednost rotacije za neku vrednost. Ta vrednost se pri iscrtavanju, uz
    //odgovarajucu proveru da li je to neophodno postavlja i rotacija se vrsi
    //kako bi ruka "otisla i vratila se" neophodno je da ima kraj rotacije
    //rotacija se vrsi sve dok ne "dodje do kraja"
    if(p1.left_foot.rotate_for >= -50 && !p1.left_foot.rotate_end){
        //rotate for se koristi samo uz postavljanje vrednosti promenljive hit
        //na true. tako se pokrece udarac noge ali on pri pomeranju zapravo simulira
        //pokret nogu
        p1.left_foot.rotate_for -= 5;
        //ako treba prstati sa rotacijom
        if(p1.left_foot.rotate_for == -50)
            p1.left_foot.rotate_end = true;
    }//vracamo ruku unazad
    else if(p1.left_foot.rotate_for <0){
        p1.left_foot.rotate_for += 5;
    }//resetuju se vrednosti da bi omogucili ponavljanje pokreta
    else{
        p1.left_foot.hit = false;
        p1.left_foot.rotate_for = 0;
        p1.left_foot.rotate_end= false; 
        left_foot_move = 0;
    }

    //isto vazi kao za left_foot odozgo :)
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


//kao sto je bio slucaj za igraca jedan imamo tajmere koji pokrecu animacije za
//igraca 2
static void on_timer_movement2(int value)
{
    
    if (value != TIMER_ID)
        return;

    double x = p2.body.Xcenter-p1.body.Xcenter;
    double z = p2.body.Zcenter - p1.body.Zcenter;

    angle = acos(z/(sqrt(x*x + z*z)));

//    printf("%lf\n", angle);
    if(x > 0)
        angle =  180.0 * angle/M_PI;
    else
        angle = - 180.0 * angle/M_PI;


    //posto se svi delovi tela pomeraju zajedno, uzimam bilo koji deo za proveru
    //ovde sam uzeo desnu ruku i proveravam koliko je jos rotacije preostalo da se
    //izvrsi tako sto rotacija ide od 5 pa se smanjuje do 0 cime se postize efekat
    //glatkog pokreta. Dakle ako je pomeranje aktivirano za neku od starna 
    if(p2.right_hand.translate_for > 0 && (move_up2 || move_down2 || move_right2 || move_left2)){
        //translate_for se sustinski kao promenljiva ne primenjuje ni u jednoj funkciji
        //njena uloga je da "broji" koliko je ostalo do kraja translacije
        p2.right_hand.translate_for -= 0.5;
        p2.left_hand.translate_for -= 0.5;
        p2.right_foot.translate_for -= 0.5;
        p2.left_foot.translate_for -= 0.5;
        p2.body.translate_for -= 0.5;
        p2.head.translate_for -= 0.5;
        p1.redirect(angle, p1.body.Xcenter, p1.body.Ycenter, p1.body.Zcenter);
        p2.redirect(angle, p2.body.Xcenter, p2.body.Ycenter, p2.body.Zcenter);
    }//kazemo da kretanje prestane
    else{
        p2.head.movement = false;
        p2.body.movement = false;
        p2.left_foot.movement = false;
        p2.right_foot.movement = false;
        p2.left_hand.movement = false;
        p2.right_hand.movement = false;

        //kako bi bilo moguce ponovo se kretati, vrednost "brojaca" translate_for
        //se resetuje na stare vrednosti
        p2.right_hand.translate_for = 5;
        p2.left_hand.translate_for = 5;
        p2.right_foot.translate_for = 5;
        p2.left_foot.translate_for = 5;
        p2.body.translate_for = 5;
        p2.head.translate_for = 5;
        //omogucujemo da se ponovo mogu aktivirati pokreti
        move_up2 = 0; 
        move_down2 = 0;
        move_right2 = 0;
        move_left2 = 0 ;

    }
    
    glutPostRedisplay();

    if (move_up2 || move_down2 || move_right2 || move_left2) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_movement2, TIMER_ID);
    }
}

static void on_timer_movement_legs2(int value)
{
    
    if (value != TIMER_ID)
        return;

    //objektu p2, njegovoj levoj nozi, saopstili smo da joj je neophodno postaviti
    //vrednost rotacije za neku vrednost. Ta vrednost se pri iscrtavanju, uz
    //odgovarajucu proveru da li je to neophodno postavlja i rotacija se vrsi
    //kako bi ruka "otisla i vratila se" neophodno je da ima kraj rotacije
    //rotacija se vrsi sve dok ne "dodje do kraja"
    if(p2.left_foot.rotate_for >= -50 && !p2.left_foot.rotate_end){
        //rotate for se koristi samo uz postavljanje vrednosti promenljive hit
        //na true. tako se pokrece udarac noge ali on pri pomeranju zapravo simulira
        //pokret nogu
        p2.left_foot.rotate_for -= 5;
        //ako treba prstati sa rotacijom
        if(p2.left_foot.rotate_for == -50)
            p2.left_foot.rotate_end = true;
    }//vracamo ruku unazad
    else if(p2.left_foot.rotate_for <0){
        p2.left_foot.rotate_for += 5;
    }//resetuju se vrednosti da bi omogucili ponavljanje pokreta
    else{
        p2.left_foot.hit = false;
        p2.left_foot.rotate_for = 0;
        p2.left_foot.rotate_end= false; 
        left_foot_move2 = 0;
    }

    //isto vazi kao za left_foot odozgo :)
    if(p2.right_foot.rotate_for <= 50 && !p2.right_foot.rotate_end){
        p2.right_foot.rotate_for += 5;
        if(p2.right_foot.rotate_for == 50)
            p2.right_foot.rotate_end = true;
    }
    else if(p2.right_foot.rotate_for >0){
        p2.right_foot.rotate_for -= 5;
    }
    else{
        p2.right_foot.hit = false;
        p2.right_foot.rotate_for = 0;
        p2.right_foot.rotate_end= false; 
        right_foot_move2 = 0;
    }
    
    glutPostRedisplay();

    if (left_foot_move2 || right_foot_move2) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs2, TIMER_ID);
    }
}


//tajmer za drzanje bloka igraca
static void on_timer_guard2(int value)
{
    
    if (value != TIMER_ID)
        return;


    //izgleda kao pokret ruke uz dodatak provere da li ruku treba saviti u gard
    if(p2.left_hand.rotate_for >= -100 && !p2.left_hand.rotate_end && 
            p2.left_hand.guard){
        p2.left_hand.rotate_for -= 10;
        
        p2.redirect(angle, p2.body.Xcenter, p2.body.Ycenter, p2.body.Zcenter);
        //ako je rotacija dosla do kraja treba stati
        if(p2.left_hand.rotate_for == -100)
            p2.left_hand.rotate_end = true;
    }//vracamo ruku nazad ako smo prekinuli drzanje bloka
    else if(p2.left_hand.rotate_for <0){
        if(!p2.left_hand.guard)
            p2.left_hand.rotate_for += 10;
    }//resetujemo odgovarajuce vrednosti kako bi se pokret mogao ponoviti
    else{
        p2.left_hand.hit = false;
        p2.left_hand.rotate_for = 0;
        left_hand_punch2 = 0;
        guard2 = 0;
        p2.left_hand.rotate_end= false; 
    }

    //isto kao za left_hand :)
    if(p2.right_hand.rotate_for >= -100 && !p2.right_hand.rotate_end &&
            p2.right_hand.guard){
        p2.right_hand.rotate_for -= 10;
        if(p2.right_hand.rotate_for == -100)
            p2.right_hand.rotate_end = true;
    }
    else if(p2.right_hand.rotate_for <0){
        if(!p2.right_hand.guard)
            p2.right_hand.rotate_for += 10;
    }
    else{
        p2.right_hand.hit = false;
        p2.right_hand.rotate_for = 0;
        right_hand_punch2 = 0;
        guard2 = 0;
        p2.right_hand.rotate_end= false; 
    }



    glutPostRedisplay();

    if (guard2) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_guard2, TIMER_ID);
    }
}


//tajmer funkcija kao za prethodnog igraca
static void on_timer_guard(int value)
{
    
    if (value != TIMER_ID)
        return;


    //izgleda kao pokret ruke uz dodatak provere da li ruku treba saviti u gard
    if(p1.left_hand.rotate_for >= -100 && !p1.left_hand.rotate_end && 
            p1.left_hand.guard){
        p1.left_hand.rotate_for -= 10;
        
        p1.redirect(angle, p1.body.Xcenter, p1.body.Ycenter, p1.body.Zcenter);
        //ako je rotacija dosla do kraja treba stati
        if(p1.left_hand.rotate_for == -100)
            p1.left_hand.rotate_end = true;
    }//vracamo ruku nazad ako smo prekinuli drzanje bloka
    else if(p1.left_hand.rotate_for <0){
        if(!p1.left_hand.guard)
            p1.left_hand.rotate_for += 10;
    }//resetujemo odgovarajuce vrednosti kako bi se pokret mogao ponoviti
    else{
        p1.left_hand.hit = false;
        p1.left_hand.rotate_for = 0;
        left_hand_punch = 0;
        guard = 0;
        p1.left_hand.rotate_end= false; 
    }

    //isto kao za left_hand :)
    if(p1.right_hand.rotate_for >= -100 && !p1.right_hand.rotate_end &&
            p1.right_hand.guard){
        p1.right_hand.rotate_for -= 10;
        if(p1.right_hand.rotate_for == -100)
            p1.right_hand.rotate_end = true;
    }
    else if(p1.right_hand.rotate_for <0){
        if(!p1.right_hand.guard)
            p1.right_hand.rotate_for += 10;
    }
    else{
        p1.right_hand.hit = false;
        p1.right_hand.rotate_for = 0;
        right_hand_punch = 0;
        guard = 0;
        p1.right_hand.rotate_end= false; 
    }

     glutPostRedisplay();

    if (guard) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_guard, TIMER_ID);
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
        if (!left_hand_punch and !p1.right_hand.guard) {
            //kazemo da se vrednost za udarac leve ruke stavi na true
            //u svakom iscrtavanju se proverava da li je ta vrednost true
            //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
            p1.left_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            left_hand_punch = 1;
            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju ruka jednog igraca
            // i centar tela drugog igraca
            if(p1.body.Zcenter < p2.body.Zcenter)
            	p1.set_axes(p2.body.Zcenter - p1.left_hand.Zcenter,
                     	  -p2.body.Xcenter + p1.left_hand.Xcenter);
            else
            	p1.set_axes(-p1.body.Zcenter + p2.left_hand.Zcenter,
                     	  +p1.body.Xcenter - p2.left_hand.Xcenter);

            glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
        }
        break;

    case 'h':
    case 'H':
        //kazemo da se vrednost za udarac desne ruke stavi na true
        //u svakom iscrtavanju se proverava da li je ta vrednost true
        //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
        if (!right_hand_punch and !p1.right_hand.guard) {
            p1.right_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            right_hand_punch = 1;
            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju ruka jednog igraca
            //i telo protivnika
            //u zavisnosti od pozicije igraca treba namestiti vektor(nisam siguran
            //zasto ovo bas radi ovako tj zasto zavisi od pozicija igraca ali
            //ovako radi :) )
            if(p1.body.Zcenter < p2.body.Zcenter)
            	p1.set_axes(p2.body.Zcenter - p1.right_hand.Zcenter,
                       	-p2.body.Xcenter + p1.right_hand.Xcenter);
            else
            	p1.set_axes(-p1.body.Zcenter + p2.right_hand.Zcenter,
                       	p1.body.Xcenter - p2.right_hand.Xcenter);

            glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
        }
        break;


    case 'w':
    case 'W':{


    	/********************************************
          Ovaj deo koda vezan za udaljenost je dugo imao bag
          NAZALOST otklonio sam bag uvodjenjem real_distnace promenljive
          Da nisam otklonio bag napravio bih da je to ustvari klinc u kojem
          mogu bokseri stvarno da se nadju. Obzirom da sam otklonio bag, i nemam
          vremena da pravim "klinc", ipak ce ostati bez te opcije
          "It's not a bug it's a feature"*
          *******************************************/

    	//racuna se koja bi bila udaljenost kad bi igrac prisao drugom
    	//igraci se zapravo krecu za po 0.023 medjutim ako se prdrzi dugme 
    	//moguce je da igrac "prokliza" tako da premasi mnogo vrednost od 0.023
    	//zato proveravam za pomeraj od 0.1
        double distance = sqrt((p1.body.Xcenter-0.1-p2.body.Xcenter) * (p1.body.Xcenter-0.1-p2.body.Xcenter)
            +(p1.body.Zcenter-0.1-p2.body.Zcenter) * (p1.body.Zcenter-0.1-p2.body.Zcenter));

        //stvarna udaljenost bez pomeraja
        double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

        //pretpostavimo da je igrac preblizu pa ne moze da se krece
		bool can_move = false;

		//ako smo dovoljno daleko mozemo da se krecemo sigurno
		if(distance > 0.4)
			can_move = true;

		//ako se klikom nasa udaljenost povecava, mozemo da se krecemo
		if(distance > real_distance)
			can_move = true;

        // Pokrece se animacija ako je globalna promenljiva move_up = 0
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_up and p1.body.Xcenter >= -2.0 and p1.body.Zcenter >= -2.30
        	and !p1.right_hand.guard and can_move) {
            //zapravo nije udarac vec se simulira da se noga pomera pri kretanju
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);

            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);

            //za odgovarajuci pokret na gore aktivira se funkcija move_up()
            p1.move_up();
            //globalno promenljivoj kazemo da se podesi na 1 kako bi funkcija
            //za pokret znala da treba da pocne sa radom
            move_up = 1;
            //kazemo svim delovima tela da treba da se pomeraju
            p1.head.movement = true;
            p1.body.movement = true;
            p1.left_foot.movement = true;
            p1.right_foot.movement = true;
            p1.left_hand.movement = true;
            p1.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
        }
    }
        break;    
        
    case 's':
    case 'S':{

        double distance = sqrt((p1.body.Xcenter+0.1-p2.body.Xcenter) * (p1.body.Xcenter+0.1-p2.body.Xcenter)
            +(p1.body.Zcenter+0.1-p2.body.Zcenter) * (p1.body.Zcenter+0.1-p2.body.Zcenter));
        
       double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

		bool can_move = false;

		if(distance > 0.4)
			can_move = true;

		if(distance > real_distance)
			can_move = true;

        //ukoliko je vrednost move_down globalne promenljive 0 pokrece se animacija
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_down and p1.body.Xcenter <= 3.50 and p1.body.Zcenter <=3.7
        	and !p1.right_hand.guard and can_move) {
            //nije udarac samo je isti pokret kada se krece
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);

            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);

            //poziva se metoda move_down koja nam podesava da se translacija vrsi
            //za neke hardkodirane vrednosti
            p1.move_down();
            //pokrecemo animaciju u timer funkciji promenom vrednosti globalne
            //promenljive na 1
            move_down = 1;
            //kazemo svim delovima tela da se pokrecu
            p1.head.movement = true;
            p1.body.movement = true;
            p1.left_foot.movement = true;
            p1.right_foot.movement = true;
            p1.left_hand.movement = true;
            p1.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs, TIMER_ID);
        }
    }
        break;

    case 'd':
    case 'D':{


        double distance = sqrt((p1.body.Xcenter+0.1-p2.body.Xcenter) * (p1.body.Xcenter+0.1-p2.body.Xcenter)
            +(p1.body.Zcenter-0.1-p2.body.Zcenter) * (p1.body.Zcenter-0.1-p2.body.Zcenter));
       
        double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

		bool can_move = false;

		if(distance > 0.4)
			can_move = true;

		if(distance > real_distance)
			can_move = true;

        //vazi sve isto kao za komande W i S samo sto se vrsi translacija za
        //odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_right and  p1.body.Xcenter <= 3.50 and p1.body.Zcenter >= -2.30
        	and !p1.right_hand.guard and can_move) {
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);

            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);


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
    }
        break;  

    case 'a':
    case 'A':{


        double distance = sqrt((p1.body.Xcenter-0.1-p2.body.Xcenter) * (p1.body.Xcenter-0.1-p2.body.Xcenter)
            +(p1.body.Zcenter+0.1-p2.body.Zcenter) * (p1.body.Zcenter+0.1-p2.body.Zcenter));
       
        double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

		bool can_move = false;

		if(distance > 0.4)
			can_move = true;

		if(distance > real_distance)
			can_move = true;

        //vazi isto kao i za W i S samo za odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena 
        if (!move_left and p1.body.Zcenter <= 3.7 and p1.body.Xcenter >= -2.0
        	and !p1.right_hand.guard and can_move) {
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);

            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);


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
    }
        break;


    case '4':
        /* Pokrece se animacija. */
        if (!left_hand_punch2 and !p2.right_hand.guard) {
            //kazemo da se vrednost za udarac leve ruke stavi na true
            //u svakom iscrtavanju se proverava da li je ta vrednost true
            //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
            p2.left_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            left_hand_punch2 = 1;

            //isto kao i za igraca 1 samo drugaciji pravci vektora
            if(p1.body.Zcenter < p2.body.Zcenter)
            	p2.set_axes(p1.body.Zcenter - p2.left_hand.Zcenter,
                      	 -p1.body.Xcenter + p2.left_hand.Xcenter);
            else
            	p2.set_axes(-p2.body.Zcenter +p1.left_hand.Zcenter,
                      	 p2.body.Xcenter -p1.left_hand.Xcenter);

            glutTimerFunc(TIMER_INTERVAL, on_timer_hand2, TIMER_ID);
        }
        break;


    case '6':
        //kazemo da se vrednost za udarac desne ruke stavi na true
        //u svakom iscrtavanju se proverava da li je ta vrednost true
        //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
        if (!right_hand_punch2 and !p2.right_hand.guard) {
            p2.right_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            right_hand_punch2 = 1;

            if(p1.body.Zcenter < p2.body.Zcenter)
        	    p2.set_axes(p1.body.Zcenter - p2.right_hand.Zcenter,
            	           -p1.body.Xcenter + p2.right_hand.Xcenter);
        	else
        		p2.set_axes(-p2.body.Zcenter +p1.right_hand.Zcenter,
            	           p2.body.Xcenter - p1.right_hand.Xcenter);

            glutTimerFunc(TIMER_INTERVAL, on_timer_hand2, TIMER_ID);
        }
        break;

    case '8':{
            //ako hocemo da nam igrac drzi gard na pritisak tastera
            //fjom postavljenom u igracu kazemo levoj i desnoj ruci da se saviju
            //pise "punch" jer je pokret isti kao za udarac ruke uz dodatak savijanja
            p2.guard_me(true);
            if (!left_hand_punch2 and !right_hand_punch2 and !guard2) {
            //kazemo da se vrednost za udarac leve ruke stavi na true
            //u svakom iscrtavanju se proverava da li je ta vrednost true
            //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
            p2.left_hand.hit = true;
            p2.right_hand.hit = true;

            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);
            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            left_hand_punch2 = 1;
            right_hand_punch2 = 1;
            guard2 = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_guard2, TIMER_ID);
        }
            
    }
    break;

    case 'y':
    case 'Y':{
            //ako hocemo da nam igrac drzi gard na pritisak tastera
            //fjom postavljenom u igracu kazemo levoj i desnoj ruci da se saviju
            //pise "punch" jer je pokret isti kao za udarac ruke uz dodatak savijanja
            p1.guard_me(true);
            if (!left_hand_punch and !right_hand_punch and !guard) {
            //kazemo da se vrednost za udarac leve ruke stavi na true
            //u svakom iscrtavanju se proverava da li je ta vrednost true
            //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
            p1.left_hand.hit = true;
            p1.right_hand.hit = true;

            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);
            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            left_hand_punch = 1;
            right_hand_punch = 1;
            guard = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_guard, TIMER_ID);
        }
    }
    break;
   
    }
}

static void glutSpecialInput(int key, int x, int y){

    switch (key) {
   
    case GLUT_KEY_UP:{

    	double distance = sqrt((p1.body.Xcenter-p2.body.Xcenter+0.1) * (p1.body.Xcenter-p2.body.Xcenter+0.1)
            +(p1.body.Zcenter-p2.body.Zcenter+0.1) * (p1.body.Zcenter-p2.body.Zcenter+0.1));
 
    	double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

		bool can_move = false;

		if(distance > 0.4)
			can_move = true;

		if(distance > real_distance)
			can_move = true;

        // Pokrece se animacija ako je globalna promenljiva move_up = 0
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_up2 and p2.body.Xcenter >= -2.0 and p2.body.Zcenter >= -2.30
        	and !p2.right_hand.guard and can_move) {
            //zapravo nije udarac vec se simulira da se noga pomera pri kretanju
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);

            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);
            //za odgovarajuci pokret na gore aktivira se funkcija move_up()
            p2.move_up();
            //globalno promenljivoj kazemo da se podesi na 1 kako bi funkcija
            //za pokret znala da treba da pocne sa radom
            move_up2 = 1;
            //kazemo svim delovima tela da treba da se pomeraju
            p2.head.movement = true;
            p2.body.movement = true;
            p2.left_foot.movement = true;
            p2.right_foot.movement = true;
            p2.left_hand.movement = true;
            p2.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement2, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs2, TIMER_ID);
        }
    }
        break;    
        
    
    case GLUT_KEY_DOWN:{


    	double distance = sqrt((p1.body.Xcenter-p2.body.Xcenter-0.1) * (p1.body.Xcenter-p2.body.Xcenter-0.1)
            +(p1.body.Zcenter-p2.body.Zcenter-0.1) * (p1.body.Zcenter-p2.body.Zcenter-0.1));

    	double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

		bool can_move = false;

		if(distance > 0.4)
			can_move = true;

		if(distance > real_distance)
			can_move = true;

        //ukoliko je vrednost move_down globalne promenljive 0 pokrece se animacija
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_down2 and p2.body.Xcenter <= 3.50 and p2.body.Zcenter <=3.7
        	and !p2.right_hand.guard and can_move) {
            //nije udarac samo je isti pokret kada se krece
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);

            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);
            //poziva se metoda move_down koja nam podesava da se translacija vrsi
            //za neke hardkodirane vrednosti
            p2.move_down();
            //pokrecemo animaciju u timer funkciji promenom vrednosti globalne
            //promenljive na 1
            move_down2 = 1;
            //kazemo svim delovima tela da se pokrecu
            p2.head.movement = true;
            p2.body.movement = true;
            p2.left_foot.movement = true;
            p2.right_foot.movement = true;
            p2.left_hand.movement = true;
            p2.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement2, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs2, TIMER_ID);
        }
    }
        break;

    
    case GLUT_KEY_RIGHT:{


    	double distance = sqrt((p1.body.Xcenter-p2.body.Xcenter-0.1) * (p1.body.Xcenter-p2.body.Xcenter-0.1)
            +(p1.body.Zcenter-p2.body.Zcenter+0.1) * (p1.body.Zcenter-p2.body.Zcenter+0.1));
 
    	double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

		bool can_move = false;

		if(distance > 0.4)
			can_move = true;

		if(distance > real_distance)
			can_move = true;

        //vazi sve isto kao za komande W i S samo sto se vrsi translacija za
        //odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_right2 and  p2.body.Xcenter <= 3.50 and p2.body.Zcenter >= -2.30
        	and !p2.right_hand.guard and can_move) {
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);

            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);

            p2.move_right();
            
            move_right2 = 1;
            p2.head.movement = true;
            p2.body.movement = true;
            p2.left_foot.movement = true;
            p2.right_foot.movement = true;
            p2.left_hand.movement = true;
            p2.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement2, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs2, TIMER_ID);
        }
    }
        break;  

    
    case GLUT_KEY_LEFT:{

    	//racuna se koja bi bila udaljenost kad bi igrac prisao drugom
    	//igraci se zapravo krecu za po 0.023 medjutim ako se prdrzi dugme 
    	//moguce je da igrac "prokliza" tako da premasi mnogo vrednost od 0.023
    	//zato proveravam za pomeraj od 0.1
    	double distance = sqrt((p1.body.Xcenter-p2.body.Xcenter+0.1) * (p1.body.Xcenter-p2.body.Xcenter+0.1)
            +(p1.body.Zcenter-p2.body.Zcenter-0.1) * (p1.body.Zcenter-p2.body.Zcenter-0.1));
 
 		//kolika je zapravo udaljenost bez pomeranja na novu poziciju
    	double real_distance =sqrt((p1.body.Xcenter-p2.body.Xcenter) * (p1.body.Xcenter-p2.body.Xcenter)
            +(p1.body.Zcenter-p2.body.Zcenter) * (p1.body.Zcenter-p2.body.Zcenter));

    	//pretpostavimo da igrac ne moze da se krece ka drugom jer je preblziu
		bool can_move = false;

		//ako je nova udaljenost veca od 0.5 svakako smo dovoljno daleko pa se
		//mozemo kretati
		if(distance > 0.4)
			can_move = true;

		//ako se nasim porketom udaljavamo od prethodne pozicije, mozemo da
		//se krecemo
		if(distance > real_distance)
			can_move = true;

        //vazi isto kao i za W i S samo za odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena 
        if (!move_left2 and p2.body.Zcenter <= 3.7 and p2.body.Xcenter >= -2.0
        	and !p2.right_hand.guard and can_move) {
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;


            //postavljanje vektora oko kojeg se vrsi rotacija
            //on je vektor normalan na pravac koji zaklapaju igraci
            p2.set_axes(p1.body.Zcenter - p2.body.Zcenter,
                       -p1.body.Xcenter + p2.body.Xcenter);

            p1.set_axes(p2.body.Zcenter - p1.body.Zcenter,
                       -p2.body.Xcenter + p1.body.Xcenter);

            p2.move_left();
            
            move_left2 = 1;
            p2.head.movement = true;
            p2.body.movement = true;
            p2.left_foot.movement = true;
            p2.right_foot.movement = true;
            p2.left_hand.movement = true;
            p2.right_hand.movement = true;
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement2, TIMER_ID);
            glutTimerFunc(TIMER_INTERVAL, on_timer_movement_legs2, TIMER_ID);
        }
    }
        break; 

    }

}

//celokupna funkcija radi na "on_release" dogadjaje
//slicno kao on_keyboard uz to sto se dogadjaj "okida" kada se dugme "pusti" 
static void on_Upkeyboard(unsigned char key, int x, int y){

    switch(key){

        case 'y':
        case 'Y':{
                //za prestanak garda i vracanje ruke u pocetni polozaj
                //nije neophodno raditi nista osim ove naredbe jer se u odgovarajucoj
                //timer funkciji nalazi samo provera da li igrac drzi gard ili ne
                p1.guard_me(false);
        }
        break;

        case '8':{
                //isto kao za igraca1 na komandu "Y"
                p2.guard_me(false);
            }
        break;

  }

}

static void finish_him(int wich_player){

	if(wich_player == 1){
		//printf("Pobedio je igrac 2");
		

		// lansiramo protivnika zavrsnim udarcem
		p1.sky_launch();
        
        //pokrece se bilo koji tajmer(recimo za kretanje na gore) da bi animacija izgledala "glatko"
        move_up = 1;
        
        //protivnik se krece pod dejstvom udarca
        p1.head.movement = true;
        p1.body.movement = true;
        p1.left_foot.movement = true;
        p1.right_foot.movement = true;
        p1.left_hand.movement = true;
        p1.right_hand.movement = true;

        //pomocna inicijalizacija promenljive koja regulise koliko puta se ponovi translacija
        //veca promenljiva + manji korak za koji se nesto translira = sporija animacija
        p2.right_hand.translate_for = 10;
        p2.left_hand.translate_for = 10;
        p2.right_foot.translate_for = 10;
        p2.left_foot.translate_for = 10;
        p2.body.translate_for = 10;
        p2.head.translate_for = 10;
        
        //pokretanje tajmera za gubitnika
        glutTimerFunc(TIMER_INTERVAL, on_timer_movement2, TIMER_ID);

        who_should_dance = 2;
        animation_ongoing += 1;
        if(animation_ongoing == 1)
        	glutTimerFunc(TIMER_INTERVAL, on_timer_end, TIMER_ID);

	}
	else{
		//printf("Pobedio je igrac 1");
		
		// lansiramo protivnika zavrsnim udarcem
		p2.sky_launch();
        
        //pokrece se bilo koji tajmer(recimo za kretanje na gore) da bi animacija izgledala "glatko"
        move_up = 1;
        
        //protivnik se krece pod dejstvom udarca
        p2.head.movement = true;
        p2.body.movement = true;
        p2.left_foot.movement = true;
        p2.right_foot.movement = true;
        p2.left_hand.movement = true;
        p2.right_hand.movement = true;

        //pomocna inicijalizacija promenljive koja regulise koliko puta se ponovi translacija
        //veca promenljiva + manji korak za koji se nesto translira = sporija animacija
        p1.right_hand.translate_for = 10;
        p1.left_hand.translate_for = 10;
        p1.right_foot.translate_for = 10;
        p1.left_foot.translate_for = 10;
        p1.body.translate_for = 10;
        p1.head.translate_for = 10;
        
        //pokretanje tajmera za gubitnika
        glutTimerFunc(TIMER_INTERVAL, on_timer_movement, TIMER_ID);

        who_should_dance = 1;
        animation_ongoing += 1;
        if(animation_ongoing == 1)
        	glutTimerFunc(TIMER_INTERVAL, on_timer_end, TIMER_ID);

	}
	
	

}

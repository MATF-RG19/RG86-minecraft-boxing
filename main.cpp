#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "player.hpp"
#include "image.h"


#define FILENAME0 "textura.bmp"

/* Identifikatori tekstura. */
static GLuint names[1];

//dimenzije prozora
static int window_width, window_height;

//pokazivac na klasu player u vidu globalne promenljive p1
//globalna kako bi se mogla koristiti u svim funkcijama
player p1;
player p2;

//centar ringa
double ring_centerX = 1.25;
double ring_centerZ = 1.25;

//prvi igrac
static int left_hand_punch;
static int right_hand_punch; 
static int left_foot_move;
static int right_foot_move;
static int move_up;
static int move_down;
static int move_left;
static int move_right; 

//drugi igrac
static int left_hand_punch2;
static int right_hand_punch2; 
static int left_foot_move2;
static int right_foot_move2;
static int move_up2;
static int move_down2;
static int move_left2;
static int move_right2; 


//podesavanja tajmera
#define TIMER_ID 0
#define TIMER_INTERVAL 20

static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void glutSpecialInput(int key, int x, int y);

static void initialize(void);


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
    glutSpecialFunc(glutSpecialInput);

    
    
    glClearColor(0.75, 0.75, 0.75, 0);
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

    glutMainLoop();

    return 0;
}


static void initialize(void)
{
    // Image * image;

    // glClearColor(0, 0, 0, 0);

    // glEnable(GL_DEPTH_TEST);

    // glEnable(GL_TEXTURE_2D);

    // glTexEnvf(GL_TEXTURE_ENV,
    //           GL_TEXTURE_ENV_MODE,
    //           GL_REPLACE);

    
    // image = image_init(0, 0);

    // std::string s = "tekstura.bmp";
    // image_read(image, s.c_str());

    // glGenTextures(1, names);

    // glBindTexture(GL_TEXTURE_3D, names[0]);
    // glTexParameteri(GL_TEXTURE_3D,
    //                 GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_3D,
    //                 GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_3D,
    //                 GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_3D,
    //                 GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_3D, 0, GL_RGB,
    //              image->width - 0.5, image->height - 0.5, 0,
    //              GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    // image_done(image);

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

    //siva boja za tribine
    GLfloat ambient_coeffsS[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat diffuse_coeffsS[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat specular_coeffsS[] = { 1, 1, 1, 1 };
    

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

    //pod
    glPushMatrix();


        glLineWidth(1);        
        
        glColor3f(1,1,1);
        glBegin(GL_POLYGON);
            glVertex3f(-100, 0, 100);
            glVertex3f(100, 0, 100);
            glVertex3f(100, 0, -100);
            glVertex3f(-100, 0, -100);
        glEnd();

    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffsS);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffsS);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffsS);
    glMaterialf(GL_FRONT,  GL_SHININESS, shininess);

    //tribine
    glPushMatrix();
        glLineWidth(1);        
        
        // leva-gore tribina
        //glColor3f(1,1,1);
        // glBindTexture(GL_TEXTURE_3D, 0);
        glBegin(GL_POLYGON);
            // glNormal3f(1,1,0);

            // glTexCoord3f(0, 0, 1);
            glVertex3f(-5, 0, 50);

            // glTexCoord3f(0, 0, -1);
            glVertex3f(-5, 0, -50);

            // glTexCoord3f(-1, 1, -1);
            glVertex3f(-120, 4, -50);

            // glTexCoord3f(-1, 1, 1);
            glVertex3f(-120, 4, 50);
        glEnd();

    glPopMatrix();
        //desna-gore tribina
        glColor3f(1,1,1);
        glBegin(GL_POLYGON);
            glVertex3f(-100, 0, -5);
            glVertex3f(100, 0, -5);
            glVertex3f(100, 10, -8);
            glVertex3f(-100, 10, -8);
        glEnd();

        //leva-dole tribina
        glColor3f(1,1,1);
        glBegin(GL_POLYGON);
            glVertex3f(-100, 0, 5);
            glVertex3f(100, 0, 5);
            glVertex3f(100, 10, 8);
            glVertex3f(-100, 10, 8);
        glEnd();

        glColor3f(1,1,1);
        glBegin(GL_POLYGON);
            glVertex3f(5, 0, 100);
            glVertex3f(5, 0, -100);
            glVertex3f(8, 10, -100);
            glVertex3f(8, 10, 100);
        glEnd();



    //iscrtavanje igraca
    p1.draw_player();
    p2.draw_player();


    glutSwapBuffers();
}



static void on_timer_hand(int value)
{
    
    if (value != TIMER_ID)
        return;


    //udarac leve ruke
    //ruci kazemo da se rotira za odredjeni deo. Kako je pritiskom na dugme
    //promenljiva hit stavljena na true, prilikom iscrtavanja se proverava njena
    //vrednost. Kako je ona true, vrsi se rotacija za rotate_for deo
    //da bi ruka stala sa rotacijom u nekom trenutku imamo rotate_end promenljivu
    if(p1.left_hand.rotate_for >= -120 && !p1.left_hand.rotate_end){
        p1.left_hand.rotate_for -= 10;
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


    //udarac leve ruke
    //ruci kazemo da se rotira za odredjeni deo. Kako je pritiskom na dugme
    //promenljiva hit stavljena na true, prilikom iscrtavanja se proverava njena
    //vrednost. Kako je ona true, vrsi se rotacija za rotate_for deo
    //da bi ruka stala sa rotacijom u nekom trenutku imamo rotate_end promenljivu
    if(p2.left_hand.rotate_for >= -120 && !p2.left_hand.rotate_end){
        p2.left_hand.rotate_for -= 10;
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

    //neophodno mi je da odredim ugao za koji rotiram igraca kako bi ostali
    //uvek okrenuti jedan ka drugome
    //pocetno slovo(x|y) oznacava vektor pocetka(x) ili kraja(y) kretanja
    //drugo slovo predstavlja koordinatu
    // long double xx = p2.body.Xcenter - p1.body.Xcenter;
    // long double yx = p2.body.Xcenter - p1.body.previous_Xcenter;
    // long double xz = p2.body.Zcenter - p1.body.Zcenter;
    // long double yz = p2.body.Zcenter - p1.body.previous_Zcenter;


    //ugao racunam prema formuli za skalarni proizvod vektora
    // long double angle = (xx*yx + xz*yz)/(sqrt(xx*xx + xz*xz) * sqrt(yx*yx + yz*yz));
    // angle = acos(angle);

    //acos vraca ugao u radianima, proporciom da vracam u stepene
    // angle = (180.0 * angle)/(3.14159265359);

    //odredjivanje orijentacije kako bih znao u kom smeru treba da rotiram igraca
    // long double y1 = p2.body.Zcenter;
    // long double y2 = p1.body.Zcenter;
    // long double y3 = p1.body.previous_Zcenter;
    // long double x1 = p2.body.Xcenter;
    // long double x2 = p1.body.Xcenter;
    // long double x3 = p1.body.previous_Xcenter;
    // long double retVal = (y2 - y1)*(x3 - x2) -(y3 - y2)*(x2 -x1);

    //provera orijentacije
    //negativno => suprotno od smera kazaljke
    // if(retVal > 0)
    //     angle = -angle;
    // else if(retVal == 0)
    //     angle = 0;

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
        // p1.redirect(angle, p1.body.Xcenter, p1.body.Ycenter, p1.body.Zcenter);
        //p2.redirect(angle, p2.body.Xcenter, p2.body.Ycenter, p2.body.Zcenter);

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
            //kazemo da se vrednost za udarac leve ruke stavi na true
            //u svakom iscrtavanju se proverava da li je ta vrednost true
            //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
            p1.left_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            left_hand_punch = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
        }
        break;

    case 'h':
    case 'H':
        //kazemo da se vrednost za udarac desne ruke stavi na true
        //u svakom iscrtavanju se proverava da li je ta vrednost true
        //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
        if (!right_hand_punch) {
            p1.right_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            right_hand_punch = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_hand, TIMER_ID);
        }
        break;


    case 'w':
    case 'W':

        // Pokrece se animacija ako je globalna promenljiva move_up = 0
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_up and p1.body.Xcenter >= -2.0 and p1.body.Zcenter >= -2.30) {
            //zapravo nije udarac vec se simulira da se noga pomera pri kretanju
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

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
        break;    
        
    case 's':
    case 'S':

        //ukoliko je vrednost move_down globalne promenljive 0 pokrece se animacija
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_down and p1.body.Xcenter <= 3.50 and p1.body.Zcenter <=3.9) {
            //nije udarac samo je isti pokret kada se krece
            p1.left_foot.hit = true;
            left_foot_move = 1;

            p1.right_foot.hit = true;
            right_foot_move = 1;

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
        break;

    case 'd':
    case 'D':

        //vazi sve isto kao za komande W i S samo sto se vrsi translacija za
        //odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_right and  p1.body.Xcenter <= 3.50 and p1.body.Zcenter >= -2.30) {
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
        //vazi isto kao i za W i S samo za odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena 
        if (!move_left and p1.body.Zcenter <= 3.7 and p1.body.Xcenter >= -2.0) {
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


    case '4':
        /* Pokrece se animacija. */
        if (!left_hand_punch2) {
            //kazemo da se vrednost za udarac leve ruke stavi na true
            //u svakom iscrtavanju se proverava da li je ta vrednost true
            //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
            p2.left_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            left_hand_punch2 = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_hand2, TIMER_ID);
        }
        break;


    case '6':
        //kazemo da se vrednost za udarac desne ruke stavi na true
        //u svakom iscrtavanju se proverava da li je ta vrednost true
        //ako jeste vrse se odgovarajuce rotacije kako bi se simulirao glatki pokret
        if (!right_hand_punch2) {
            p2.right_hand.hit = true;
            //vrednost globalne promenljive stavljamo na 1 kako bi tajmer znao da
            //treba da pocne sa animacijom
            right_hand_punch2 = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_hand2, TIMER_ID);
        }
        break;


    case 'o':
    case 'O':
        //fullScreen mode
        glutFullScreen();
    break;
    }
}

static void glutSpecialInput(int key, int x, int y){

    switch (key) {
   
    case GLUT_KEY_UP:

        // Pokrece se animacija ako je globalna promenljiva move_up = 0
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_up2 and p2.body.Xcenter >= -2.0 and p2.body.Zcenter >= -2.30) {
            //zapravo nije udarac vec se simulira da se noga pomera pri kretanju
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

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
        break;    
        
    
    case GLUT_KEY_DOWN:

        //ukoliko je vrednost move_down globalne promenljive 0 pokrece se animacija
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_down2 and p2.body.Xcenter <= 3.50 and p2.body.Zcenter <=3.9) {
            //nije udarac samo je isti pokret kada se krece
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

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
        break;

    
    case GLUT_KEY_RIGHT:

        //vazi sve isto kao za komande W i S samo sto se vrsi translacija za
        //odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena
        if (!move_right2 and  p2.body.Xcenter <= 3.50 and p2.body.Zcenter >= -2.30) {
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

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
        break;  

    
    case GLUT_KEY_LEFT:
        //vazi isto kao i za W i S samo za odgovarajuce vrednosti
        //dodatno ako se igrac nalazi blizu ivice ringa, nije moguce kretati se dalje
        //kako igrac ne bi napustio granice terena 
        if (!move_left2 and p2.body.Zcenter <= 3.7 and p2.body.Xcenter >= -2.0) {
            p2.left_foot.hit = true;
            left_foot_move2 = 1;

            p2.right_foot.hit = true;
            right_foot_move2 = 1;

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
        break; 

    }

}
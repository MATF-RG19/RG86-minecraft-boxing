#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include "player.hpp"

//dimenzije prozora
static int window_width, window_height;
player p1;

static double player1_X = p1.body.Xcenter;
static double player1_Y = p1.body.Ycenter;
static double player1_Z = p1.body.Zcenter;

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

    
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);

    //postavljanje svetlosti koje je globalno za sve objekte
    //kasnije ce svaki objekat dobiti koeficijente kako bi odbijao odgovarajucu boju
    GLfloat light_position[] = { 50, 50, -50, 0 };
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

    
    //plava svetlost se najvise odbija od rina
    GLfloat ambient_coeffs[] = { 0.3, 0.6, 0.9, 1 };
    GLfloat diffuse_coeffs[] = { 0.2, 0.2, 1, 1 };
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


    p1.draw_player();
    //azuriranje koordinata tela igraca1 pri svakom pokretu
    player1_X = p1.body.Xcenter;
    player1_Y = p1.body.Ycenter;
    player1_Z = p1.body.Zcenter;
    printf("%lf %lf %lf\n", player1_X, player1_Y, player1_Z);

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


    //posto se svi delovi tela pomeraju zajedno, uzimam bilo koji deo za proveru
    //ovde sam uzeo desnu ruku i proveravam koliko je jos rotacije preostalo da se
    //izvrsi tako sto rotacija ide od 5 pa se smanjuje do 0 cime se postize efekat
    //glatkog pokreta. Dakle ako je pomeranje aktivirano za neku od starna 
    if(p1.right_hand.translate_for > 0 && (move_up || move_down || move_right || move_left)
        and player1_X){
        //translate_for se sustinski kao promenljiva ne primenjuje ni u jednoj funkciji
        //njena uloga je da "broji" koliko je ostalo do kraja translacije
        p1.right_hand.translate_for -= 0.5;
        p1.left_hand.translate_for -= 0.5;
        p1.right_foot.translate_for -= 0.5;
        p1.left_foot.translate_for -= 0.5;
        p1.body.translate_for -= 0.5;
        p1.head.translate_for -= 0.5;
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
        if (!move_up) {
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
        if (!move_down) {
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
        //vazi isto kao i za W i S samo za odgovarajuce vrednosti 
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
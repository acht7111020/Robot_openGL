#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3
#define MENU_SPEED_UP 4
#define MENU_SPEED_DOWN 5
#define MENU_STOP 6
#define MENU_ROTATE 7
#define MENU_PAUSE 8
#define MENU_START 9
#define MENU_ANI_WALK 10
#define MENU_ANI_DANCE 11

using std::cout;
using std::endl;

const GLubyte robox_body[] = {74, 194, 164};
const GLubyte robox_connect[] = {255, 153, 51};
const GLubyte robox_eyes[] = {0, 0, 0};
const GLubyte robox_leg[] = {26, 181, 170};

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLubyte timer_cnt = 0;
GLuint timer_flag = 90, auto_rotate = 1, flag = 0;
GLint x_place_tmp = 0, x_place = 0;
GLfloat position_x = 400, position_y = 400;
bool timer_enabled = true;
unsigned int timer_speed = 16;
GLUquadricObj *quadricObj = gluNewQuadric();

// menu
GLfloat MENU_speed = 1;
GLbyte MENU_stop = 0, MENU_rotate = 1, MENU_pause = 0;

struct animate_paras
{
    GLfloat move = 0;
    GLint flag = 0;
} head, hand1, hand2, hand1_l, hand2_l, leg1, leg2, leg1_l, leg2_l,
H_hand1, H_hand2, H_hand1_l, H_hand2_l,
fingerH1, fingerH2, fingerH3, fingerB1, fingerB2, fingerB3,
fingerLH1, fingerLH2, fingerLH3, fingerLB1, fingerLB2, fingerLB3;
struct papameters {
    GLfloat head_biggest;
    GLfloat head_smallest;
    GLfloat hand1_biggest[2]; // right, left
    GLfloat hand1_smallest[2];
    GLfloat hand2_biggest[2];
    GLfloat hand2_smallest[2];
    GLfloat leg1_biggest[2];
    GLfloat leg1_smallest[2];
    GLfloat leg2_biggest[2];
    GLfloat leg2_smallest[2];
} paras[3];

struct papameters_ani {
    GLfloat hand1_biggest[2]; // right, left
    GLfloat hand1_smallest[2];
    GLfloat hand2_biggest[2];
    GLfloat hand2_smallest[2];
} paras_ani[3];

GLuint paras_num = 0;
// Print OpenGL context related information.
void dumpInfo(void)
{
    printf("Vendor: %s\n", glGetString (GL_VENDOR));
    printf("Renderer: %s\n", glGetString (GL_RENDERER));
    printf("Version: %s\n", glGetString (GL_VERSION));
    printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}
animate_paras trans_animate_paras(animate_paras parts, GLfloat biggest, GLfloat smallest, GLfloat rate){
    
    if(parts.move < smallest || parts.flag == 1){
        parts.move += rate;
        parts.flag = 1;
        if(parts.move > biggest) parts.flag = 0;
    }
    else parts.move -= rate;
    
    return parts;
}
void trans_animate(void){
    if (paras_num != 1) {
        if (MENU_rotate && !MENU_pause)auto_rotate ++;
    }
    if (!MENU_stop && !MENU_pause) {
        head = trans_animate_paras(head, paras[paras_num].head_biggest, paras[paras_num].head_smallest, 0.001*MENU_speed);
        /*hand1 = trans_animate_paras(hand1, 10, -10, 0.3*MENU_speed);
        hand2 = trans_animate_paras(hand2, 30, -10, 0.6*MENU_speed);
        hand1_l = trans_animate_paras(hand1_l, 10, -10, 0.3*MENU_speed);
        hand2_l = trans_animate_paras(hand2_l, 10, -30, 0.6*MENU_speed);
        
        leg1 = trans_animate_paras(leg1, 0, -40, 0.6*MENU_speed);
        leg2 = trans_animate_paras(leg2, 40, 0, 0.6*MENU_speed);
        leg1_l = trans_animate_paras(leg1_l, 0, -40, 0.6*MENU_speed);
        leg2_l = trans_animate_paras(leg2_l, 40, 0, 0.6*MENU_speed);*/
        
        hand1 = trans_animate_paras(hand1, paras[paras_num].hand1_biggest[0], paras[paras_num].hand1_smallest[0], 0.3*MENU_speed);
        hand2 = trans_animate_paras(hand2, paras[paras_num].hand2_biggest[0], paras[paras_num].hand2_smallest[0], 0.6*MENU_speed);
        hand1_l = trans_animate_paras(hand1_l, paras[paras_num].hand1_biggest[1], paras[paras_num].hand1_smallest[1], 0.3*MENU_speed);
        hand2_l = trans_animate_paras(hand2_l, paras[paras_num].hand2_biggest[1], paras[paras_num].hand2_smallest[1], 0.6*MENU_speed);
        
        leg1 = trans_animate_paras(leg1, paras[paras_num].leg1_biggest[0], paras[paras_num].leg1_smallest[0], 0.6*MENU_speed);
        leg2 = trans_animate_paras(leg2, paras[paras_num].leg2_biggest[0], paras[paras_num].leg2_smallest[0], 0.6*MENU_speed);
        leg1_l = trans_animate_paras(leg1_l, paras[paras_num].leg1_biggest[1], paras[paras_num].leg1_smallest[1], 0.6*MENU_speed);
        leg2_l = trans_animate_paras(leg2_l, paras[paras_num].leg2_biggest[1], paras[paras_num].leg2_smallest[1], 0.6*MENU_speed);
        
        fingerH1 = trans_animate_paras(fingerH1, 200, 180, 0.1*MENU_speed);
        fingerH2 = trans_animate_paras(fingerH2, 200, 180, 0.1*MENU_speed);
        fingerH3 = trans_animate_paras(fingerH3, 180, 160, 0.1*MENU_speed);
        fingerB1 = trans_animate_paras(fingerB1, 0, -30, 0.15*MENU_speed);
        fingerB2 = trans_animate_paras(fingerB2, 0, -30, 0.15*MENU_speed);
        fingerB3 = trans_animate_paras(fingerB3, 30, 0, 0.15*MENU_speed);
        fingerLH1 = trans_animate_paras(fingerLH1, 200, 180, 0.1*MENU_speed);
        fingerLH2 = trans_animate_paras(fingerLH2, 180, 160, 0.1*MENU_speed);
        fingerLH3 = trans_animate_paras(fingerLH3, 180, 160, 0.1*MENU_speed);
        fingerLB1 = trans_animate_paras(fingerLB1, 0, -30, 0.15*MENU_speed);
        fingerLB2 = trans_animate_paras(fingerLB2, 30, 0, 0.15*MENU_speed);
        fingerLB3 = trans_animate_paras(fingerLB3, 30, 0, 0.15*MENU_speed);

        
        if (paras_num == 1) {
            position_x -= 0.0001 ; position_y -= 0.0001 ;
            H_hand1 = trans_animate_paras(H_hand1, paras_ani[paras_num].hand1_biggest[0], paras_ani[paras_num].hand1_smallest[0], 0.5*MENU_speed);
            H_hand2 = trans_animate_paras(H_hand2, paras_ani[paras_num].hand2_biggest[0], paras_ani[paras_num].hand2_smallest[0], 0.8*MENU_speed);
            H_hand1_l =
            trans_animate_paras(H_hand1_l, paras_ani[paras_num].hand1_biggest[1], paras_ani[paras_num].hand1_smallest[1], 0.5*MENU_speed);
            H_hand2_l =
            trans_animate_paras(H_hand2_l, paras_ani[paras_num].hand2_biggest[1], paras_ani[paras_num].hand2_smallest[1], 0.8*MENU_speed);
        }
    }
    //printf("%lf\n", head.move);

}
void init_para(){
    // init parameters
    leg1.move = -20; leg1.flag = 0;
    leg2.move = 20; leg2.flag = 1;
    leg1_l.move = -20; leg1_l.flag = 1;
    leg2_l.move = 20; leg2_l.flag = 0;
    
    hand1.move = 0; hand1.flag = 0;
    hand2.move = 0; hand2.flag = 0;
    hand1_l.move = 0; hand1_l.flag = 0;
    hand2_l.move = 0; hand2_l.flag = 0;
    head.move = 0; head.flag = 0;
    
    fingerH1.move = 200;
    fingerB1.move = 0;
    fingerH2.move = 160;
    fingerB2.move = 0;
    fingerH3.move = 160;
    fingerB3.move = 0;
    
    fingerLH1.move = 200;
    fingerLB1.move = 0;
    fingerLH2.move = 160;
    fingerLB2.move = 0;
    fingerLH3.move = 160;
    fingerLB3.move = 0;
    
    if(paras_num == 1){
        H_hand1.move = 300;
        H_hand2.move = 80;
        H_hand1_l.move = 240;
        H_hand2_l.move = -80;
    }
    else {
        H_hand1.move = 60;
        H_hand2.move = 20;
        H_hand1_l.move = 240;
        H_hand2_l.move = 20;
    }
    
    paras[0] = {0, -0.15, {10,10}, {-10,-10}, {30, 10}, {-10,-30}, {0,0}, {-40,-40}, {40,40}, {0,0}};
    paras[1] = {0, -0.15, {10,10}, {-10,-10}, {30, 10}, {-10,-30}, {0,0}, {-40,-40}, {40,40}, {0,0}};
    paras_ani[0] ={{60,240}, {60,240}, {20, 20}, {20, 20}};
    paras_ani[1] ={{420,230}, {320,130}, {80, 80}, {-80, -80}};
    //paras[0] = {0, -0.15, {10,10}, {-10,-10}, {30, 10}, {-10,-30}, {0,0}, {-40,-40}, {40,40}, {0,0}};

}
void draw_finger(GLfloat angle1, GLfloat angle2, GLfloat y, GLfloat z){
    
    glPushMatrix();
    glColor3ub(160, 160, 160);
    glTranslatef(0, y, z);
    //glutSolidSphere(0.03, 50, 50);
    
    glPushMatrix();
    glRotatef(angle1, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.035f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f);
    //glutSolidSphere(0.02, 50, 50);
    glutSolidCube(0.04);
    glPopMatrix();
    
    glRotatef(angle1, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.067f, 0, 0);
    glRotatef(angle2, 0, 0, 1);
    glTranslatef(-0.04f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f);
    //glutSolidSphere(0.02, 50, 50);
    glutSolidCube(0.04);
    
    /*glScalef(1/2.0f, 1.0f, 1.0f);
    glTranslatef(-0.05f, 0.0f, 0.0f);
    glutSolidSphere(0.02, 50, 50);*/
    
    glPopMatrix();
}

// Print My display
void print(void){
    
    
    glPushMatrix();
    glTranslatef(400-position_x, 400 - position_y, 0);
    glRotatef(auto_rotate,0.0f, 1.0f, 0.0f);
    // BODY PART
    glPushMatrix();
    glColor3ubv(robox_body);
    glScalef(0.85f, 1.0f, 0.5f);
    glutSolidCube(0.7);
    glPopMatrix();
    
    // HEAD CONNECT PART
    glPushMatrix();
    glTranslatef(0, head.move, 0);
    glColor3ubv(robox_connect);
    glTranslatef(0.0f, 0.35f, 0.0f);
    glScalef(1.0f, 0.5f, 1.0f);
    glutSolidCube(0.10);
    glTranslatef(0.0f, 0.1f, 0.0f);
    glutSolidCube(0.11);
    glTranslatef(0.0f, 0.11f, 0.0f);
    glutSolidCube(0.12);
    glTranslatef(0.0f, 0.12f, 0.0f);
    glutSolidCube(0.13);
    glPopMatrix();
    
    // HEAD PART
    glPushMatrix();
        glTranslatef(0, head.move, 0);
        glColor3ubv(robox_body);
        glTranslatef(0.0f, 0.68f, 0.0f);
        glScalef(2.2f, 1.0f, 1.1f);
        glutSolidCube(0.3);
    
        // EYES
        glPushMatrix();
            glColor3ubv(robox_eyes);
            glScalef(1/2.2f, 1.0f, 1/1.1f);
            glTranslatef(0.15f, 0.005f, 0.16f);
            glutSolidSphere(0.055, 50, 50);
            glTranslatef(-0.3f, 0.0f, 0.0f);
            glutSolidSphere(0.055, 50, 50);
        glPopMatrix();
    
        // HEAD DISCROATION
    {
        glPushMatrix();
            glColor3ubv(robox_leg);
            glTranslatef(0, 0.15, 0);
            glScalef(2, 5, 4);
            glutSolidCube(0.01);
            glScalef(1/2.0f, 1/5.0f, 1/4.0f);

            glColor3ub(255, 255, 255);
            glTranslatef(0, 0.1, 0);
            glPushMatrix();
                glScalef(0.5, 35, 1);
                glutSolidCube(0.005);
            glPopMatrix();
            glTranslatef(0, 0.1, 0);
            glScalef(1/2.2f, 1, 1/1.1f);
            glutSolidSphere(0.015, 50, 50);
        glPopMatrix();
        
    }
        glPushMatrix();
            glColor3ub(255, 255, 255);
            glTranslatef(0.15f, 0.0f, 0.0f);
            glScaled(0.02f, 0.05f, 0.05f);
            glutSolidIcosahedron();
        glPopMatrix();
    
        glPushMatrix();
            glColor3ub(255, 255, 255);
            glTranslatef(-0.15f, 0.0f, 0.0f);
            glScaled(0.02f, 0.05f, 0.05f);
            glutSolidIcosahedron();
        glPopMatrix();
    glPopMatrix();
    
    // RIGHT HAND
    glPushMatrix();
    glColor3ubv(robox_connect);
    glTranslatef(0.33f, 0.27f, 0.0f);
    //glutSolidSphere(0.085, 30, 30);
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glutSolidTorus(0.05, 0.035, 30, 30);
    glTranslatef(0, 0, 0.03);
    glutSolidTorus(0.05, 0.015, 30, 30);
    glPopMatrix();
    
    //glRotatef(60, 0, 0, -1);
    glRotatef(H_hand1.move, 0, 0, -1);
    glRotatef(hand1.move, 0, -1, 0);
    glColor3ubv(robox_body);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glScalef(2.2f, 0.7f, 1.0f);
    glutSolidCube(0.11);
    
    glScalef(1/2.2f, 1/0.7f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glutSolidSphere(0.08, 30, 30);
    /*glPushMatrix();
    glScaled(0.8f, 1.0f, 1.0f);
    glRotated(90, 0, 1, 0);
    glutSolidTorus(0.08, 0.035, 30, 30);
    glPopMatrix();
    */
    //glRotatef(20, 0, 0, -1);
    glRotatef(H_hand2.move, 0, 0, -1);
    glRotatef(hand2.move, 0, -1, 0);
    glColor3ubv(robox_body);
    glTranslatef(0.18f, 0.0f, 0.0f);
    glScalef(1.6f, 1.0f, 1.0f);
    glutSolidCube(0.15);
    
    glScalef(1/2.0f, 1.0f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.22f, 0.0f, 0.0f);
    //glutSolidSphere(0.085, 30, 30);
    glPushMatrix();
    glScaled(1, 1.2f, 1.2f);
    glutSolidCube(0.14);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.075, 0.0f, 0.0f);
    glScalef(0.7f, 1.0f, 1.0f);
    /*draw_finger(180, -20, 0.04, 0.035);
    draw_finger(180, -35, 0.04, -0.035);
    draw_finger(160, 30, -0.05, 0.0);*/
    draw_finger(fingerH1.move, fingerB1.move, 0.04, 0.035);
    draw_finger(fingerH2.move, fingerB2.move, 0.04, -0.035);
    draw_finger(fingerH3.move, fingerB3.move, -0.05, 0.0);
    glPopMatrix();
    glPopMatrix();
    
    // LIEF HAND
    glPushMatrix();
    glColor3ubv(robox_connect);
    glTranslatef(-0.32f, 0.27f, 0.0f);
    //glutSolidSphere(0.085, 30, 30);
    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glutSolidTorus(0.05, 0.035, 30, 30);
    glTranslatef(0, 0, -0.03);
    glutSolidTorus(0.05, 0.015, 30, 30);
    glPopMatrix();
    
    //glRotatef(240, 0, 0, 1);
    glRotatef(H_hand1_l.move, 0, 0, 1);
    glRotatef(hand1_l.move, 0, 1, 0);
    glColor3ubv(robox_body);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glScalef(2.2f, 0.7f, 1.0f);
    glutSolidCube(0.11);
    
    glScalef(1/2.2f, 1/0.7f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glutSolidSphere(0.08, 30, 30);
    
    //glRotatef(20, 0, 0, 1);
    glRotatef(H_hand2_l.move, 0, 0, 1);
    glRotatef(hand2_l.move, 0, 1, 0);
    glColor3ubv(robox_body);
    glTranslatef(0.18f, 0.0f, 0.0f);
    glScalef(1.6f, 1.0f, 1.0f);
    glutSolidCube(0.15);
    
    glScalef(1/2.0f, 1.0f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    //glutSolidSphere(0.085, 30, 30);
    glPushMatrix();
    glScaled(1, 1.2f, 1.2f);
    glutSolidCube(0.14);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.075, 0.0f, 0.0f);
    glScalef(0.7f, 1.0f, 1.0f);
    draw_finger(fingerLH1.move, fingerLB1.move, 0.04, 0);
    draw_finger(fingerLH2.move, fingerLB2.move, -0.05, 0.035);
    draw_finger(fingerLH3.move, fingerLB3.move, -0.05, -0.035);
    glPopMatrix();
    glPopMatrix();
    
    // RIGHT LEG
    glPushMatrix();
        glColor3ubv(robox_connect);
        glTranslatef(0, -0.4f, 0.0f);
    
        glPushMatrix();
            glTranslatef(0.15, 0, 0);
            //glutSolidSphere(0.085, 30, 30);
            glPushMatrix();
                glScalef(0.07f, 0.05f, 0.09f);
                glutSolidDodecahedron();
            glPopMatrix() ;
            // LEG 1
            glColor3ubv(robox_leg);
            glRotatef(leg1.move, 1, 0, 0);
            glTranslatef(0, -0.18, 0);
            glScalef(1.0, 1.8, 1.0);
            glutSolidCube(0.13);
            // LEG 2
            glScalef(1, 0.5, 1);
            glTranslatef(0, -0.11, 0);
            glRotatef(leg2.move, 1, 0, 0);
            glTranslatef(0, -0.15, 0);
            glScalef(1.0, 3.0, 1.0);
            glutSolidCube(0.11);
            
            glPushMatrix();
                glColor3ubv(robox_connect);
                glTranslatef(0, -0.055, 0);
            
                glRotatef(90, 0, 0, 1);
                glScalef(0.16f, 1.3f, 1.0f);
                glutSolidCube(0.2);
                //glutSolidOctahedron();
            glPopMatrix();
        
        glPopMatrix();
    
   
        // LEFT LEG
        glPushMatrix();
            glTranslatef(-0.15, 0, 0);
            //glutSolidSphere(0.085, 30, 30);
    
            glPushMatrix();
                glScalef(0.07f, 0.05f, 0.09f);
                glutSolidDodecahedron();
            glPopMatrix() ;
    
            // LEG 1
            glColor3ubv(robox_leg);
            glRotatef(leg1_l.move, 1, 0, 0);
            glTranslatef(0, -0.18, 0);
            glScalef(1.0, 1.8, 1.0);
            glutSolidCube(0.13);
            // LEG 2
            glScalef(1, 0.5, 1);
            glTranslatef(0, -0.11, 0);
            glRotatef(leg2_l.move, 1, 0, 0);
            glTranslatef(0, -0.15, 0);
            glScalef(1.0, 3.0, 1.0);
            glutSolidCube(0.11);
            
            glPushMatrix();
                glColor3ubv(robox_connect);
                glTranslatef(0, -0.055, 0);
                glRotatef(90, 0, 0, 1);
                glScalef(0.16f, 1.3f, 1.0f);
                glutSolidCube(0.2);
            glPopMatrix();
        
        glPopMatrix();
    
    
    
    glPopMatrix();
    
    
    glPopMatrix();
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    /*glBegin(GL_TRIANGLES);
     {
     glColor3ub(timer_cnt, 0, 255 - timer_cnt);
     glVertex3fv(tri_v1);
     glColor3ub(255, timer_cnt, 255 - timer_cnt);
     glVertex3fv(tri_v2);
     glColor3ub(255 - timer_cnt, 0, timer_cnt);
     glVertex3fv(tri_v3);
     
     
     }
     glEnd();*/
    
    
    print();
    trans_animate();
    
    //glPopMatrix();
    glutSwapBuffers();
}

void My_Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    
    float viewportAspect = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.4 * viewportAspect, 1.4 * viewportAspect, -1.4, 1.4);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void My_Timer(int val)
{
    timer_cnt++;
    glutPostRedisplay();
    if(timer_enabled)
    {
        glutTimerFunc(timer_speed, My_Timer, val);
    }
}

void My_Mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
        x_place = x;
    }
    else if(state == GLUT_UP)
    {
        printf("Mouse %d is released at (%d, %d)\n", button, x, y);
        x_place_tmp = x_place;
        x_place = x;
        if ( (x_place - x_place_tmp) > 100  ) {
            auto_rotate += 100;
        }else if( (x_place - x_place_tmp) < -100  ) {
            auto_rotate -= 100;
        }
    }
}

void My_Keyboard(unsigned char key, int x, int y)
{
    printf("Key %c is pressed at (%d, %d)\n", key, x, y);
    switch (key) {
        case 'a':
            position_x += 0.01;
            break;
        case 'd':
            position_x -= 0.01;
            break;
        default:
            break;
    }
    cout << position_x << endl;
}

void My_SpecialKeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            printf("F1 is pressed at (%d, %d)\n", x, y);
            break;
        case GLUT_KEY_PAGE_UP:
            printf("Page up is pressed at (%d, %d)\n", x, y);
            break;
        case GLUT_KEY_LEFT:
            printf("Left arrow is pressed at (%d, %d)\n", x, y);
            auto_rotate --;
            break;
        case GLUT_KEY_RIGHT:
            printf("Right arrow is pressed at (%d, %d)\n", x, y);
            auto_rotate ++;
            break;
        case GLUT_KEY_UP:
            printf("UPUPUP arrow is pressed at (%d, %d)\n", x, y);
            break;
        default:
            printf("Other special key is pressed at (%d, %d)\n", x, y);
            break;
    }
}

void My_Menu(int id)
{
    switch(id)
    {
        case MENU_TIMER_START:
            if(!timer_enabled)
            {
                timer_enabled = true;
                glutTimerFunc(timer_speed, My_Timer, 0);
            }
            break;
        case MENU_TIMER_STOP:
            timer_enabled = false;
            break;
        case MENU_EXIT:
            exit(0);
            break;
        case MENU_SPEED_UP:
            MENU_stop = 0;
            MENU_speed = 2;
            init_para();
            break;
        case MENU_SPEED_DOWN:
            MENU_stop = 0;
            MENU_speed = 1;
            init_para();
            break;
        case MENU_STOP:
            MENU_stop = 1;
            init_para();
            break;
        case MENU_ROTATE:
            MENU_rotate = (MENU_rotate)? 0 : 1;
            break;
        case MENU_PAUSE:
            MENU_pause = 1;
            break;
        case MENU_START:
            MENU_pause = 0;
            MENU_stop = 0;
            break;
        case MENU_ANI_WALK:
            paras_num = 0;
            init_para();
            break;
        case MENU_ANI_DANCE:
            paras_num = 1;
            init_para();
            break;
        default:
            break;
    }
}
void initial_statement(){
    
    
    // Create a menu and bind it to mouse right button.
    ////////////////////////////
    int menu_main = glutCreateMenu(My_Menu);
    int menu_ani = glutCreateMenu(My_Menu);
    int menu_speed = glutCreateMenu(My_Menu);
    
    glutSetMenu(menu_main);
    //glutAddSubMenu("Timer", menu_color);
    glutAddSubMenu("Speed", menu_speed);
    glutAddSubMenu("Animation", menu_ani);
    glutAddMenuEntry("Auto rotate enable/disable", MENU_ROTATE);
    glutAddMenuEntry("Start", MENU_START);
    glutAddMenuEntry("Pause", MENU_PAUSE);
    glutAddMenuEntry("Stop", MENU_STOP);
    glutAddMenuEntry("Exit", MENU_EXIT);
    
    /*glutSetMenu(menu_color);
    glutAddMenuEntry("Start", MENU_TIMER_START);
    glutAddMenuEntry("Stop", MENU_TIMER_STOP);*/
    
    glutSetMenu(menu_speed);
    glutAddMenuEntry("Speed up", MENU_SPEED_UP);
    glutAddMenuEntry("Speed down", MENU_SPEED_DOWN);
    
    glutSetMenu(menu_ani);
    glutAddMenuEntry("Walking", MENU_ANI_WALK);
    glutAddMenuEntry("Dancing", MENU_ANI_DANCE);
    
    glutSetMenu(menu_main);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    ////////////////////////////
    
    // Register GLUT callback functions.
    ///////////////////////////////
    glutDisplayFunc(My_Display);
    glutReshapeFunc(My_Reshape);
    glutMouseFunc(My_Mouse);
    glutKeyboardFunc(My_Keyboard);
    glutSpecialFunc(My_SpecialKeys);
    glutTimerFunc(timer_speed, My_Timer, 0);
    ///////////////////////////////
    
    // LIGHT
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    
    
    
}
int main(int argc, char *argv[])
{
    // Initialize GLUT and GLEW, then create a window.
    ////////////////////
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Hw 1 robox design"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
    dumpInfo();
    ////////////////////
    
    // Initialize OpenGL states.
    ////////////////////////
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    ////////////////////////
    
    initial_statement();
    init_para();
    // Enter main event loop.
    //////////////
    glutMainLoop();
    //////////////
    return 0;
}
/*
glBegin(GL_QUADS);
{
    glColor3ubv(robox_connect);
    glVertex3f( 0.2f, 0.15f, 0.175f);
    glVertex3f(-0.2f, 0.15f, 0.175f);
    glVertex3f(-0.3f, -0.15f, 0.175f);
    glVertex3f( 0.3f, -0.15f, 0.175f);
    
    
     // Bottom face (y = -1.0f)
     glVertex3f( 1.0f, -1.0f,  1.0f);
     glVertex3f(-1.0f, -1.0f,  1.0f);
     glVertex3f(-1.0f, -1.0f, -1.0f);
     glVertex3f( 1.0f, -1.0f, -1.0f);
     
     // Front face  (z = 1.0f)
     glVertex3f( 1.0f,  1.0f, 1.0f);
     glVertex3f(-1.0f,  1.0f, 1.0f);
     glVertex3f(-1.0f, -1.0f, 1.0f);
     glVertex3f( 1.0f, -1.0f, 1.0f);
     
     // Back face (z = -1.0f)
     glVertex3f( 1.0f, -1.0f, -1.0f);
     glVertex3f(-1.0f, -1.0f, -1.0f);
     glVertex3f(-1.0f,  1.0f, -1.0f);
     glVertex3f( 1.0f,  1.0f, -1.0f);
     
     // Left face (x = -1.0f)
     glVertex3f(-1.0f,  1.0f,  1.0f);
     glVertex3f(-1.0f,  1.0f, -1.0f);
     glVertex3f(-1.0f, -1.0f, -1.0f);
     glVertex3f(-1.0f, -1.0f,  1.0f);
     
     // Right face (x = 1.0f)
     glVertex3f(1.0f,  1.0f, -1.0f);
     glVertex3f(1.0f,  1.0f,  1.0f);
     glVertex3f(1.0f, -1.0f,  1.0f);
     glVertex3f(1.0f, -1.0f, -1.0f);
     
}
glEnd();*/
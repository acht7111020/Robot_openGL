#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "texture_loader.h"

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


GLuint textureID, tele;

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
H_hand1, H_hand2, H_hand1_l, H_hand2_l, leg_y1, leg_y2, leg_ly1, leg_ly2,
fingerH1, fingerH2, fingerH3, fingerB1, fingerB2, fingerB3,
fingerLH1, fingerLH2, fingerLH3, fingerLB1, fingerLB2, fingerLB3,
position_hx, position_hy;
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
        //if (MENU_rotate && !MENU_pause)auto_rotate ++;
    }
    if (!MENU_stop && !MENU_pause) {
        head = trans_animate_paras(head, paras[paras_num].head_biggest, paras[paras_num].head_smallest, 0.001*MENU_speed);
        
        hand1 = trans_animate_paras(hand1, paras[paras_num].hand1_biggest[0], paras[paras_num].hand1_smallest[0], 0.3*MENU_speed);
        hand2 = trans_animate_paras(hand2, paras[paras_num].hand2_biggest[0], paras[paras_num].hand2_smallest[0], 0.6*MENU_speed);
        hand1_l = trans_animate_paras(hand1_l, paras[paras_num].hand1_biggest[1], paras[paras_num].hand1_smallest[1], 0.3*MENU_speed);
        hand2_l = trans_animate_paras(hand2_l, paras[paras_num].hand2_biggest[1], paras[paras_num].hand2_smallest[1], 0.6*MENU_speed);
        
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
            //position_x -= 0.0001 ; position_y -= 0.0001 ;
            position_hx = trans_animate_paras(position_hx, 0.8, -0.8, 0.0012*MENU_speed);
            position_hy = trans_animate_paras(position_hy, 0.1, -0.1, 0.0036*MENU_speed);
            H_hand1 = trans_animate_paras(H_hand1, paras_ani[paras_num].hand1_biggest[0], paras_ani[paras_num].hand1_smallest[0], 0.5*MENU_speed);
            H_hand2 = trans_animate_paras(H_hand2, paras_ani[paras_num].hand2_biggest[0], paras_ani[paras_num].hand2_smallest[0], 0.8*MENU_speed);
            H_hand1_l =
            trans_animate_paras(H_hand1_l, paras_ani[paras_num].hand1_biggest[1], paras_ani[paras_num].hand1_smallest[1], 0.5*MENU_speed);
            H_hand2_l =
            trans_animate_paras(H_hand2_l, paras_ani[paras_num].hand2_biggest[1], paras_ani[paras_num].hand2_smallest[1], 0.8*MENU_speed);
            
            leg_y1 = trans_animate_paras(leg_y1, 80, -10, 0.57*MENU_speed);
            leg2 = trans_animate_paras(leg2, 40, 20, 0.38*MENU_speed);
            leg_ly1 = trans_animate_paras(leg_ly1, 10, -80, 0.57*MENU_speed);
            leg2_l = trans_animate_paras(leg2_l, 40, 20, 0.38*MENU_speed);
            
        }
        else {
            
            leg1 = trans_animate_paras(leg1, paras[paras_num].leg1_biggest[0], paras[paras_num].leg1_smallest[0], 0.6*MENU_speed);
            leg2 = trans_animate_paras(leg2, paras[paras_num].leg2_biggest[0], paras[paras_num].leg2_smallest[0], 0.6*MENU_speed);
            leg1_l = trans_animate_paras(leg1_l, paras[paras_num].leg1_biggest[1], paras[paras_num].leg1_smallest[1], 0.6*MENU_speed);
            leg2_l = trans_animate_paras(leg2_l, paras[paras_num].leg2_biggest[1], paras[paras_num].leg2_smallest[1], 0.6*MENU_speed);
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
    fingerH2.move = 200;
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
        leg_y1.move = 35; leg_y1.flag = 0;
        leg_ly1.move = -35; leg_ly1.flag = 1;
        leg2.move = 30; leg2.flag = 0;
        leg2_l.move = 30; leg2_l.flag = 0;
    }
    else {
        H_hand1.move = 60;
        H_hand2.move = 20;
        H_hand1_l.move = 240;
        H_hand2_l.move = 20;
        leg_y1.move = 0; leg_y1.flag = 0;
        leg_ly1.move = 0; leg_ly1.flag = 0;
        position_hy.move = 0;position_hx.move = 0;
    }
    
    paras[0] = {0, -0.15, {10,10}, {-10,-10}, {30, 10}, {-10,-30}, {0,0}, {-40,-40}, {40,40}, {0,0}};
    paras[1] = {0, -0.15, {10,10}, {-10,-10}, {30, 10}, {-10,-30}, {0,0}, {-40,-40}, {40,40}, {0,0}};
    paras_ani[0] ={{60,240}, {60,240}, {20, 20}, {20, 20}};
    paras_ani[1] ={{420,230}, {320,130}, {80, 80}, {-80, -80}};
    //paras[0] = {0, -0.15, {10,10}, {-10,-10}, {30, 10}, {-10,-30}, {0,0}, {-40,-40}, {40,40}, {0,0}};

}

void head_ear(GLfloat x){
    glPushMatrix();
        glColor3ub(255, 255, 255);
        glTranslatef(x, 0.0f, 0.0f);
        glScaled(0.02f, 0.05f, 0.05f);
        glutSolidIcosahedron();
    glPopMatrix();

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
    
    glPopMatrix();
}
void hand(GLfloat x, GLint flag, animate_paras Hhand1, animate_paras Bhand1, animate_paras Hhand2, animate_paras Bhand2,
          GLfloat x2, char finger){
    
    // RIGHT HAND
    glPushMatrix();
        glColor3ubv(robox_connect);
    
        glTranslatef(x, 0.27f, 0.0f);
        glPushMatrix();
            glRotated(90, 0, 1, 0);
            glutSolidTorus(0.05, 0.035, 30, 30);
    
            glTranslatef(0, 0, 0.03*flag);
            glutSolidTorus(0.05, 0.015, 30, 30);
        glPopMatrix();
 
    
        glRotatef(Hhand1.move, 0, 0, -1*flag);
        
        glRotatef(Bhand1.move, 0, -1*flag, 0);
        glColor3ubv(robox_body);
        glTranslatef(0.19f, 0.0f, 0.0f);
        glScalef(2.2f, 0.7f, 1.0f);
        glutSolidCube(0.11);
        
        glScalef(1/2.2f, 1/0.7f, 1.0f);
        glColor3ubv(robox_connect);
        glTranslatef(0.19f, 0.0f, 0.0f);
        glutSolidSphere(0.08, 30, 30);
        
        glRotatef(Hhand2.move, 0, 0, -1*flag);
        
        glRotatef(Bhand2.move, 0, -1*flag, 0);
        glColor3ubv(robox_body);
        glTranslatef(0.18f, 0.0f, 0.0f);
        glScalef(1.6f, 1.0f, 1.0f);
        glutSolidCube(0.15);
        
        glScalef(1/2.0f, 1.0f, 1.0f);
        glColor3ubv(robox_connect);
        
        glTranslatef(x2, 0.0f, 0.0f);
        glPushMatrix();
            glScaled(1, 1.2f, 1.2f);
            glutSolidCube(0.14);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.075, 0.0f, 0.0f);
            glScalef(0.7f, 1.0f, 1.0f);
            if (finger == 'r') {
                draw_finger(fingerH1.move, fingerB1.move, 0.04, 0.035);
                draw_finger(fingerH2.move, fingerB2.move, 0.04, -0.035);
                draw_finger(fingerH3.move, fingerB3.move, -0.05, 0.0);
            }
            else{
                draw_finger(fingerLH1.move, fingerLB1.move, 0.04, 0);
                draw_finger(fingerLH2.move, fingerLB2.move, -0.05, 0.035);
                draw_finger(fingerLH3.move, fingerLB3.move, -0.05, -0.035);
            }
        
        glPopMatrix();
    glPopMatrix();
}
void draw_leg(GLfloat x, animate_paras Hleg1, animate_paras Bleg1, animate_paras Yleg1, GLuint texture){
    
    glPushMatrix();
    
    
        glTranslatef(x, 0, 0);
        glColor3ub(255, 255, 255);
        
        glPushMatrix();
            glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        
                glScalef(0.07f, 0.05f, 0.08f);
                glutSolidDodecahedron();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_R);
        glPopMatrix() ;
        
        // LEG 1
        glColor3ubv(robox_leg);
        glRotated(Yleg1.move, 0, 1, 0);
        glRotatef(Hleg1.move, 1, 0, 0);
        glTranslatef(0, -0.18, 0);
        glScalef(1.0, 1.8, 1.0);
        glutSolidCube(0.13);
        // LEG 2
        glScalef(1, 0.5, 1);
        glTranslatef(0, -0.11, 0);
        glRotatef(Bleg1.move, 1, 0, 0);
        
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
}
// Print My display
void print(void){
    
    
    glPushMatrix();
    glTranslatef(400-position_x-position_hx.move, 400 -position_y - position_hy.move, 0);
    glRotatef(auto_rotate,0.0f, 1.0f, 0.0f);
    // BODY PART
    glPushMatrix();
    glColor3ubv(robox_body);
    glScalef(0.85f, 1.0f, 0.5f);
    glutSolidCube(0.7);
    
    glColor3ub(255, 255, 255);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tele);
    glBegin(GL_QUADS);
    {
        glNormal3f(0, 0, 1);
        //glColor3f(1, 1, 1);
        glTexCoord2f(0.05, 1.01); glVertex3f(-0.18f, 0.15f, 0.36f);
        glTexCoord2f(0.05, 0.01); glVertex3f(-0.18f, -0.25f, 0.36f);
        glTexCoord2f(1.05, 0.01); glVertex3f(0.22f, -0.25f, 0.36f);
        glTexCoord2f(1.05, 1.01); glVertex3f(0.22f, 0.15f, 0.36f);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
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
        head_ear(0.15);
        head_ear(-0.15);
    glPopMatrix();
    
    // RIGHT HAND
    hand(0.33, 1, H_hand1, hand1, H_hand2, hand2, 0.22, 'r');
    // LIEF HAND
    hand(-0.32, -1, H_hand1_l, hand1_l, H_hand2_l, hand2_l, 0.19, 'l');
    
    glPushMatrix();
        glColor3ub(255, 255, 255);
        glTranslatef(0, -0.4f, 0.0f);
        // RIGHT LEG
        draw_leg(0.15, leg1, leg2, leg_y1, textureID);
        // LEFT LEG
        draw_leg(-0.15, leg1_l, leg2_l, leg_ly1, textureID);
    glPopMatrix();
    
    glPopMatrix();
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    print();
    trans_animate();

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
        case 's':
            position_y += 0.01;
            break;
        case 'w':
            position_y -= 0.01;
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
GLuint loadBMP_custom(const char * imagepath)
{
    
    // Texture
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
    
    FILE * file = fopen(imagepath, "rb");
    if (!file){ printf("Image could not be opened\n"); }
    if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
    }
    if (header[0] != 'B' || header[1] != 'M'){
        printf("Not a correct BMP file\n");
    }
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    if (imageSize == 0)    imageSize = width*height * 3;
    if (dataPos == 0)      dataPos = 54;
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);
    
    GLuint texture;
    glGenTextures(1, &texture);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture);
    // Give the image to OpenGL
    glEnable(GL_TEXTURE_2D);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    return texture;
}

GLuint loading_png(const char * imagepath)
{
    texture_data tdata = load_png(imagepath); // return width * height * 4 uchars
    GLuint texture;
    if(tdata.data == 0)
    {
        printf("Load png failed\n\n\n");
        //load failed
        return texture;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data); // Use GL_RGBA
    glGenerateMipmap(GL_TEXTURE_2D);
    glAlphaFunc (GL_GREATER, 0) ; //解決alpha與z-buffer的問題
    glEnable (GL_ALPHA_TEST) ;
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    free_texture_data(tdata);
    
    return texture;
    
}
void setTexture()
{
    textureID = loadBMP_custom("/Users/Mac/Desktop/drawing code/hw1_robox/hw1_robox/texture_metal3.bmp");
    tele = loading_png("/Users/Mac/Desktop/drawing code/hw1_robox/hw1_robox/television.png");

    GLfloat border_color[] = { 1.0, 0, 0, 1.0 };
    GLfloat env_color[] = { 0, 1.00, 0, 1.00 };
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,border_color);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,env_color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
}
void initial_statement(){
    
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
    
    glutSetMenu(menu_speed);
    glutAddMenuEntry("Speed up", MENU_SPEED_UP);
    glutAddMenuEntry("Speed down", MENU_SPEED_DOWN);
    
    glutSetMenu(menu_ani);
    glutAddMenuEntry("Walking", MENU_ANI_WALK);
    glutAddMenuEntry("Dancing", MENU_ANI_DANCE);
    
    glutSetMenu(menu_main);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(My_Display);
    glutReshapeFunc(My_Reshape);
    glutMouseFunc(My_Mouse);
    glutKeyboardFunc(My_Keyboard);
    glutSpecialFunc(My_SpecialKeys);
    glutTimerFunc(timer_speed, My_Timer, 0);
  
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Hw 1 robot design");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    setTexture();
    initial_statement();
    init_para();
    
    glutMainLoop();
    return 0;
}
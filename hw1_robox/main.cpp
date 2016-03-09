#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cstdio>
#include <cstdlib>

#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3

const GLubyte robox_body[] = {74, 194, 164};
const GLubyte robox_connect[] = {255, 153, 51};
const GLubyte robox_eyes[] = {0, 0, 0};
const GLfloat tri_v2[3] = { 0.5f, -0.4f, 0.0f};
const GLfloat tri_v3[3] = { 0.0f,  0.6f, 0.0f};

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLubyte timer_cnt = 0;
GLuint timer_flag = 90, auto_rotate = 0;
GLuint flag = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;

// Print OpenGL context related information.
void dumpInfo(void)
{
    printf("Vendor: %s\n", glGetString (GL_VENDOR));
    printf("Renderer: %s\n", glGetString (GL_RENDERER));
    printf("Version: %s\n", glGetString (GL_VERSION));
    printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
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
    glutSolidSphere(0.02, 50, 50);
    glPopMatrix();
    
    glRotatef(angle1, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.067f, 0, 0);
    glRotatef(angle2, 0, 0, 1);
    glTranslatef(-0.04f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f);
    glutSolidSphere(0.02, 50, 50);
    
    glScalef(1/2.0f, 1.0f, 1.0f);
    glTranslatef(-0.05f, 0.0f, 0.0f);
    glutSolidSphere(0.02, 50, 50);
    
    glPopMatrix();
}

// Print My display
void print(void){
    
    glRotatef(auto_rotate,0.0f, 1.0f, 0.0f);
    glPushMatrix();
    
    // BODY PART
    glPushMatrix();
    glColor3ubv(robox_body);
    glScalef(0.85f, 1.0f, 0.5f);
    glutSolidCube(0.7);
    glPopMatrix();
    
    // HEAD CONNECT PART
    glPushMatrix();
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
    glColor3ubv(robox_body);
    glTranslatef(0.0f, 0.68f, 0.0f);
    glScalef(2.2f, 1.0f, 1.1f);
    glutSolidCube(0.3);
    
    // EYES
    glColor3ubv(robox_eyes);
    glScalef(1/2.2f, 1.0f, 1/1.1f);
    glTranslatef(0.15f, 0.005f, 0.16f);
    glutSolidSphere(0.055, 50, 50);
    glTranslatef(-0.3f, 0.0f, 0.0f);
    glutSolidSphere(0.055, 50, 50);
    glPopMatrix();
    
    // RIGHT HAND
    glPushMatrix();
    glColor3ubv(robox_connect);
    glTranslatef(0.33f, 0.27f, 0.0f);
    glutSolidSphere(0.085, 30, 30);
    
    glRotatef(60, 0, 0, -1);
    glColor3ubv(robox_body);
    glTranslatef(0.2f, 0.0f, 0.0f);
    glScalef(2.4f, 1.0f, 1.0f);
    glutSolidCube(0.11);
    
    glScalef(1/2.4f, 1.0f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glutSolidSphere(0.085, 30, 30);
    
    glRotatef(20, 0, 0, -1);
    glColor3ubv(robox_body);
    glTranslatef(0.2f, 0.0f, 0.0f);
    glScalef(2.4f, 1.0f, 1.0f);
    glutSolidCube(0.11);
    
    glScalef(1/2.4f, 1.0f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glutSolidSphere(0.085, 30, 30);
    
    glPushMatrix();
    glTranslatef(0.065, 0.0f, 0.0f);
    glScalef(0.7f, 1.0f, 1.0f);
    draw_finger(180, -20, 0.04, 0.035);
    draw_finger(180, -35, 0.04, -0.035);
    draw_finger(160, 30, -0.05, 0.0);
    glPopMatrix();
    glPopMatrix();
    
    // LIEF HAND
    glPushMatrix();
    glColor3ubv(robox_connect);
    glTranslatef(-0.33f, 0.27f, 0.0f);
    glutSolidSphere(0.085, 30, 30);
    
    glRotatef(240, 0, 0, 1);
    glColor3ubv(robox_body);
    glTranslatef(0.2f, 0.0f, 0.0f);
    glScalef(2.4f, 1.0f, 1.0f);
    glutSolidCube(0.11);
    
    glScalef(1/2.4f, 1.0f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glutSolidSphere(0.085, 30, 30);
    
    glRotatef(20, 0, 0, 1);
    glColor3ubv(robox_body);
    glTranslatef(0.2f, 0.0f, 0.0f);
    glScalef(2.4f, 1.0f, 1.0f);
    glutSolidCube(0.11);
    
    glScalef(1/2.4f, 1.0f, 1.0f);
    glColor3ubv(robox_connect);
    glTranslatef(0.19f, 0.0f, 0.0f);
    glutSolidSphere(0.085, 30, 30);
    
    glPushMatrix();
    glTranslatef(0.065, 0.0f, 0.0f);
    glScalef(0.7f, 1.0f, 1.0f);
    draw_finger(180, -10, 0.04, 0);
    draw_finger(180, 30, -0.05, 0.035);
    draw_finger(160, 30, -0.05, -0.035);
    glPopMatrix();
    glPopMatrix();
    
    /*glColor3ubv(robox_connect);
    glutSolidSphere(0.1, 30, 30);
    
    glColor3ubv(robox_body);
     
    glTranslatef(0.25f, 0.0f, 0.0f);
    glScalef(2.5f, 1.0f, 1.0f);
    //glutSolidSphere(0.1, 30, 30);
    glutSolidCube(0.13);
    glPopMatrix();
    
    glPushMatrix();
    if (timer_cnt == 90 || flag == 1) {
        timer_flag--;
        flag = 1;
        if (timer_flag <= 0) {
            timer_flag = 90;
            timer_cnt = 0;
            flag = 0;
            glRotatef(timer_cnt, 0.0f, 0.0f, -1.0f);
        }
        else glRotatef(timer_flag, 0.0f, 0.0f, -1.0f);
    }
    else glRotatef(timer_cnt, 0.0f, 0.0f, -1.0f);
    
    //arm
    glColor3ubv(robox_body);
    glTranslatef(-0.25f, 0.0f, 0.0f);
    glScalef(2.5f, 1.0f, 1.0f);
    //glutSolidSphere(0.1, 30, 30);
    glutSolidCube(0.13);
    
    //sphere
    glColor3ubv(robox_connect);
    glScalef(1/2.5f, 1.0f, 1.0f);
    glTranslatef(-0.25f, 0.0f, 0.0f);
    glutSolidSphere(0.11, 50, 50);
    
    
    //finger
    draw_finger(-20, 50, 0.06, 0.055);
    draw_finger(-20, 60, 0.06, -0.055);
    draw_finger(20, -60, -0.07, 0.0);
    */
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
    
    auto_rotate ++;
    //glPopMatrix();
    glutSwapBuffers();
}

void My_Reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    
    float viewportAspect = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1 * viewportAspect, 1 * viewportAspect, -1, 1);
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
    }
    else if(state == GLUT_UP)
    {
        printf("Mouse %d is released at (%d, %d)\n", button, x, y);
    }
}

void My_Keyboard(unsigned char key, int x, int y)
{
    printf("Key %c is pressed at (%d, %d)\n", key, x, y);
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
        default:
            break;
    }
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
    
    // Create a menu and bind it to mouse right button.
    ////////////////////////////
    int menu_main = glutCreateMenu(My_Menu);
    int menu_color = glutCreateMenu(My_Menu);
    
    glutSetMenu(menu_main);
    glutAddSubMenu("Timer", menu_color);
    glutAddMenuEntry("Exit", MENU_EXIT);
    
    glutSetMenu(menu_color);
    glutAddMenuEntry("Start", MENU_TIMER_START);
    glutAddMenuEntry("Stop", MENU_TIMER_STOP);
    
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
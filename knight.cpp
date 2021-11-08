#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <math.h>
#include "shape.cpp"
using namespace std;

GLfloat g_base;
GLfloat g_onbase;
GLfloat g_back;
GLfloat g_neck;
GLfloat g_head;
GLfloat g_hair;

void ReShape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)width / (float)height;
    // glOrtho(-10.0, 10.0, -10.0, 10.0, 10.0, -10.0);
    gluPerspective(45.0, ratio, 1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


void SetMaterialColor(float r,float g,float b, float apha)
{
    GLfloat ambien[] = {r, g, b, apha};
    GLfloat diff_use[] = {r,g, b, apha};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambien);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_use);
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(5.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    SetMaterialColor(1,1,0,0);
    glPushMatrix();
    glRotatef(-70,0,1,0);
    glTranslatef(0,0,0);
    glRotatef(90,1,0,0);
    glCallList(g_base);
    glRotatef(-65,1,0,0);
    glTranslatef(-0.6,0,-0.8);
    glCallList(g_onbase);
    glRotatef(-25,1,0,0);
    glCallList(g_back);
    glTranslatef(0.2,0,0);
    glCallList(g_neck);
    glTranslatef(0,3,0);
    glRotatef(30,1,0,0);
    glCallList(g_head);
    glTranslatef(0.1,-2,1.5);
    glRotatef(90,1,0,0);
    glCallList(g_head);
    SetMaterialColor(1,128.0/255.0,0,0);
    glRotatef(-120,1,0,0);
    glTranslatef(0.1,0.5,-0.64);
    glCallList(g_hair);
    glPopMatrix();
    glFlush();
}

void Init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = {0.0, 0.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    GLfloat ambient[] = {1.0, 1.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

    GLfloat diff_use[] = {0.5, 0.5, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_use);

    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

    GLfloat shininess = 50.0f;
    glMateriali(GL_FRONT, GL_SHININESS, shininess);

    g_base = MakeOctagon(1,0.5);
    g_onbase = MakeBox(1.3,1,1.3);
    g_back = MakeBox(1.3,0.5,1.1);
    g_neck = MakeBox(0.8,0.6,3);
    g_head = MakeBox(0.8,1.3,0.6);
    g_hair = MakeBox(0.1,0.8,2.5);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Loc");
    Init();
    glutReshapeFunc(ReShape);
    glutDisplayFunc(RenderScene);
    glutMainLoop();
    return 0;
}

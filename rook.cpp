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

void DrawCoordinate() {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 10.0);
    glEnd();

    glEnable(GL_LIGHTING);
}


void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    DrawCoordinate();
    SetMaterialColor(1,1,1,0);    
    glTranslatef(0,0,0);
    glRotatef(90,1,0,0);
    glCallList(g_base);

    glRotatef(-90,1,0,0);

    SetMaterialColor(0.6,0.6,0.6,0);
    glTranslatef(-0.75,0,0.75);
    glRotatef(-90,1,0,0);
    glCallList(g_onbase);
    
    glRotatef(90,1,0,0);
    glTranslatef(0.75,0,-0.75);

    SetMaterialColor(1,1,1,0); 
    glTranslatef(0,0.2,0);
    glCallList(g_back);

    glTranslatef(0,-0.2,0);
    
    SetMaterialColor(0.6,0.6,0.6,0);
    glTranslatef(0,1.8,0);
    glRotatef(-90,1,0,0);
    glCallList(g_neck);

    glRotatef(90,1,0,0);
    glTranslatef(0,-1.8,0);
    
    SetMaterialColor(1,1,1,0);
    glTranslatef(0.45,2.38,-0.2);
    glRotatef(-90,0,0,1);
    glCallList(g_head);

    glRotatef(90,0,0,1);
    glTranslatef(-0.45,-2.38,0.2);

    glTranslatef(-0.45,2.28,-0.2);
    glRotatef(90,0,0,1);
    glCallList(g_head);

    glRotatef(-90,0,0,1);
    glTranslatef(0.45,-2.28,0.2);

    glTranslatef(-0.2,2.28,0.45);
    glRotatef(90,0,0,1);
    glRotatef(90,1,0,0);
    glCallList(g_head);

    glRotatef(-90,1,0,0);
    glRotatef(-90,0,0,1);
    glTranslatef(0.2,-2.28,-0.45);

    glTranslatef(-0.2,2.28,-0.65);
    glRotatef(90,0,0,1);
    glRotatef(90,1,0,0);
    glCallList(g_head);

    glRotatef(-90,1,0,0);
    glRotatef(-90,0,0,1);
    glTranslatef(0.2,-2.28,0.65);

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
    g_onbase = MakeBox(1.5,0.2,1.5);
    g_back = MakeFrustumShape(1.1,0.5,2);
    g_neck = MakeOctagon(0.5,0.5);
    g_head = MakeBox(0.1,0.4,0.2);
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

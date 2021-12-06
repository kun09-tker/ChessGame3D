#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>

#include <iostream>

void ReShape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)width / (float)height;
    // glOrtho(-10.0, 10.0, -10.0, 10.0, 10.0, -10.0);
    gluPerspective(45.0, ratio, 1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // SetMaterialColor(1, 1, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    // glCallList(g_box);
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

    // g_box = MakeOctagon(1.5, 0.8);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Chess");
    Init();
    glutReshapeFunc(ReShape);
    glutDisplayFunc(RenderScene);
    glutMainLoop();
    return 0;
}
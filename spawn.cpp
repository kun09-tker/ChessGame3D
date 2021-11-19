#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <iostream>

#include "tool.cpp"
using namespace std;

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

float fraction = 0.1f;

float zoomFactor = 1.0f;

// the key states. These variables will be zero
// when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

void Draw();
void Initialize();
void ReShape(GLint, GLint);
void mouseWheel(int, int, int, int);
void processSpecialKeys(int, int, int);
void pressKey(int, int, int);
void releaseKey(int, int, int);
void mouseButton(int, int, int, int);
void mouseMove(int, int);

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Chess");
    Initialize();

    // register callbacks
    glutDisplayFunc(Draw);
    glutReshapeFunc(ReShape);
    glutIdleFunc(Draw);

    glutIgnoreKeyRepeat(1);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);

    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutMouseWheelFunc(mouseWheel);

    glutMainLoop();
    return 0;
}

void mouseWheel(int button, int dir, int x, int y) {
    // zoomFactor += dir > 0 ? 0.1 : -0.1;
    if (dir > 0) {
        x += lx * fraction;
        z += lz * fraction;
    } else {
        x -= lx * fraction;
        z -= lz * fraction;
    }
    glutPostRedisplay();
}

void ReShape(GLint width, GLint height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // zoomFactor *= ((float)width / (float)height);
    // atan(tan(50.0 * 3.14159 / 360.0) / zoomFactor) * 360.0 / 3.14159
    gluPerspective(45.0, -(GLfloat)width / (GLfloat)height, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove) {
    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {
    angle += deltaAngle;
    lx = sin(angle);
    lz = -cos(angle);
}

void pressKey(int key, int xx, int yy) {
    switch (key) {
    case GLUT_KEY_LEFT:
        deltaAngle = -0.01f;
        break;
    case GLUT_KEY_RIGHT:
        deltaAngle = 0.01f;
        break;
    case GLUT_KEY_UP:
        deltaMove = 0.5f;
        break;
    case GLUT_KEY_DOWN:
        deltaMove = -0.5f;
        break;
    }
}

void releaseKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        deltaAngle = 0.0f;
        break;
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
        deltaMove = 0;
        break;
    }
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    if (deltaMove)
        computePos(deltaMove);
    if (deltaAngle)
        computeDir(deltaAngle);

    // Set material properties
    GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
    GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat qaBlue[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat qaLowAmbient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat qaFullAmbient[] = {1.0, 1.0, 1.0, 1.0};

    glShadeModel(GL_SMOOTH);

    // Set, ambient, diffuse and specular lighting. Set ambient to 20%.
    glMaterialfv(GL_FRONT, GL_AMBIENT, qaWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);

    // Draw a sphere
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
    DrawCoordinate();

    glutSolidSphere(.5, 20, 20);
    glPopMatrix();

    // Turn off diffuse and specular reflection. Set ambient light to full.
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlack);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaBlack);
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaFullAmbient);

    glFlush();
    glutSwapBuffers();
}

void Initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);

    // Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat qaDiffuseLight[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat qaSpecularLight[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = {0.0, 1.0, -.5, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}

void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
    case GLUT_KEY_LEFT:
        angle -= 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_RIGHT:
        angle += 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_UP:
        x += lx * fraction;
        z += lz * fraction;
        break;
    case GLUT_KEY_DOWN:
        x -= lx * fraction;
        z -= lz * fraction;
        break;
    }
}

void mouseButton(int button, int state, int x, int y) {
    // cout << xOrigin << endl;
    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        // when the button is released
        cout << state << " " << GLUT_UP << endl;
        if (state == GLUT_UP) {
            angle += deltaAngle;
            xOrigin = -1;
            deltaAngle = 0.0f;
            deltaMove = 0.0f;
        } else {  // state = GLUT_DOWN
            xOrigin = x;
        }
    }
}

void mouseMove(int x, int y) {
    // this will only be true when the left button is down
    cout << xOrigin << " " << x << " " << y << endl;
    if (xOrigin >= 0) {
        // update deltaAngle
        deltaAngle = (x - xOrigin) * 0.0001f;

        // update camera's direction
        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
    }
}
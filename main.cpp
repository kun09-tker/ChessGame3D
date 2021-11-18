#include<iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include "view.cpp"
#include "tool.cpp"
using namespace std;

class Cell
{
    public:
        GLfloat shape;   // hình dạng con cờ
        string name;    // tên cờ
        Cell(string n = "empty",GLfloat s = 0){
            this->name = n;
            this->shape = s; 
        }
};

Cell MatrixBroad[8][8];

GLfloat g_board;
float zoom = 1;

void mouseMove(int x, int y)
{
    cout << x << " " << y << endl;
}

void mouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        zoom -= 0.1; // Zoom in
    }
    else
    {
        zoom += 0.1;// Zoom out
    }
    glutPostRedisplay();
    return;
}

void ReShape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)width / (float)height;
    // glOrtho(-10.0, 10.0, -10.0, 10.0, 10.0, -10.0);
    gluPerspective(45.0, ratio, 1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// mỗi lần đi là cell width = 3
void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(15.0, 15.0, 13.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPushMatrix();
    glScalef(zoom,zoom,zoom); // Zoom In and Zoom Out
    DrawCoordinate();
    glTranslatef(0,-1,0); // để cho bằng với mp Oxz 
    glCallList(g_board);
    glTranslatef(1.5,1.5,1.5); // để cho con cờ nằm vừa trong ô 

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if(MatrixBroad[i][j].name != "empty"){
                glTranslatef(i*3,0,j*3);
                glCallList(MatrixBroad[i][j].shape);
                glTranslatef(-i*3,0,-j*3);
            }
        }
    }
    
    // glCallList(g_rook);
    // glTranslatef(3,0,0); // khoảng cách 2 con cờ
    // glCallList(g_knight);
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}
void Init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // GLfloat light_pos[] = {0.0, 0.25, 1.0, 0.0};
    // glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    GLfloat ambient[] = {1.0, 1.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

    GLfloat diff_use[] = {0.5, 0.5, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_use);

    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

    GLfloat shininess = 50.0f;
    glMateriali(GL_FRONT, GL_SHININESS, shininess);

    string listName[16] = {
        "R1","N1","B1","Q","K","B2","N2","R2","P1","P2","P3","P4","P5","P6","P7","P8"
    };
    
    //K : Vua
    //Q : Hậu
    //R : Xe
    //B : Tượng
    //N : Mã
    //P : Tốt

    //KB : Vua bên xanh dương
    //X1B: Xe 1 bên xanh dương

    GLfloat listShape[12] = {
        Rook_view("blue"), Knight_view("blue"),Rook_view("blue"),Rook_view("blue"),Rook_view("blue"),Knight_view("blue"),
        Rook_view("yellow"), Knight_view("yellow"),Rook_view("yellow"),Rook_view("yellow"),Rook_view("yellow"),Knight_view("yellow")
    };

    for (int i = 0; i < 8; i++){
        MatrixBroad[0][i] = Cell(listName[i]+"B",listShape[i < 5 ? i : i - 3 - i%5 * 2]);      // Xây hàng xe, ngựa, ... trắng
        MatrixBroad[1][i] = Cell(listName[i+8]+"B",listShape[5]);                             // Xây hàng tốt trắng
    }
    for (int i = 0; i < 8; i++){
        MatrixBroad[7][7-i] = Cell(listName[i]+"Y",listShape[(i < 5 ? i : i - 3 - i%5 * 2)+6]);     // Xây hàng xe, ngựa, ... đen
        MatrixBroad[6][7-i] = Cell(listName[i+8]+"Y",listShape[11]);                            // Xây hàng tốt đen
    }
    g_board = Board_view(3,1);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Chess");
    Init();
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            cout << MatrixBroad[i][j].shape << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            cout << MatrixBroad[i][j].name << " ";
        }
        cout << endl;
    }
    glutReshapeFunc(ReShape);
    glutDisplayFunc(RenderScene);
    glClearColor(0.0, 0.6, 1.0, 1.0); // Blue background
    glutMouseWheelFunc(mouseWheel);
    glutMotionFunc(mouseMove);
    glutMainLoop();
    return 0;
}







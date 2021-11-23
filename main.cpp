#include<iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <math.h>
#include "view.h"
#include "rule.h"

using namespace std;

string player = "BULE";              // đội xanh đi đầu tiên
int x,y;
float dx=-1,dy=-1;              // khoảng cách hai ô
float xtmp = 1, ytmp = 1;       // dấu của khoảng cách 1 là dương, -1 là âm
float down = 0;
int x_old = -1, y_old = -1;     // lưu vị trí cũ xử lý trong process input
int x_old_copy, y_old_copy;     // lưu vị trí cũ sử dụng ngoài process input 
int x_down, y_down;             // lưu vị trí cờ bị ăn
Cell MatrixBoard[8][8];
GLfloat g_board;
GLfloat g_chessDown;
float zoom = 10;
bool animationMove = false;
bool animationDown = false;

void mouseMove(int x, int y)
{
    cout << x << " " << y << endl;
}

void mouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        zoom -= 0.5; // Zoom in
    }
    else
    {
        zoom += 0.5;// Zoom out
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
    gluLookAt(zoom, 15.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPushMatrix();
    DrawCoordinate();
    glTranslatef(0,-1,0); // để cho bằng với mp Oxz 
    glCallList(g_board);
    glTranslatef(1.5,1.5,1.5); // để cho con cờ nằm vừa trong ô 
    drawTextColor("TURN: ",10,10,1,1,1,1);
    if(player=="BULE") drawTextColor("Player01",10,9,0,1,1,1);
    else drawTextColor("Player02",10,9,1,1,0,1);
    if(animationMove){
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                Cell Local = MatrixBoard[i][j];
                if(i!=x_old || j!=y_old){
                    if(Local.name != "empty"){
                        glTranslatef(i*3,0,j*3);
                        glCallList(MatrixBoard[i][j].shape);
                        glTranslatef(-i*3,0,-j*3);
                    }
                    if(Local.choose || Local.move){
                        glTranslatef(i*3-1.5,-1.49,j*3-1.5);
                        glCallList(StatusCell_view("green"));
                        glTranslatef(-i*3+1.5,1.49,-j*3+1.5);
                    }
                    if(Local.target){
                        glTranslatef(i*3-1.5,-1.49,j*3-1.5);
                        glCallList(StatusCell_view("red"));
                        glTranslatef(-i*3+1.5,1.49,-j*3+1.5);
                    }
                }

            }
        }
        if(animationDown){
            //cout << x_old << " " << y_old << endl;
            glTranslatef(x_old_copy*3,0,y_old_copy*3);
            glCallList(MatrixBoard[x][y].shape);
            glTranslatef(-x_old_copy*3,0,-y_old_copy*3);

            glTranslatef(x_down*3,-down,y_down*3);
            glCallList(g_chessDown);
            glTranslatef(-x_down*3,down,-y_down*3); 
        }
        else{
            glTranslatef((x+dx)*3,0,(y+dy)*3);
            glCallList(MatrixBoard[x][y].shape);
            glTranslatef(-(x+dx)*3,0,-(y+dy)*3);
        }

    }

    else{
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                glPushName(i*8+j);
                Cell Local = MatrixBoard[i][j];
                if(Local.name != "empty"){
                    glTranslatef(i*3,0,j*3);
                    glCallList(MatrixBoard[i][j].shape);
                    glTranslatef(-i*3,0,-j*3);
                }
                if(Local.choose || Local.move){
                    glTranslatef(i*3-1.5,-1.49,j*3-1.5);
                    glCallList(StatusCell_view("green"));
                    glTranslatef(-i*3+1.5,1.49,-j*3+1.5);
                }
                if(Local.target){
                    glTranslatef(i*3-1.5,-1.49,j*3-1.5);
                    glCallList(StatusCell_view("red"));
                    glTranslatef(-i*3+1.5,1.49,-j*3+1.5);
                }

            }
        }
    }

    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void timer(int){
    if(animationMove){
        glutPostRedisplay();
        glutTimerFunc(5,timer,0);
        if(animationDown){
            down = down + 0.01; 
            if(int(down)==5){
                animationDown = false;
                down = 0;
            }
        }
        else{
            dx = (int(dx*100)==0) ? 0 : dx = dx - (0.01*xtmp); 
            dy = (int(dy*100)==0) ? 0 : dy = dy - (0.01*ytmp);
            if(int(dx*100)==0 && int(dy*100)==0){
                animationMove = false;
            }
        }
    }
}

void processInput(int x, int y){
    if(ruleCheckBoundary(x,y)){
        if(x_old!=-1 && y_old!=-1) MatrixBoard[x_old][y_old].choose = false;
        MatrixBoard[x][y].choose = true;                              // chọn cờ
        if((MatrixBoard[x][y].move || MatrixBoard[x][y].target)){       // di chuyển cờ và ăn cờ đối phương
            
            x_down = x;
            y_down = y;

            if(MatrixBoard[x][y].target){
                animationDown = true;
                g_chessDown = MatrixBoard[x][y].shape;
                x_old_copy = x_old; y_old_copy = y_old; 
            }

            dx = float(x_old - x);
            dy = float(y_old - y);
            xtmp = (dx>0) ? 1 : -1;
            ytmp = (dy>0) ? 1 : -1;  
            animationMove = true;

            ruleSwapChess(MatrixBoard,x_old,y_old,x,y,x_down,y_down,g_chessDown);

            glutTimerFunc(0,timer,0);                           
            
            if(MatrixBoard[x][y].name[1]!=player[0]) ruleDirection(MatrixBoard,x,y);
            MatrixBoard[x_old][y_old].choose = false;
            MatrixBoard[x][y].choose = true;
            player = ruleSwapPlayer(player);

        }
        else if(!MatrixBoard[x][y].move && MatrixBoard[x][y].name[1]==player[0]){       // xác định hướng đi của cờ
            ruleClear(MatrixBoard);
            ruleDirection(MatrixBoard,x,y);
        }
        else if(!MatrixBoard[x][y].move){
            ruleClear(MatrixBoard);
        }
        // for (int i = 0; i < 8; i++){
        //     for (int j = 0; j < 8; j++){
        //         cout << MatrixBoard[i][j].name << " " ;
        //     }
        //     cout << endl;
        // }
    }
    x_old = x; y_old = y;
    glutPostRedisplay();
}

int dem = 0; 

void tasten(unsigned char key, int xmouse, int ymouse)
{	
    if(dem == 0){
        x = key - '0';
        dem++;
        cout << "x = " << x << endl;
    }
    else if(dem == 1){
        y = key - '0';
        cout << "y = " << y << endl;
        processInput(x,y);
        dem = 0;
    }
}

void Init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // Lighting set up
    glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);


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

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    string listName[6] = {
        "R","N","B","Q","K","P"
    };
    
    //K : Vua
    //Q : Hậu
    //R : Xe
    //B : Tượng
    //N : Mã
    //P : Tốt

    //KB : Vua bên xanh dương

    GLfloat listShape[12] = {
        Rook_view("blue"), Knight_view("blue"),Rook_view("blue"),Rook_view("blue"),Rook_view("blue"),Knight_view("blue"),
        Rook_view("yellow"), Knight_view("yellow"),Rook_view("yellow"),Rook_view("yellow"),Rook_view("yellow"),Knight_view("yellow")
    };

    for (int i = 0; i < 8; i++){
        int tmp = i < 5 ? i : i - 3 - i%5 * 2;
        MatrixBoard[0][i] = Cell(listName[tmp]+"B",listShape[tmp]);      // Xây hàng xe, ngựa, ... xanh dương
        MatrixBoard[1][i] = Cell(listName[5]+"B",listShape[5]);            // Xây hàng tốt xanh dương
        MatrixBoard[7][7-i] = Cell(listName[tmp]+"Y",listShape[tmp+6]);     // Xây hàng xe, ngựa, ... vàng
        MatrixBoard[6][7-i] = Cell(listName[5]+"Y",listShape[11]);           // Xây hàng tốt vàng
    }
    g_board = Board_view(3,1);
}


int main(int argc, char* argv[]) {
    while(true){
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(1000, 1000);
        glutInitWindowPosition(100, 100);
        glutCreateWindow("3D Chess");
        Init();
        glutReshapeFunc(ReShape);
        glutDisplayFunc(RenderScene);
        glutKeyboardFunc(tasten);
        glutMouseWheelFunc(mouseWheel);
        glutMainLoop();
    }
    return 0;
}







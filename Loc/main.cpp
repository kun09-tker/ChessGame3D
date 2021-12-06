#include <GL/freeglut.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "rule.h"
#include "view.h"

using namespace std;

string player = "BULE";  // đội xanh đi đầu tiên

int x = 0, y = 0;            // tọa độ ô cờ chọn
int xmove = 0, ymove = 0;    // tọa độ di chuyển
int x_old = -1, y_old = -1;  // lưu vị trí cũ xử lý trong process input
int x_old_copy, y_old_copy;  // lưu vị trí cũ sử dụng ngoài process input
int x_down, y_down;          // lưu vị trí cờ bị ăn

Cell MatrixBoard[8][8];

GLfloat g_board;      // hình dạng bàn cờ
GLfloat g_chessDown;  // lưu hình dạng con cờ bị ăn
GLfloat g_chessWait;
GLfloat g_box;

GLuint texture;  // quản lý texture

char texture_name[100] = {"./shinichi.bmp"};  // lưu trữ tên file texture
char texture_name_clear[100] = {"./clear.bmp"};

float eyeX = 16, eyeY = 33, eyeZ = -21;
float centerX = 16, centerY = 2, centerZ = 7;
float dx = -1, dy = -1;    // khoảng cách hai ô
float xtmp = 1, ytmp = 1;  // dấu của khoảng cách 1 là dương, -1 là âm
float down = 0;            // lưu độ cao của cờ

bool animationMove = false;  // trạng thái di chuyển cờ
bool animationDown = false;  // trạng thái cờ bị ăn
bool promotion = false;      // phong cấp
bool en_passant = false;     // bắt tốt qua đường
bool castling = false;       // nhập thành

map<string, bool> chess_move;  // kiểm tra con cờ di chuyển hay chưa

vector<GLfloat> MatrixEat_Blue;    // lưu các con cờ xanh bị ăn
vector<GLfloat> MatrixEat_Yellow;  // lưu các con cờ vàng bị ăn

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
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
    glPushMatrix();
    DrawCoordinate();
    glTranslatef(0, -1, 0);  // để cho bằng với mp Oxz
    glCallList(g_board);
    glTranslatef(2, 1.5, 2);  // để cho con cờ nằm vừa trong ô
    if (player == "BULE") {
        drawTextColor("Player01", 0, 10, 8, 0, 1, 1, 1);
        drawTextColor("Player02", 30, 10, 8, 0.6, 0.6, 0.6, 1);
    } else {
        drawTextColor("Player01", 0, 10, 8, 0.6, 0.6, 0.6, 1);
        drawTextColor("Player02", 30, 10, 8, 1, 1, 0, 1);
    }
    glTranslatef(10.5, 12, 8);
    glCallList(Notification_view());
    // glCallList(Test());
    glTranslatef(-10.5, -12, -8);

    // glTranslatef(10.5, 20, 8);
    // glCallList(g_box);
    // glTranslatef(-10.5, -20, -8);

    if (animationMove) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Cell Local = MatrixBoard[i][j];
                if (i != x_old || j != y_old) {
                    if (Local.name != "empty") {
                        glTranslatef(i * 4, 0, j * 4);
                        glCallList(MatrixBoard[i][j].shape);
                        glTranslatef(-i * 4, 0, -j * 4);
                    }
                }
            }
        }
        if (animationDown) {
            // cout << x_old << " " << y_old << endl;
            glTranslatef(x_old_copy * 4, 0, y_old_copy * 4);
            glCallList(g_chessWait);
            glTranslatef(-x_old_copy * 4, 0, -y_old_copy * 4);

            glTranslatef(x_down * 4, -down, y_down * 4);
            glCallList(g_chessDown);
            glTranslatef(-x_down * 4, down, -y_down * 4);
        } else {
            glTranslatef((x + dx) * 4, 0, (y + dy) * 4);
            glCallList(MatrixBoard[x][y].shape);
            glTranslatef(-(x + dx) * 4, 0, -(y + dy) * 4);
        }

    }

    else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Cell Local = MatrixBoard[i][j];
                if (Local.name != "empty") {
                    glTranslatef(i * 4, 0, j * 4);
                    glCallList(MatrixBoard[i][j].shape);
                    glTranslatef(-i * 4, 0, -j * 4);
                }
                if (Local.choose || Local.move) {
                    glTranslatef(i * 4 - 2, -1.49, j * 4 - 2);
                    glCallList(StatusCell_view("green"));
                    glTranslatef(-i * 4 + 2, 1.49, -j * 4 + 2);
                }
                if (Local.target) {
                    glTranslatef(i * 4 - 2, -1.49, j * 4 - 2);
                    glCallList(StatusCell_view("red"));
                    glTranslatef(-i * 4 + 2, 1.49, -j * 4 + 2);
                }
            }
        }
        glTranslatef(xmove * 4 - 2, -1.48, ymove * 4 - 2);
        glCallList(StatusCell_view(player));
        glTranslatef(-xmove * 4 + 2, 1.48, -ymove * 4 + 2);
    }

    for (int i = 0; i < MatrixEat_Yellow.size(); i++) {
        int pos_x = (i < 8) ? -1 : -2;
        int pos_y = i % 8;

        glTranslatef(pos_x * 4, -1.5, pos_y * 4);
        glCallList(MatrixEat_Yellow[i]);
        glTranslatef(-pos_x * 4, 1.5, -pos_y * 4);
    }

    for (int i = 0; i < MatrixEat_Blue.size(); i++) {
        int pos_x = (i < 8) ? 8 : 9;
        int pos_y = i % 8;

        glTranslatef(pos_x * 4, -1.5, pos_y * 4);
        glCallList(MatrixEat_Blue[i]);
        glTranslatef(-pos_x * 4, 1.5, -pos_y * 4);
    }

    // cout << eyeX << endl;
    // cout << eyeY << endl;
    // cout << eyeZ << endl;
    // cout << centerX << endl;
    // cout << centerY << endl;
    // cout << centerZ << endl;
    // cout << endl << endl;

    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void timer(int) {
    if (animationMove) {
        glutPostRedisplay();
        glutTimerFunc(5, timer, 0);
        if (animationDown) {
            down = down + 0.01;
            if (int(down) == 5) {
                animationDown = false;
                down = 0;
            }
        } else {
            dx = (int(dx * 100) == 0) ? 0 : dx = dx - (0.01 * xtmp);
            dy = (int(dy * 100) == 0) ? 0 : dy = dy - (0.01 * ytmp);
            if (int(dx * 100) == 0 && int(dy * 100) == 0) {
                animationMove = false;
            }
        }
    }
}

void menuPromotion(int id) {
    animationMove = true;
    animationDown = true;
    promotion = false;
    x_down = x;
    y_down = y;
    g_chessDown = MatrixBoard[x_down][y_down].shape;
    g_chessWait = 0;
    glutTimerFunc(0, timer, 0);
    string P(1, player[0]);

    if (id == 1) {
        MatrixBoard[x][y] = Cell("Q" + P, Rook_view(player[0]));
    } else if (id == 2) {
        MatrixBoard[x][y] = Cell("B" + P, Rook_view(player[0]));
    } else if (id == 3) {
        MatrixBoard[x][y] = Cell("N" + P, Rook_view(player[0]));
    } else {
        MatrixBoard[x][y] = Cell("R" + P, Rook_view(player[0]));
    }

    player = ruleSwapPlayer(player);
    glutDestroyMenu(glutGetMenu());
    glutPostRedisplay();
}

void processInput(int x, int y) {
    if (ruleCheckBoundary(x, y)) {
        g_chessWait = MatrixBoard[x_old][y_old].shape;
        if (x_old != -1 && y_old != -1)
            MatrixBoard[x_old][y_old].choose = false;
        MatrixBoard[x][y].choose = true;  // chọn cờ
        if ((MatrixBoard[x][y].move ||
             MatrixBoard[x][y].target)) {  // di chuyển cờ và ăn cờ đối phương

            x_down = x;
            y_down = y;

            if (MatrixBoard[x][y].target) {
                animationDown = true;
                g_chessDown = MatrixBoard[x][y].shape;
                x_old_copy = x_old;
                y_old_copy = y_old;
            }

            dx = float(x_old - x);
            dy = float(y_old - y);
            xtmp = (dx > 0) ? 1 : -1;
            ytmp = (dy > 0) ? 1 : -1;
            animationMove = true;

            ruleSwapChess(MatrixBoard, MatrixEat_Blue, MatrixEat_Yellow, x_old, y_old, x, y, x_down,
                          y_down, g_chessDown, en_passant, castling, chess_move);

            if (MatrixBoard[x][y].name[1] != player[0])
                ruleDirection(MatrixBoard, x, y, en_passant, chess_move, castling);

            if (MatrixBoard[x][y].name[0] == 'P' &&
                (x == 0 || x == 7)) {  // tốt vị trí cuối ở đối thủ
                if (ruleCheckBoundary(x, y)) {
                    promotion = true;
                }
            }

            MatrixBoard[x_old][y_old].choose = false;
            MatrixBoard[x][y].choose = true;

            if (promotion) {
                glutCreateMenu(menuPromotion);
                glutAddMenuEntry("Queen", 1);
                glutAddMenuEntry("Bishop", 2);
                glutAddMenuEntry("Knight", 3);
                glutAddMenuEntry("Rook", 4);
                glutAttachMenu(GLUT_LEFT_BUTTON);
            } else {
                player = ruleSwapPlayer(player);
            }

        } else if (!MatrixBoard[x][y].move &&
                   MatrixBoard[x][y].name[1] == player[0]) {  // xác định hướng đi của cờ
            ruleClear(MatrixBoard);
            ruleDirection(MatrixBoard, x, y, en_passant, chess_move, castling);
        } else if (!MatrixBoard[x][y].move) {
            ruleClear(MatrixBoard);
        }
        // for (int i = 0; i < 8; i++){
        //     for (int j = 0; j < 8; j++){
        //         cout << MatrixBoard[i][j].name << " " ;
        //     }
        //     cout << endl;
        // }
        glutTimerFunc(0, timer, 0);
        cout << player << endl;
    }

    x_old = x;
    y_old = y;
    glutPostRedisplay();
}

int dem = 0;

void tasten(unsigned char key, int xmouse, int ymouse) {
    if (key == 'a') {
        xmove += 1;
    } else if (key == 'd') {
        xmove -= 1;
    } else if (key == 'w') {
        ymove += 1;
    } else if (key == 's') {
        ymove -= 1;
    }

    if (int(key) == 13) {
        x = xmove;
        y = ymove;
        processInput(x, y);
    }

    if (key == 'q') {
        player = ruleSwapPlayer(player);
    }
    glutPostRedisplay();
}

void Init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    string listName[6] = {"R", "N", "B", "Q", "K", "P"};

    // K : Vua
    // Q : Hậu
    // R : Xe
    // B : Tượng
    // N : Mã
    // P : Tốt

    // KB : Vua bên xanh dương

    GLfloat listShape[12] = {Rook_view('B'), Knight_view('B'), Rook_view('B'), Rook_view('B'),
                             Rook_view('B'), Knight_view('B'), Rook_view('Y'), Knight_view('Y'),
                             Rook_view('Y'), Rook_view('Y'),   Rook_view('Y'), Knight_view('Y')};

    MatrixEat_Blue.push_back(listShape[0]);
    MatrixEat_Yellow.push_back(listShape[8]);
    for (int i = 0; i < 8; i++) {
        int tmp = i < 5 ? i : i - 3 - i % 5 * 2;
        MatrixBoard[0][i] =
            Cell(listName[tmp] + "B", listShape[tmp]);  // Xây hàng xe, ngựa, ... xanh dương
        MatrixBoard[1][i] = Cell(listName[5] + "B", listShape[5]);  // Xây hàng tốt xanh dương
        MatrixBoard[7][7 - i] =
            Cell(listName[tmp] + "Y", listShape[tmp + 6]);  // Xây hàng xe, ngựa, ... vàng
        MatrixBoard[6][7 - i] = Cell(listName[5] + "Y", listShape[11]);  // Xây hàng tốt vàng
    }
    g_board = Board_view(4, 1);
    g_box = MakeCube(3);
    // texture =  loadBMP_custom(texture_name);
}

int main(int argc, char *argv[]) {
    while (true) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(1000, 1000);
        glutInitWindowPosition(100, 100);
        glutCreateWindow("3D Chess");
        Init();
        glutReshapeFunc(ReShape);
        glutDisplayFunc(RenderScene);
        glutKeyboardFunc(tasten);
        glutMainLoop();
    }
    return 0;
}
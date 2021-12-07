#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
using namespace std;

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

void SetLightColor(float r,float g,float b, float apha)
{
    GLfloat qaAmbientLight[] = {r, g, b, apha};

    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
}
void SetMaterialColor(float r,float g,float b, float apha)
{
    GLfloat ambien[] = {r, g, b, apha};
    GLfloat diff_use[] = {r,g, b, apha};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambien);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_use);
}

void renderBitMap(float x, float y,float z, void *font, char *string, float xScale = 0, float yScale = 0, float zScale = 0) {
	char *c;
	c = string;
	glPushMatrix();
    glRasterPos3f(x,y,z);
	for (c = string; *c != '\0'; c++) {
		glScalef(xScale, yScale, zScale);
		glutBitmapCharacter(font, (int)*c);
	}
	glPopMatrix();
}
void drawTextColor(const char* text,float x, float y,float z, float r, float g, float b, float apha , float xScale = 0, float yScale = 0, float zScale = 0){
	char buf[1000] = {0};
    sprintf_s(buf, text);
    SetLightColor(r,g,b,apha);
	renderBitMap(x, y,z, GLUT_BITMAP_TIMES_ROMAN_24, buf, xScale, yScale, zScale);
    SetLightColor(0,0,0,1);
}


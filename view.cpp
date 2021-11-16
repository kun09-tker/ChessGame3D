#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "shape.cpp"

GLuint Knight_view()
{
  GLfloat g_base;
  GLfloat g_onbase;
  GLfloat g_back;
  GLfloat g_neck;
  GLfloat g_head;
  GLfloat g_hair;
  GLuint dp_list;

  g_base = MakeOctagon(1,0.5);
  g_onbase = MakeBox(1.3,1,1.3);
  g_back = MakeBox(1.3,0.5,1.1);
  g_neck = MakeBox(0.8,0.6,3);
  g_head = MakeBox(0.8,1.3,0.6);
  g_hair = MakeBox(0.1,0.8,2.5);

  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
    glPushMatrix();
    SetMaterialColor(1,1,1,0);
    glTranslatef(0,0,0);
    glRotatef(90,1,0,0);
    glCallList(g_base);

    SetMaterialColor(1,1,1,0);
    glRotatef(-65,1,0,0);
    glTranslatef(-0.6,0,-0.8);
    glCallList(g_onbase);

    SetMaterialColor(1,1,1,0);
    glRotatef(-25,1,0,0);
    glCallList(g_back);

    SetMaterialColor(1,1,1,0);
    glTranslatef(0.2,0,0);
    glCallList(g_neck);

    SetMaterialColor(0.6,0.6,0.6,0);
    glTranslatef(0,3,0);
    glRotatef(30,1,0,0);
    glCallList(g_head);

    SetMaterialColor(1,1,1,0);
    glTranslatef(0.1,-2,1.5);
    glRotatef(90,1,0,0);
    glCallList(g_head);

    SetMaterialColor(0.6,0.6,0.6,0);
    glRotatef(-120,1,0,0);
    glTranslatef(0.1,0.5,-0.64);
    glCallList(g_hair);
    glPopMatrix();
  glEndList();
 
  return dp_list;
}
GLint Rook_view(){
  GLfloat g_base;
  GLfloat g_onbase;
  GLfloat g_back;
  GLfloat g_neck;
  GLfloat g_head;

  g_base = MakeOctagon(1,0.5);
  g_onbase = MakeBox(1.5,0.2,1.5);
  g_back = MakeFrustumShape(1.1,0.5,2);
  g_neck = MakeOctagon(0.5,0.5);
  g_head = MakeBox(0.1,0.4,0.2);

  GLuint dp_list;

  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
    glPushMatrix();
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
  glEndList();

  return dp_list;
}


// Board ----------------------------------------------------------------------

GLint Board_view(float width, float height)
{
  GLuint dp_list;
  GLfloat square = MakeBox(width,width,height);
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  for (int i = 0; i < 8; i++){
    for (int j = 0; j < 8; j++){
      glPushMatrix();
      if ((i+j) % 2 == 0){
        SetMaterialColor(1,1,1,0);    // White color
      }
      else {
        SetMaterialColor(0,0,0,0); // Black color
      }
      glTranslatef(i*width, 0, j*width); // Translate cell
      glCallList(square);
      glPopMatrix();
    }
  }
  glEndList();
  return dp_list;
}
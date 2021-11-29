#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>


GLuint MakeBox(const float length, const float width, const float height)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  float x = length;
  float y = height;
  float z = width ;
 
  //Back
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(0, 0, 0);
  glVertex3f(x, 0, 0);
  glVertex3f(x, y, 0);
  glVertex3f(0, y, 0);
  glEnd();
 
  // left
  glBegin(GL_QUADS);
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, z);
  glVertex3f(0, y, z);
  glVertex3f(0, y, 0);
  glEnd();
 
  //front
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0, 0, z);
  glVertex3f(0, y, z);
  glVertex3f(x, y, z);
  glVertex3f(x, 0, z);
  glEnd();
 
  //// right
  glBegin(GL_QUADS);
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(x, 0, z);
  glVertex3f(x, 0, 0);
  glVertex3f(x, y, 0);
  glVertex3f(x, y, z);
  glEnd();
 
  //Top
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0, y, 0);
  glVertex3f(x, y, 0);
  glVertex3f(x, y, z);
  glVertex3f(0, y, z);
 
  //Bottom
  glBegin(GL_QUADS);
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(0, 0, 0);
  glVertex3f(x, 0, 0);
  glVertex3f(x, 0, z);
  glVertex3f(0, 0, z);
 
  glEnd();
 
  glEndList();
 
  return dp_list;
}


GLuint MakeSphere(const float& radius)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  glutSolidSphere(radius, 64, 64);
  glEndList();
  
  return dp_list;
}

GLuint MakeCylinder(const float& radius, const float& length)
{
  GLuint dp_list;
  dp_list = glGenLists(1); 
  glNewList(dp_list, GL_COMPILE);
  GLUquadricObj *quadratic_obj;
  quadratic_obj = gluNewQuadric();
  glRotatef(-90, 1.0, 0.0, 0.0);
  gluCylinder(quadratic_obj, radius, radius, length, 32, 32);
  glEndList();
  return dp_list;
}

GLuint MakeTruncatedCone(const float& base_rad, const float & top_rad, const float& length)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  GLUquadricObj *quadratic_obj;
  quadratic_obj = gluNewQuadric();
  gluCylinder(quadratic_obj, base_rad, top_rad, length, 32, 32);
  glEndList();
  
  return dp_list;
}

GLuint MakeCone(const float& base_rad, const float& length)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  GLUquadricObj *quadratic_obj;
  quadratic_obj = gluNewQuadric();
  gluCylinder(quadratic_obj, base_rad, 0.0, length, 32, 32);
  glEndList();
 
  return dp_list;
}
 
GLuint MakePyramid(const float& size, const float& height)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  double half_size = size*0.5;
  glBegin( GL_TRIANGLES );
  //Front face
  glNormal3f(0.0, 0.0, 1.0f);
  glVertex3f(0.0f, height, 0.0f);
  glVertex3f(-half_size, 0, half_size);
  glVertex3f(half_size, 0, half_size);
 
  //left face
  glNormal3f(-1.0, 0.0, 0.0f);
  glVertex3f(0.0, height, 0.0);
  glVertex3f(-half_size, 0.0, -half_size);
  glVertex3f(-half_size, 0.0, half_size);
 
  //back face
  glNormal3f(0.0, 0.0, -1.0f);
  glVertex3f(0.0f, height, 0.0f);
  glVertex3f(-half_size, 0, -half_size);
  glVertex3f(half_size, 0, -half_size);
 
  //Right face
  glNormal3f(1.0, 0.0, 0.0f);
  glVertex3f(0.0, height, 0.0);
  glVertex3f(half_size, 0.0, -half_size);
  glVertex3f(half_size, 0.0, half_size);
  glEnd();
 
  //Bottom face
  glBegin(GL_QUADS);
  glNormal3f(0.0, -1.0, 0.0f);
  glVertex3f(half_size, 0.0, half_size);
  glVertex3f(half_size, 0.0, -half_size);
  glVertex3f(-half_size, 0.0, -half_size);
  glVertex3f(-half_size, 0.0, half_size);
  glEnd();
  glEndList();
 
  return dp_list;
}
GLuint MakeFrustumShape(const float& bottom_size, const float& top_size, const float& height)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  double half_bottom_size = 0.5*bottom_size;
  double half_top_size = 0.5*top_size;
  
  glBegin(GL_QUADS);
  // Front Face
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3f(-half_bottom_size, 0.0, half_bottom_size);
  glVertex3f(half_bottom_size, 0.0, half_bottom_size);
  glVertex3f(half_top_size, height, half_top_size);
  glVertex3f(-half_top_size, height, half_top_size);
  // Back Face
  glNormal3f(0.0, 0.0, -1.0);
  glVertex3f(-half_bottom_size, 0.0, -half_bottom_size);
  glVertex3f(half_bottom_size, 0.0, -half_bottom_size);
  glVertex3f(half_top_size, height, -half_top_size);
  glVertex3f(-half_top_size, height, -half_top_size);
  
  // Top Face
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(-half_top_size, height, -half_top_size);
  glVertex3f(-half_top_size, height, half_top_size);
  glVertex3f(half_top_size, height, half_top_size);
  glVertex3f(half_top_size, height, -half_top_size);
  // Bottom Face
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(-half_bottom_size, 0.0, -half_bottom_size);
  glVertex3f( half_bottom_size, 0.0, -half_bottom_size);
  glVertex3f( half_bottom_size, 0.0, half_bottom_size);
  glVertex3f(-half_bottom_size, 0.0, half_bottom_size);
  // Right face
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f(half_bottom_size, 0.0, -half_bottom_size);
  glVertex3f(half_bottom_size, 0.0, half_bottom_size);
  glVertex3f(half_top_size, height, half_top_size);
  glVertex3f(half_top_size, height, -half_top_size);
  // Left Face
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(-half_bottom_size, 0.0, -half_bottom_size);
  glVertex3f(-half_bottom_size, 0.0, half_bottom_size);
  glVertex3f(-half_top_size, height, half_top_size);
  glVertex3f(-half_top_size, height, -half_top_size);
  glEnd();
  
  glEndList();
  
  return dp_list;
}
 
 GLuint MakeOctagon(const float& side, const float& thickness)
{
  GLuint dp_list;
  dp_list = glGenLists(1);
  glNewList(dp_list, GL_COMPILE);
  double anpha = 3.14159265/4.0;
  float x = sin(anpha) * side;
  float y = 0.5*side;
 
  float z = thickness;
  float center_to_mid_size = x + y;
  for (int j = 0; j < 8; j++)
  {
    glPushMatrix();
    glTranslatef(-center_to_mid_size, 0.0, 0.0);
    //Draw 8 rectangle side
    glBegin(GL_QUADS);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(0.0, -y, z);
    glVertex3f(0.0, y, z);
    glVertex3f(0.0, y, 0);
    glVertex3f(0.0, -y, 0);
    glEnd();
    glPopMatrix();
 
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, z);
    glVertex3f(-center_to_mid_size, -y, z);
    glVertex3f(-center_to_mid_size, y, z);
 
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(-center_to_mid_size, y, 0.0);
    glVertex3f(-center_to_mid_size, -y, 0.0);
    glEnd();
 
    glRotatef(45.0, 0.0, 0.0, 1.0);
   }
 
   glEndList();
 
   return dp_list;
}
 
GLuint MakeRWindow(const float &bottom_size, const float &top_size, const float &height, const float length)
{
    GLuint dp_list;
    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    double half_bottom_size = 0.5 * bottom_size;
    double half_top_size = 0.5 * top_size;
    glBegin(GL_QUADS);
    // Front Face
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(half_bottom_size + length, 0.0, 0.0);
    glVertex3f(half_top_size + length - 0.7, height, 0.0);
    glVertex3f(0.0, height, 0.0);
    glEnd();

    glEndList();

    return dp_list;
}

GLuint MakeCube(const float& size)
{
  GLuint boxDisplay;
  boxDisplay=glGenLists(1);
  glNewList(boxDisplay,GL_COMPILE);
 
  glBegin(GL_QUADS);
  // Front Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, size);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, size);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
  // Back Face
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
  glVertex3f(-size, size, -size);
  glVertex3f( size, size, -size);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, -size);
  // Top Face
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( size, size, size);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, -size);
  // Bottom Face
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( size, -size, -size);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, size);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
  // Right face
  glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, -size);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( size, size, -size);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( size, size, size);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( size, -size, size);
  // Left Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
  glEnd();
 
  glEndList();
  return boxDisplay;
}
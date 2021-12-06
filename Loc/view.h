#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include <iostream>

#include "shape.h"
#include "tool.h"

#define GL_BGR 0x80E0

using namespace std;

GLuint Knight_view(char name_color) {
    int color[3] = {0, 1, 1};
    if (name_color == 'Y') {
        color[0] = 1;
        color[1] = 1;
        color[2] = 0;
    }
    
    GLfloat g_base;
    GLfloat g_onbase;
    GLfloat g_onbase1;
    GLfloat g_back;
    GLfloat g_neck;
    GLfloat g_head;
    GLfloat g_hair;
    GLuint dp_list;

    g_base = MakeOctagon(1, 0.5);
    g_onbase = MakeBox(1.3, 1, 1.3);
    g_onbase1 = MakeBox(1.5, 0.2, 1.5);
    g_back = MakeBox(1.3, 0.5, 1.1);
    g_neck = MakeBox(0.8, 0.6, 3);
    g_head = MakeBox(0.8, 1.3, 0.6);
    g_hair = MakeBox(0.1, 0.8, 2.5);

    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    glPushMatrix();

    if (name_color == 'Y')
        glRotatef(-90, 0, 1, 0);
    else
        glRotatef(90, 0, 1, 0);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glTranslatef(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    glCallList(g_base);

    glRotatef(-90, 1, 0, 0);

    SetMaterialColor(0.6, 0.6, 0.6, 1);
    glTranslatef(-0.75, 0, 0.75);
    glRotatef(-90, 1, 0, 0);
    glCallList(g_onbase1);

    glRotatef(-180, 1, 0, 0);
    glTranslatef(0.6, -0.5, 0);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glRotatef(-65, 1, 0, 0);
    glTranslatef(-0.6, 0, -0.8);
    glCallList(g_onbase);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glRotatef(-25, 1, 0, 0);
    glCallList(g_back);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glTranslatef(0.2, 0, 0);
    glCallList(g_neck);

    SetMaterialColor(0.6, 0.6, 0.6, 1);
    glTranslatef(0, 3, 0);
    glRotatef(30, 1, 0, 0);
    glCallList(g_head);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glTranslatef(0.1, -2, 1.5);
    glRotatef(90, 1, 0, 0);
    glCallList(g_head);

    SetMaterialColor(0.6, 0.6, 0.6, 1);
    glRotatef(-120, 1, 0, 0);
    glTranslatef(0.1, 0.5, -0.64);
    glCallList(g_hair);
    glPopMatrix();
    glEndList();

    return dp_list;
}
GLint Rook_view(char name_color) {
    int color[3] = {0, 1, 1};
    if (name_color == 'Y') {
        color[0] = 1;
        color[1] = 1;
        color[2] = 0;
    }

    GLfloat g_base;
    GLfloat g_onbase;
    GLfloat g_back;
    GLfloat g_neck;
    GLfloat g_head;

    g_base = MakeOctagon(1, 0.5);
    g_onbase = MakeBox(1.5, 0.2, 1.5);
    g_back = MakeFrustumShape(1.1, 0.5, 2);
    g_neck = MakeOctagon(0.5, 0.5);
    g_head = MakeBox(0.1, 0.4, 0.2);

    GLuint dp_list;

    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    glPushMatrix();
    SetMaterialColor(color[0], color[1], color[2], 1);
    glTranslatef(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    glCallList(g_base);

    glRotatef(-90, 1, 0, 0);

    SetMaterialColor(0.6, 0.6, 0.6, 1);
    glTranslatef(-0.75, 0, 0.75);
    glRotatef(-90, 1, 0, 0);
    glCallList(g_onbase);

    glRotatef(90, 1, 0, 0);
    glTranslatef(0.75, 0, -0.75);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glTranslatef(0, 0.2, 0);
    glCallList(g_back);

    glTranslatef(0, -0.2, 0);

    SetMaterialColor(0.6, 0.6, 0.6, 1);
    glTranslatef(0, 1.8, 0);
    glRotatef(-90, 1, 0, 0);
    glCallList(g_neck);

    glRotatef(90, 1, 0, 0);
    glTranslatef(0, -1.8, 0);

    SetMaterialColor(color[0], color[1], color[2], 1);
    glTranslatef(0.45, 2.38, -0.2);
    glRotatef(-90, 0, 0, 1);
    glCallList(g_head);

    glRotatef(90, 0, 0, 1);
    glTranslatef(-0.45, -2.38, 0.2);

    glTranslatef(-0.45, 2.28, -0.2);
    glRotatef(90, 0, 0, 1);
    glCallList(g_head);

    glRotatef(-90, 0, 0, 1);
    glTranslatef(0.45, -2.28, 0.2);

    glTranslatef(-0.2, 2.28, 0.45);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glCallList(g_head);

    glRotatef(-90, 1, 0, 0);
    glRotatef(-90, 0, 0, 1);
    glTranslatef(0.2, -2.28, -0.45);

    glTranslatef(-0.2, 2.28, -0.65);
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glCallList(g_head);

    glRotatef(-90, 1, 0, 0);
    glRotatef(-90, 0, 0, 1);
    glTranslatef(0.2, -2.28, 0.65);

    glPopMatrix();
    glEndList();

    return dp_list;
}

// Board ----------------------------------------------------------------------

GLint Board_view(float width, float height) {
    GLuint dp_list;
    GLfloat square = MakeBox(width, width, height);
    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            glPushMatrix();
            if ((i + j) % 2 == 0) {
                SetMaterialColor(1, 1, 1, 1);  // White color
            } else {
                SetMaterialColor(0, 0, 0, 1);  // Black color
            }
            glTranslatef(i * width, 0, j * width);  // Translate cell
            glCallList(square);
            glPopMatrix();
        }
    }
    glEndList();
    return dp_list;
}

// Status cell -----------------------------------------

GLint StatusCell_view(string name_color) {
    float apha = 0.6;
    GLuint dp_list;
    GLfloat square = MakeBox(4, 4, 1);
    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    glPushMatrix();
    if (name_color == "red")
        SetMaterialColor(1, 0, 0, apha);  // Red color
    else if (name_color == "YELLOW")
        SetMaterialColor(1,1,0.5,apha);  //Yellow color
    else if (name_color == "BULE")
        SetMaterialColor(0.5,1,1,apha);
    else
        SetMaterialColor(0, 1, 0, apha);  // Green color
    glCallList(square);
    glPopMatrix();
    glEndList();
    return dp_list;
}

// Notification ----------------------------------------------------------------------

GLint Notification_view() {
    GLfloat g_table;
    GLfloat g_chain;

    GLuint dp_list;

    g_table = MakeBox(7, 1, 3);
    g_chain = MakeOctagon(0.2, 0.2);

    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    glPushMatrix();

    SetMaterialColor(0.2, 0.2, 0.2, 1);
    glCallList(g_table);

    SetMaterialColor(1, 1, 1, 1);
    glTranslatef(0.4, 3, 0.4);
    glCallList(g_chain);

    SetMaterialColor(0.7, 0.7, 0.7, 1);
    for (int i = 0; i < 20; i++) {
        if (i & 1) {
            glTranslatef(0, 0.3, 0.1);
            glRotatef(90, 0, 1, 0);
            glCallList(g_chain);
        } else {
            glTranslatef(0.1, 0.3, 0);
            glRotatef(-90, 0, 1, 0);
            glCallList(g_chain);
        }
    }

    SetMaterialColor(1, 1, 1, 1);
    glTranslatef(-0.4, -3, -0.4);
    glTranslatef(6.4, -3, 0.4);
    glCallList(g_chain);

    SetMaterialColor(0.7, 0.7, 0.7, 1);
    for (int i = 0; i < 20; i++) {
        if (i & 1) {
            glTranslatef(0, 0.3, 0.1);
            glRotatef(90, 0, 1, 0);
            glCallList(g_chain);
        } else {
            glTranslatef(0.1, 0.3, 0);
            glRotatef(-90, 0, 1, 0);
            glCallList(g_chain);
        }
    }

    glPopMatrix();
    glEndList();
    return dp_list;
}

GLuint loadBMP_custom(const char* imagepath) {
    // Data read from the header of the BMP file
    // Each BMP file begins by a 54-bytes header
    unsigned char header[54];
    // Position in the file where the actual data begins
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize; // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
    printf("filename: %s\n",imagepath);
    FILE * file = fopen(imagepath,"rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 )
    { 
        // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Create a buffer
    data = new unsigned char [imageSize];
     
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
     
    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
     
    // "Bind" the newly created texture:
    // all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
     
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, \
        GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureID;
}

GLint Test(){
    GLuint dp_list;
    GLfloat box = MakeCube(3);
    dp_list = glGenLists(1);
    glNewList(dp_list, GL_COMPILE);
    glPushMatrix();
    loadBMP_custom("./shinichi.bmp");
    glCallList(box);
    glPopMatrix();
    glEndList();
    return dp_list;
}
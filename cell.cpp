#include "cell.h"
#include <QtOpenGL/QGLWidget>


Cell::Cell(int x, int y, GLuint texture):PozitionX(x), PozitionY(y), texture(texture)
{

}

Cell::~Cell()
{

}

void Cell::Show()
{   
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_POLYGON);
        glTexCoord2i(1, 0);
        glVertex2i(PozitionX, PozitionY);


        glTexCoord2i(1, 1);
        glVertex2i(PozitionX, PozitionY + 127);


        glTexCoord2i(0, 1);
        glVertex2i( PozitionX + 127 / 1.397, PozitionY + 127);


        glTexCoord2i(0, 0);
        glVertex2i(PozitionX + 127 / 1.397, PozitionY);
    glEnd();

}


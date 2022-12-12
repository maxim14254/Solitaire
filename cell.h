#ifndef CELL_H
#define CELL_H
#include <GL/gl.h>


class Cell
{
public:
    Cell(int x, int y, GLuint texture);
    ~Cell();
    void Show();
    int PozitionX;
    int PozitionY;
    GLuint texture;
};

#endif // CELL_H

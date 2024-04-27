#ifndef CELL_H
#define CELL_H
#include <windows.h>
#include <GL/gl.h>


class Cell
{
public:
    Cell(int x, int y, GLuint texture);
    ~Cell();
    void Show();
    int PozitionX;
    int PozitionY;
    int DeltaBetweenCards = 20;
    GLuint texture;
};

#endif // CELL_H

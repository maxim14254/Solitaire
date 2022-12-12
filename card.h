#ifndef CARD_H
#define CARD_H
#include <GL/gl.h>


class Card
{
private:
    GLuint textureFace;
    GLuint textureBack;
public:
    Card(int x, int y, GLuint&& textureFace, GLuint textureBack);
    ~Card();
    void Show();
    void Show(GLuint texture);
    int PozitionX;
    int PozitionY;
    int OldPozitionX;
    int OldPozitionY;
    int Id;
    int Colum = -1;
    int PozitionZ = -7;
    int size = 127;
    bool faceShow = false;
};

#endif // CARD_H

#ifndef CARD_H
#define CARD_H
#include <windows.h>
#include <GL/gl.h>

enum CardSuit
{
    Buby = 0,
    Chervi = 1,
    Kresti = 2,
    Piki = 3
};

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
    CardSuit Suit;
    int Value;
    int PozitionX;
    int PozitionY;
    int PozitionZ = -7;
    int OldPozitionX;
    int OldPozitionY;
    int OldPozitionZ;
    int Id;
    int Colum = -1;
    int size = 127;
    int SpeedAnim = 0;
    bool faceShow = false;
    bool isRed;
};

#endif // CARD_H

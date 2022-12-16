#include "card.h"
#include <GL/gl.h>

Card::Card(int x, int y, GLuint&& textureFace, GLuint textureBack) : PozitionX(x), PozitionY(y), textureFace(textureFace), textureBack(textureBack)
{
    OldPozitionX = x;
    OldPozitionY = y;

    int suit = (textureFace - 3) / 13;

    this->Suit = CardSuit(suit);

    this->Value = textureFace - 3 - (suit * 13);

    if(this->Suit == CardSuit::Buby || this->Suit == CardSuit::Chervi)
        this->isRed = true;
    else
        this->isRed = false;
}

Card :: ~Card()
{

}

void  Card :: Show()
{  
    GLuint texture;

    if(faceShow)
        texture = textureFace;
    else
        texture = textureBack;

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex3i(PozitionX, PozitionY, PozitionZ);

        glTexCoord2i(0, 0);
        glVertex3i(PozitionX, PozitionY + size, PozitionZ);

        glTexCoord2i(1, 0);
        glVertex3i( PozitionX + size / 1.397, PozitionY + size, PozitionZ);

        glTexCoord2i(1, 1);
        glVertex3i(PozitionX + size / 1.397, PozitionY, PozitionZ);
    glEnd();
}


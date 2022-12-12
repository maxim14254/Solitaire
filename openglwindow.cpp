#include "card.h"
#include "cell.h"
#include "openglwindow.h"
#include <cmath>
#include <list>

Cell* cells;
Card* cards;
std::vector<std::list<Card*>> cardCombinartions(7);
std::vector<Card*> openCards(12);
std::allocator<Cell> AllocCells;
std::allocator<Card> AllocCard;
size_t vec[] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
double animTime = 0.0;
Card* clickCard = nullptr;
Card* returnCardBack = nullptr;
int deltaX = 0;
int deltaY = 0;
int speedAnim = 0;
int oldZ = 0;
bool gameNew = true;
bool Anim = false;
bool flag = false;

OpenGLWindow::OpenGLWindow(QWidget *parent) : QOpenGLWidget(parent)
{
    cells = std::allocator_traits<std::allocator<Cell>>::allocate(AllocCells, 13);
    cards = std::allocator_traits<std::allocator<Card>>::allocate(AllocCard, 52);
}

OpenGLWindow :: ~OpenGLWindow()
{
    std::allocator_traits<std::allocator<Cell>>::deallocate(AllocCells, cells, 13);
    std::allocator_traits<std::allocator<Card>>::deallocate(AllocCard, cards, 52);
}

void getTextures(const char* path, GLuint& texture)
{
    QImage currentTexture;
    currentTexture.load(path);

    currentTexture = QGLWidget::convertToGLFormat(currentTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)currentTexture.width(), (GLsizei)currentTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, currentTexture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

void OpenGLWindow :: initializeGL()
{
    typedef std::allocator_traits<std::allocator<Cell>> Alloc;
    typedef std::allocator_traits<std::allocator<Card>> Alloc2;

    initializeOpenGLFunctions();
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glOrtho(0, 800, 579, 0, -8, 8);

    glEnable (GL_ALPHA_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE);
    glAlphaFunc(GL_GREATER, 0.3);

    glEnable(GL_TEXTURE_2D);
    GLuint textures[54];
    glGenTextures(54, textures);
    getTextures(":/Textures/cell.png", textures[0]);
    getTextures(":/Textures/back.png", textures[1]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    int a = 0;
    int i = 0;
    bool flag = true;
    for(; i < 6; ++i)
    {
        if(flag && i == 2)
        {
            a = a + (23 + 88);
            flag = false;
            i = 1;
            continue;
        }
        Alloc::construct(AllocCells, &cells[i], a + 23, 15, textures[0]);
        a = a + (23 + 88);
    }

    a = 0;
    for(; i < 13; ++i)
    {
        Alloc::construct(AllocCells, &cells[i], a + 23, 170, textures[0]);
        a = a + (23 + 88);
    }

    i = 0;
    std::string path;
    for(; i < 52; ++i)
    {
        if(i < 13)
        {
            path = ":/Textures/bu/";
            path += std::to_string(i + 1)+".png";
            getTextures(path.c_str(), textures[i + 2]);
            Alloc2::construct(AllocCard, &cards[i], 23, 15, std::move(textures[i + 2]), textures[1]);
        }
        else if(i >= 13 && i < 26)
        {
            path = ":/Textures/ch/";
            path += std::to_string(i - 12)+".png";
            getTextures(path.c_str(), textures[i + 2]);
            Alloc2::construct(AllocCard, &cards[i], 23, 15, std::move(textures[i + 2]), textures[1]);
        }
        else if(i >= 26 && i < 39)
        {
            path = ":/Textures/kr/";
            path += std::to_string(i - 25)+".png";
            getTextures(path.c_str(), textures[i + 2]);
            Alloc2::construct(AllocCard, &cards[i], 23, 15, std::move(textures[i + 2]), textures[1]);
        }
        else if(i >= 39 && i < 52)
        {
            path = ":/Textures/pi/";
            path += std::to_string(i - 38)+".png";
            getTextures(path.c_str(), textures[i + 2]);
            Alloc2::construct(AllocCard, &cards[i], 23, 15, std::move(textures[i + 2]), textures[1]);
        }
    }
}

void OpenGLWindow :: newGame()
{
    gameNew = true;
    Anim = false;
    for(int i = 0; i < 52; ++i)
    {
       cards[i].PozitionX = 23;
       cards[i].PozitionY = 15;
       cards[i].faceShow = false;
       cards[i].PozitionZ = -7;
    }
    paintGL();
    update();
}

void OpenGLWindow :: resizeGL(int w, int h)
{
   // glViewport(0, 0, w, h);
}

bool IsCardOnCard(int startX, int startY, int finishX, int finishY, size_t size)
{
    int kx = startX- finishX;
    int ky = startY - finishY;

    int width = size / 1.397 - abs(kx);

    int height = size - abs(ky);

    if(width > 0 && height > 0)
    {
        if(width * height > 0.1 * size * size / 1.397 )
        {
            return true;
        }
    }
    return false;
}

void OpenGLWindow :: paintGL()
{
    auto begin = std::chrono::steady_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //qDebug() << "paintGL";
    glClearColor(0.17, 0.45, 0.21, 1);

    glPushMatrix();
    glTranslated(0, 0, -8);
    for(size_t i = 0; i < 13; ++i)
    {
        cells[i].Show();
    }
    glPopMatrix();

    if(gameNew && !Anim)
    {
        srand(time(NULL));
        for (int i = 51; i >= 1; i--)
        {
            int j = rand() % (i + 1);
            std::swap(vec[i], vec[j]);
        }
    }

    for(size_t i = 0; i < 52; ++i)
    {
        cards[vec[i]].Id = i;
        cards[vec[i]].Show();
    }

    if(gameNew)
    {
        Anim = true;
        static int i = 0;
        static int j = 0;
        static int x = 0;

        float deltaX = cells[12 - i].PozitionX - cards[vec[j]].PozitionX;
        float deltaY = cells[12 - i].PozitionY + 15 * x - cards[vec[j]].PozitionY;

        if(animTime > 0 && (speedAnim == 0 || flag))
        {
            speedAnim = deltaX * 0.2;
            flag = false;

        }
        float tg = deltaY / deltaX;

        if(cards[vec[j]].PozitionX + speedAnim < cells[12 - i].PozitionX &&
           cards[vec[j]].PozitionX < cells[12 - i].PozitionX)
        {
            cards[vec[j]].PozitionX += speedAnim;
            cards[vec[j]].PozitionY += speedAnim * tg;

            if(x == 6 - i && !cards[vec[j]].faceShow)
            {
                cards[vec[j]].faceShow = true;
                cards[vec[j]].PozitionZ = -6;
                cards[vec[j]].Colum = 6 - i;
                cardCombinartions[6 - i].push_back(&cards[vec[j]]);
                openCards[6 - i] = &cards[vec[j]];

            }
        }
        else
        {
           cards[vec[j]].PozitionX = cells[12 - i].PozitionX;
           cards[vec[j]].PozitionY = cells[12 - i].PozitionY + 15 * x;

           cards[vec[j]].OldPozitionX = cards[vec[j]].PozitionX;
           cards[vec[j]].OldPozitionY = cards[vec[j]].PozitionY;

           if(x == 6 - i && !cards[vec[j]].faceShow)
           {
               cards[vec[j]].faceShow = true;
               cards[vec[j]].PozitionZ = -6;
               cards[vec[j]].Colum = 6 - i;
               openCards[6 - i] = &cards[vec[j]];
               cardCombinartions[6 - i].push_back(&cards[vec[j]]);
           }
            ++j;
            ++x;
            if(x == 7 - i)
            {
                ++i;
                x = 0;
                flag = true;
            }
        }

        if(i > 6)
        {
            gameNew = false;
            flag = true;
            i = 0;
            j = 0;
            x = 0;
            speedAnim = 0;
        }
        update();
    }

    if(returnCardBack != nullptr)
    {
        float deltaX = returnCardBack->OldPozitionX - returnCardBack->PozitionX;
        float deltaY = returnCardBack->OldPozitionY - returnCardBack->PozitionY;
        int x = 0;
        int y = 0;
        float tg;
        if(animTime > 0 && speedAnim == 0 )
        {
            speedAnim = deltaX * 0.08;

            if(abs(deltaY) > abs(deltaX))
            {
                speedAnim = deltaY * 0.08;
            }
        }

        if(abs(deltaY) < abs(deltaX))
        {
            tg = deltaY / deltaX;
            x = speedAnim;
            y = speedAnim * tg;
        }
        else
        {
             tg = deltaX / deltaY;
             x = speedAnim * tg;
             y = speedAnim;
        }

        if(!IsCardOnCard(returnCardBack->PozitionX, returnCardBack->PozitionY, returnCardBack->OldPozitionX, returnCardBack->OldPozitionY, returnCardBack->size))
        {
            returnCardBack->PozitionX += x;
            returnCardBack->PozitionY += y;
        }
        else
        {
            returnCardBack->PozitionX = returnCardBack->OldPozitionX;
            returnCardBack->PozitionY = returnCardBack->OldPozitionY;
            returnCardBack->PozitionZ = oldZ;
            returnCardBack = nullptr;
            speedAnim = 0;
            oldZ = 0;
        }
        update();
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto end = std::chrono::steady_clock::now();
    //animTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    animTime = std::chrono::duration<double>(end - begin).count();
}

void OpenGLWindow :: mouseMoveEvent(QMouseEvent* event)
{
    auto xy = event->pos();

    if(clickCard != nullptr)
    {
        clickCard->PozitionX = xy.x() - deltaX;
        clickCard->PozitionY = xy.y() - deltaY;
        //qDebug() << clickCard->PozitionY;
        update();
    }
}

void OpenGLWindow :: mousePressEvent(QMouseEvent* event)
{
    auto xy = event->pos();
    qDebug() <<"x "<< xy.x();
    qDebug() <<"y " <<xy.y();

    for(int i = 0; i < 7; ++i)
    {
        if(cardCombinartions[i].back()!= nullptr && xy.x() > cardCombinartions[i].back()->PozitionX && xy.x() < cardCombinartions[i].back()->PozitionX + cardCombinartions[i].back()->size / 1.397 &&
           xy.y() > cardCombinartions[i].back()->PozitionY && xy.y() < cardCombinartions[i].back()->PozitionY + cardCombinartions[i].back()->size)
        {
            clickCard = cardCombinartions[i].back();
            oldZ = clickCard->PozitionZ;
            clickCard->PozitionZ = 8;
           // qDebug() << &cards[i];
            deltaX = xy.x()- clickCard->PozitionX;
            deltaY = xy.y()- clickCard->PozitionY;
            clickCard->PozitionX -= 5;
            clickCard->PozitionY -= 5;

            clickCard->size += 10;
            update();
            break;
        }
    }
}

void OpenGLWindow :: mouseReleaseEvent(QMouseEvent *event)
{
    if(clickCard != nullptr)
    {
        clickCard->PozitionX += 5;
        clickCard->PozitionY += 5;
        clickCard->size -= 10;
        deltaX = 0;
        deltaY = 0;
        bool flag = true;

        for(size_t i = 0; i < 7; ++i)
        {
            if(cardCombinartions[i].back()!= nullptr && cardCombinartions[i].back() != clickCard && IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cardCombinartions[i].back()->PozitionX, cardCombinartions[i].back()->PozitionY, clickCard->size))
            {
                clickCard->PozitionX = cardCombinartions[i].back()->PozitionX;
                clickCard->PozitionY = cardCombinartions[i].back()->PozitionY + 20;
                clickCard->PozitionZ = oldZ;

                clickCard->OldPozitionX = clickCard->PozitionX;
                clickCard->OldPozitionY = clickCard->PozitionY;

                if(!cards[vec[clickCard->Id - 1]].faceShow)
                {
                    cards[vec[clickCard->Id - 1]].faceShow = true;
                    cards[vec[clickCard->Id - 1]].Colum = clickCard->Colum;
                    cardCombinartions[clickCard->Colum].back() = &cards[vec[clickCard->Id - 1]];
                    cardCombinartions[clickCard->Colum].back()->PozitionZ = -6;
                }
                else
                {
                    cardCombinartions[clickCard->Colum].pop_back();
                }

                int z = cardCombinartions[i].back()->PozitionZ;
                cardCombinartions[i].push_back(clickCard);
                clickCard->PozitionZ = z + 1;
                clickCard->Colum = i;

                flag = false;
                break;
            }
            //int width = (kx / abs(kx)) * (card->PozitionX + card->size) - clickCard->PozitionX;
            //abs(kx);
            //IsCardOnCard(clickCard->PozitionX, card->PozitionX);
        }

        if(flag)
        {
            returnCardBack = clickCard;
            //returnCardBack->PozitionZ = 8;
        }
    }
    clickCard = nullptr;
    update();
}


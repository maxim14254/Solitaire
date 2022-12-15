#include "card.h"
#include "cell.h"
#include "openglwindow.h"
#include <cmath>
#include <list>

Card* cards;
Cell* cells;
std::vector<std::list<Card*>> cardCombinations(8);
std::vector<std::list<Card*>> cardCombinationsForVictory(4);
std::vector<Card*> cardsInColode;
std::allocator<Cell> AllocCells;
std::allocator<Card> AllocCard;
size_t vec[] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
double animTime = 0.0;
Card* clickCard = nullptr;
Card* returnCardBack = nullptr;
Card* newOpenCard = nullptr;
int deltaX = 0;
int deltaY = 0;
int speedAnim = 0;
int oldZ = 0;
size_t sizeOfColode = 0;
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
    sizeOfColode = 0;
    cardsInColode.clear();
    for(int i = 0; i < 52; ++i)
    {
       cards[i].PozitionX = 23;
       cards[i].PozitionY = 15;
       cards[i].faceShow = false;
       cards[i].PozitionZ = -7;

       if(i < 8)
       {
           cardCombinations[i].clear();
       }

       if(i < 4)
       {
           cardCombinationsForVictory[i].clear();
       }
    }
    paintGL();
    update();
}

void OpenGLWindow :: resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
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

    if(cardCombinationsForVictory[0].size() == 13 &&
       cardCombinationsForVictory[1].size() == 13 &&
       cardCombinationsForVictory[2].size() == 13 &&
       cardCombinationsForVictory[3].size() == 13)
    {
        newGame();
    }

    if(gameNew)
    {
        Anim = true;
        static int i = 0;
        static int x = 0;

        float deltaX = cells[12 - i].PozitionX - cards[vec[sizeOfColode]].PozitionX;
        float deltaY = cells[12 - i].PozitionY + 15 * x - cards[vec[sizeOfColode]].PozitionY;

        if(animTime > 0 && (speedAnim == 0 || flag))
        {
            speedAnim = deltaX * 0.2;
            flag = false;

        }
        float tg = deltaY / deltaX;

        if(cards[vec[sizeOfColode]].PozitionX + speedAnim < cells[12 - i].PozitionX &&
           cards[vec[sizeOfColode]].PozitionX < cells[12 - i].PozitionX)
        {
            cards[vec[sizeOfColode]].PozitionX += speedAnim;
            cards[vec[sizeOfColode]].PozitionY += speedAnim * tg;

            if(x == 6 - i && !cards[vec[sizeOfColode]].faceShow)
            {
                cards[vec[sizeOfColode]].faceShow = true;
                cards[vec[sizeOfColode]].PozitionZ = -6;
                cards[vec[sizeOfColode]].Colum = 6 - i;
                cardCombinations[6 - i].push_back(&cards[vec[sizeOfColode]]);
            }
        }
        else
        {
           cards[vec[sizeOfColode]].PozitionX = cells[12 - i].PozitionX;
           cards[vec[sizeOfColode]].PozitionY = cells[12 - i].PozitionY + 15 * x;

           cards[vec[sizeOfColode]].OldPozitionX = cards[vec[sizeOfColode]].PozitionX;
           cards[vec[sizeOfColode]].OldPozitionY = cards[vec[sizeOfColode]].PozitionY;

           if(x == 6 - i && !cards[vec[sizeOfColode]].faceShow)
           {
               cards[vec[sizeOfColode]].faceShow = true;
               cards[vec[sizeOfColode]].PozitionZ = -6;
               cards[vec[sizeOfColode]].Colum = 6 - i;
               cardCombinations[6 - i].push_back(&cards[vec[sizeOfColode]]);
           }

           if(sizeOfColode < 24)
           {
               cardsInColode.push_back(&cards[vec[51 - sizeOfColode]]);
           }

            ++sizeOfColode;
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
            sizeOfColode = 0;
            gameNew = false;
            flag = true;
            i = 0;
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

    if(newOpenCard != nullptr)
    {
        float deltaX = cells[1].PozitionX - newOpenCard->PozitionX;

        if(animTime > 0 && speedAnim == 0 )
        {
            speedAnim = deltaX * 0.08;
            newOpenCard->faceShow = true;
            newOpenCard->PozitionZ = -6;
        }

        if(!IsCardOnCard(newOpenCard->PozitionX, newOpenCard->PozitionY, cells[1].PozitionX * 1.5, cells[1].PozitionY * 1.5, newOpenCard->size))
        {
            newOpenCard->PozitionX += speedAnim;
        }
        else
        {
            newOpenCard->PozitionX = cells[1].PozitionX;
            newOpenCard->PozitionY = cells[1].PozitionY;

            newOpenCard->OldPozitionX = cells[1].PozitionX;
            newOpenCard->OldPozitionY = cells[1].PozitionY;

            if(cardCombinations[7].back() == nullptr)
                cardCombinations[7].push_back(newOpenCard);
            else
                cardCombinations[7].back() = newOpenCard;

            newOpenCard = nullptr;
            speedAnim = 0;
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
    if(speedAnim != 0)
        return;

    auto xy = event->pos();

    if(clickCard != nullptr)
    {
        clickCard->PozitionX = xy.x() - deltaX;
        clickCard->PozitionY = xy.y() - deltaY;
        //qDebug() << clickCard->PozitionY;
        update();
    }
}

void SetClickCard(Card* card, int x, int y)
{
    clickCard = card;
    oldZ = clickCard->PozitionZ;
    clickCard->PozitionZ = 8;
   // qDebug() << &cards[i];
    deltaX = x- clickCard->PozitionX;
    deltaY = y- clickCard->PozitionY;
    clickCard->PozitionX -= 5;
    clickCard->PozitionY -= 5;

    clickCard->size += 10;
}

void OpenGLWindow :: mousePressEvent(QMouseEvent* event)
{
    if(speedAnim != 0)
        return;

    auto xy = event->pos();
    qDebug() <<"x "<< xy.x();
    qDebug() <<"y " <<xy.y();

    if(xy.x() > cells[0].PozitionX && xy.x() < cells[0].PozitionX + cards[0].size / 1.397 &&
       xy.y() > cells[0].PozitionY && xy.y() < cells[0].PozitionY + cards[0].size)
    {
        if(sizeOfColode == cardsInColode.size())
        {
            for(auto d : cardsInColode)
            {
                --sizeOfColode;
                d->PozitionZ = -7;
                d->faceShow = false;
                d->PozitionX = cells[0].PozitionX;
                d->PozitionY = cells[0].PozitionY;
            }
            //sizeOfColode = 28;
            return;
        }
        newOpenCard = cardsInColode[cardsInColode.size() - sizeOfColode - 1];
        ++sizeOfColode;
        return;
    }

    for(int i = 2; i < 6; ++i)
    {
        if(xy.x() > cells[i].PozitionX && xy.x() < cells[i].PozitionX + cards[0].size / 1.397 &&
           xy.y() > cells[i].PozitionY && xy.y() < cells[i].PozitionY + cards[0].size)
        {
            SetClickCard(cardCombinationsForVictory[i - 2].back(), xy.x(), xy.y());
            update();
            return;
        }
    }

    for(int i = 0; i < 8; ++i)
    {
        if(cardCombinations[i].back()!= nullptr && xy.x() > cardCombinations[i].back()->PozitionX && xy.x() < cardCombinations[i].back()->PozitionX + cardCombinations[i].back()->size / 1.397 &&
           xy.y() > cardCombinations[i].back()->PozitionY && xy.y() < cardCombinations[i].back()->PozitionY + cardCombinations[i].back()->size)
        {           
            SetClickCard(cardCombinations[i].back(), xy.x(), xy.y());
            update();
            return;
        }
    }
}

void AddCardInColode(int x, int y, int z)
{
    clickCard->PozitionX = x;
    clickCard->PozitionY = y + 20;
    clickCard->PozitionZ = oldZ;

    clickCard->OldPozitionX = clickCard->PozitionX;
    clickCard->OldPozitionY = clickCard->PozitionY;

    if(clickCard->Id != 0 && !cards[vec[clickCard->Id - 1]].faceShow)
    {
        cards[vec[clickCard->Id - 1]].faceShow = true;
        cards[vec[clickCard->Id - 1]].Colum = clickCard->Colum;
        cardCombinations[clickCard->Colum].back() = &cards[vec[clickCard->Id - 1]];
        cardCombinations[clickCard->Colum].back()->PozitionZ = -6;
    }
    else if(clickCard->Colum < -1)
    {
        cardCombinationsForVictory[abs(clickCard->Colum) - 2].pop_back();
    }
    else if(clickCard->Colum > -1)
    {
        cardCombinations[clickCard->Colum].pop_back();
    }
    else if(sizeOfColode <= cardsInColode.size())
    {
        auto it = cardsInColode.begin();
        std::advance(it, cardsInColode.size() - sizeOfColode);
        cardsInColode.erase(it);
        --sizeOfColode;

        cardCombinations[7].back() = cardsInColode[cardsInColode.size() - sizeOfColode];
    }
    clickCard->PozitionZ = z + 1;
}

void OpenGLWindow :: mouseReleaseEvent(QMouseEvent *event)
{
    if(speedAnim != 0)
        return;

    if(clickCard != nullptr)
    {
        clickCard->PozitionX += 5;
        clickCard->PozitionY += 5;
        clickCard->size -= 10;
        deltaX = 0;
        deltaY = 0;
        bool flag = true;

        for(int i = 2; i < 6; ++i)
        {
            if(IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cells[i].PozitionX, cells[i].PozitionY, clickCard->size))
            {
                if((cardCombinationsForVictory[i - 2].front() == nullptr && clickCard->Value == 0) ||
                  (cardCombinationsForVictory[i - 2].back() != nullptr && clickCard->Suit == cardCombinationsForVictory[i - 2].back()->Suit && clickCard->Value == cardCombinationsForVictory[i - 2].back()->Value + 1))
                {
                    clickCard->PozitionX =cells[i].PozitionX;
                    clickCard->PozitionY = cells[i].PozitionY;
                    clickCard->PozitionZ = -6;

                    clickCard->OldPozitionX = clickCard->PozitionX;
                    clickCard->OldPozitionY = clickCard->PozitionY;

                    if(cardCombinationsForVictory[i - 2].back() != nullptr)
                    {
                        cardCombinationsForVictory[i - 2].back()->PozitionZ = -7;
                    }

                    cardCombinationsForVictory[i - 2].push_back(clickCard);

                    if(!cards[vec[clickCard->Id - 1]].faceShow)
                    {
                        cards[vec[clickCard->Id - 1]].faceShow = true;
                        cards[vec[clickCard->Id - 1]].Colum = clickCard->Colum;
                        cardCombinations[clickCard->Colum].back() = &cards[vec[clickCard->Id - 1]];
                        cardCombinations[clickCard->Colum].back()->PozitionZ = -6;
                    }
                    else if(clickCard->Colum > -1)
                    {
                        cardCombinations[clickCard->Colum].pop_back();
                    }
                    else if(clickCard->Colum < -1)
                    {
                        cardCombinationsForVictory[abs(clickCard->Colum) - 2].pop_back();
                    }
                    else if(clickCard->Colum == -1)
                    {
                        auto it = cardsInColode.begin();
                        std::advance(it, cardsInColode.size() - sizeOfColode);
                        cardsInColode.erase(it);
                        --sizeOfColode;

                        cardCombinations[7].back() = cardsInColode[cardsInColode.size() - sizeOfColode];
                    }
                    clickCard->Colum = -i;
                    clickCard = nullptr;
                    update();
                    return;
                }
                else
                {
                    returnCardBack = clickCard;
                    clickCard = nullptr;
                    update();
                    return;
                }
            }
        }

        for(size_t i = 0; i < 7; ++i)
        {
            if(cardCombinations[i].back()!= nullptr && cardCombinations[i].back() != clickCard &&
              IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, clickCard->size))
            {
                AddCardInColode(cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, cardCombinations[i].back()->PozitionZ);
                clickCard->Colum = i;
                cardCombinations[i].push_back(clickCard);

                flag = false;
                break;
            }
            else if((cardCombinations[i].back() == nullptr && clickCard->Value == 12) &&
                    IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cells[i + 6].PozitionX, cells[i + 6].PozitionY - 20, clickCard->size))
            {
                AddCardInColode(cells[i + 6].PozitionX, cells[i + 6].PozitionY - 20, -7);
                clickCard->Colum = i;
                cardCombinations[i].push_back(clickCard);

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


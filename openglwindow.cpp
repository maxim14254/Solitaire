#include "card.h"
#include "cell.h"
#include "openglwindow.h"
#include <cmath>
#include <list>

Card* cards;
Cell* cells;
std::vector<std::vector<Card*>> cardCombinations(8);
std::vector<std::list<Card*>> cardCombinationsForVictory(4);
std::vector<Card*> cardsInColode;
std::vector<int> numbersOpenCardInCardCombinations(8);
std::vector<Card*> returnCardBack;
std::allocator<Cell> AllocCells;
std::allocator<Card> AllocCard;
size_t vec[] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
double animTime = 0.0;
Card* clickCard = nullptr;
Card* newOpenCard = nullptr;
int deltaX = 0;
int deltaY = 0;
int speedAnim = 0;
//int oldZ = 0;
int columForMove = -1;
size_t indexForMove = -1;
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
    glOrtho(0, 800, 578, 0, -30, 30);

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

    columForMove = -1;
    indexForMove = -1;
    //numbersOpenCardInCardCombinations.clear();
    for(int i = 0; i < 52; ++i)
    {
       cards[i].PozitionX = 23;
       cards[i].PozitionY = 15;
       cards[i].faceShow = false;
       cards[i].PozitionZ = -7;
       cards[i].Colum = -1;

       if(i < 8)
       {
           cardCombinations[i].clear();
       }

       if(i < 4)
       {
           cardCombinationsForVictory[i].clear();
       }
       if(i < 13)
       {
           cells[i].DeltaBetweenCards = 20;
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
    //qDebug() << clickCard->Id;
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
            cards[vec[sizeOfColode]].PozitionZ = -6;
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
               //cards[vec[sizeOfColode]].PozitionZ = -6;
               cards[vec[sizeOfColode]].Colum = 6 - i;
               numbersOpenCardInCardCombinations[6 - i] = 6 - i;
           }
           else
               cards[vec[sizeOfColode]].PozitionZ = -7;

           cards[vec[sizeOfColode]].Colum = 6 - i;
           cardCombinations[6 - i].push_back(&cards[vec[sizeOfColode]]);
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

    if(returnCardBack.size() > 0)
    {
        for(int i = returnCardBack.size() - 1; i >= 0; --i)
        {
            float deltaX = returnCardBack[i]->OldPozitionX - returnCardBack[i]->PozitionX;
            float deltaY = returnCardBack[i]->OldPozitionY - returnCardBack[i]->PozitionY;
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
            else if(abs(deltaY) == abs(deltaX))
            {
                tg = deltaX / deltaY;
                x = -abs(speedAnim) * tg;
                y = -abs(speedAnim);
            }
            else
            {
                tg = deltaX / deltaY;
                x = speedAnim * tg;
                y = speedAnim;
            }
            qDebug()<<deltaX;
            qDebug()<<deltaY;
            if(!IsCardOnCard(returnCardBack[i]->PozitionX, returnCardBack[i]->PozitionY, returnCardBack[i]->OldPozitionX, returnCardBack[i]->OldPozitionY, returnCardBack[i]->size))
            {
                returnCardBack[i]->PozitionX += x;
                returnCardBack[i]->PozitionY += y;
            }
            else
            {
                returnCardBack[i]->PozitionX = returnCardBack[i]->OldPozitionX;
                returnCardBack[i]->PozitionY = returnCardBack[i]->OldPozitionY;
                returnCardBack[i]->PozitionZ = returnCardBack[i]->OldPozitionZ;
                returnCardBack.pop_back();

                if(returnCardBack.size() == 0)
                {
                    speedAnim = 0;
                    //oldZ = 0;
                }
            }
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

//            if(cardCombinations[7].size() == 0)
//                cardCombinations[7].push_back(newOpenCard);
//            else
//                cardCombinations[7].back() = newOpenCard;
            cardCombinations[7].push_back(newOpenCard);
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

    if(columForMove == -1 && clickCard != nullptr)
    {
        clickCard->PozitionX = xy.x() - deltaX;
        clickCard->PozitionY = xy.y() - deltaY;
        //qDebug() << clickCard->PozitionY;
        update();
    }
    else if(columForMove != -1)
    {
        for(size_t i = 0; i < cardCombinations[columForMove].size() - indexForMove ; ++i)
        {
            cardCombinations[columForMove][indexForMove + i]->PozitionX = xy.x() - deltaX - 5;
            cardCombinations[columForMove][indexForMove + i]->PozitionY = xy.y() - deltaY + (20 * i) - 5;
        }
        update();
    }
}

void SetClickCard(Card* card, int x, int y)
{
    clickCard = card;
    clickCard->OldPozitionZ = clickCard->PozitionZ;
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
                cardCombinations[7].pop_back();
            }
            //sizeOfColode = 28;
            return;
        }
        newOpenCard = cardsInColode[cardsInColode.size() - sizeOfColode - 1];
        ++sizeOfColode;
        return;
    }

    if(xy.x() > cells[1].PozitionX && xy.x() < cells[1].PozitionX + cards[0].size / 1.397 &&
       xy.y() > cells[1].PozitionY && xy.y() < cells[1].PozitionY + cards[0].size)
    {
        SetClickCard(cardCombinations[7].back(), xy.x(), xy.y());
        update();
        return;
    }

    for(int i = 2; i < 6 && xy.x() > cells[2].PozitionX && xy.x() < cells[5].PozitionX + cards[0].size / 1.397 &&
         xy.y() > cells[2].PozitionY && xy.y() < cells[5].PozitionY + cards[0].size; ++i)
    {
        if(cardCombinationsForVictory[i - 2].back() != nullptr && xy.x() > cells[i].PozitionX && xy.x() < cells[i].PozitionX + cards[0].size / 1.397 &&
           xy.y() > cells[i].PozitionY && xy.y() < cells[i].PozitionY + cards[0].size)
        {
            SetClickCard(cardCombinationsForVictory[i - 2].back(), xy.x(), xy.y());
            update();
            return;
        }
    }

    for(int i = 0; i < 7 && xy.x() > cells[6].PozitionX && xy.x() < cells[12].PozitionX + cards[0].size / 1.397 &&
        xy.y() > cells[6].PozitionY; ++i)
    {
        if(cardCombinations[i].size() > 0 && xy.x() > cardCombinations[i].back()->PozitionX && xy.x() < cardCombinations[i].back()->PozitionX + cardCombinations[i].back()->size / 1.397 &&
           xy.y() > cardCombinations[i][numbersOpenCardInCardCombinations[i]]->PozitionY && xy.y() < cardCombinations[i].back()->PozitionY + cardCombinations[i].back()->size)
        {
            if(xy.y() > cardCombinations[i].back()->PozitionY)
            {
                SetClickCard(cardCombinations[i].back(), xy.x(), xy.y());
            }
            else
            {
                int delta = xy.y() - cardCombinations[i][numbersOpenCardInCardCombinations[i]]->PozitionY;
                int count = delta / cells[6 + i].DeltaBetweenCards;

                indexForMove = numbersOpenCardInCardCombinations[i] + count;
                columForMove = i;

                deltaX = xy.x() - cardCombinations[i].front()->PozitionX;
                deltaY = xy.y() - cardCombinations[i][numbersOpenCardInCardCombinations[i] + count]->PozitionY;

                clickCard = cardCombinations[i][indexForMove];

                for(size_t i = 0; i < cardCombinations[columForMove].size() - indexForMove ; ++i)
                {
                    cardCombinations[columForMove][indexForMove + i]->OldPozitionZ =  cardCombinations[columForMove][indexForMove + i]->PozitionZ;
                    cardCombinations[columForMove][indexForMove + i]->PozitionZ += 8;
                    cardCombinations[columForMove][indexForMove + i]->size = 137;
                }
            }

            update();
            return;
        }
    }
}

void AddCardInCombination(int x, int y, int z, int deltaY, int index, int numbColum)
{
    int oldColum = clickCard->Colum;
    for(size_t i = index + 1; oldColum >= 0 && i < cardCombinations[oldColum].size(); ++i)
    {
        cardCombinations[oldColum][i]->size -= 10;
        cardCombinations[oldColum][i]->PozitionX = x;
        cardCombinations[oldColum][i]->PozitionY = y + (deltaY * (i - index));
        cardCombinations[oldColum][i]->PozitionZ = z + (i - index);

        cardCombinations[oldColum][i]->Colum = numbColum;
        cardCombinations[oldColum][i]->OldPozitionX = cardCombinations[oldColum][i]->PozitionX;
        cardCombinations[oldColum][i]->OldPozitionY = cardCombinations[oldColum][i]->PozitionY;
        cardCombinations[oldColum][i]->OldPozitionZ = cardCombinations[oldColum][i]->PozitionZ;
    }

    if(oldColum > -1 && index >= 0 && cardCombinations[oldColum].size() > 1 && !cardCombinations[oldColum][index]->faceShow)
    {
        cardCombinations[oldColum][numbersOpenCardInCardCombinations[oldColum] - 1]->faceShow = true;
        cardCombinations[oldColum][numbersOpenCardInCardCombinations[oldColum] - 1]->PozitionZ = -6;
        cardCombinations[oldColum].erase(cardCombinations[oldColum].begin() + index + 1, cardCombinations[oldColum].end());
        numbersOpenCardInCardCombinations[oldColum] = numbersOpenCardInCardCombinations[oldColum] - 1;
    }
    else if(oldColum < -1)
    {
        cardCombinationsForVictory[abs(oldColum) - 2].pop_back();
        clickCard->PozitionX = x;
        clickCard->PozitionY = y + deltaY;
        clickCard->PozitionZ = z + 1;
        clickCard->OldPozitionY = clickCard->PozitionY;
        clickCard->OldPozitionZ = clickCard->PozitionZ;
        clickCard->size -= 10;
        clickCard->Colum = numbColum;
    }
    else if(oldColum > -1)
    {
        cardCombinations[oldColum].erase(cardCombinations[oldColum].begin() + index + 1, cardCombinations[oldColum].end());

        if(cardCombinations[oldColum].size() == 0)
        {
            numbersOpenCardInCardCombinations[oldColum] = - 1;
        }
    }
    else if(sizeOfColode <= cardsInColode.size())
    {
        auto it = cardsInColode.begin();
        std::advance(it, cardsInColode.size() - sizeOfColode);
        cardsInColode.erase(it);
        --sizeOfColode;

        clickCard->PozitionX = x;
        clickCard->PozitionY = y + deltaY;
        clickCard->PozitionZ = z + 1;
        clickCard->OldPozitionY = clickCard->PozitionY;
        clickCard->OldPozitionZ = clickCard->PozitionZ;
        clickCard->size -= 10;
        clickCard->Colum = numbColum;

        //cardCombinations[7].back() = cardsInColode[cardsInColode.size() - sizeOfColode];
        cardCombinations[7].pop_back();
    }
   //clickCard->PozitionZ = z + 1;
}

void SetDeltaBetweenCards(int i)
{
    if(cardCombinations[i].back()->PozitionY + cardCombinations[i].back()->size + cells[6 + i].DeltaBetweenCards > 578)
    {
        cells[6 + i].DeltaBetweenCards = (578 - cardCombinations[i][numbersOpenCardInCardCombinations[i]]->PozitionY - cardCombinations[i].back()->size) / (cardCombinations[i].size() - numbersOpenCardInCardCombinations[i]);

        for(size_t j = numbersOpenCardInCardCombinations[i]; j < cardCombinations[i].size(); ++j)
        {
            if(j == 0) j = 1;

            cardCombinations[i][j]->PozitionY = cardCombinations[i][j - 1]->PozitionY + cells[6 + i].DeltaBetweenCards;
            cardCombinations[i][j]->OldPozitionY = cardCombinations[i][j - 1]->OldPozitionY + cells[6 + i].DeltaBetweenCards;
        }
    }
}

void SetDeltaBetweenCards(int i, bool)
{
    int k = 1;
    if(cardCombinations[i].size() == numbersOpenCardInCardCombinations[i] + 1)
    {
        k = 0;
    }

    if(cells[6 + i].DeltaBetweenCards < 20)
    {
        cells[6 + i].DeltaBetweenCards = (578 - cardCombinations[i][numbersOpenCardInCardCombinations[i]]->PozitionY - cardCombinations[i].back()->size) / (cardCombinations[i].size() - k - numbersOpenCardInCardCombinations[i]);

        if(cells[6 + i].DeltaBetweenCards > 20)
            cells[6 + i].DeltaBetweenCards = 20;

        for(size_t j = numbersOpenCardInCardCombinations[i]; j < cardCombinations[i].size(); ++j)
        {
            if(j == 0) j = 1;

            cardCombinations[i][j]->PozitionY = cardCombinations[i][j - 1]->PozitionY + cells[6 + i].DeltaBetweenCards;
            cardCombinations[i][j]->OldPozitionY = cardCombinations[i][j - 1]->OldPozitionY + cells[6 + i].DeltaBetweenCards;
        }
    }
}

void OpenGLWindow :: mouseReleaseEvent(QMouseEvent *event)
{
    if(speedAnim != 0)
        return;

    if(clickCard != nullptr)
    {
        deltaX = 0;
        deltaY = 0;
        bool flag = true;

        for(int i = 2; i < 6 && columForMove == -1 && clickCard->PozitionX < cells[5].PozitionX + clickCard->size / 1.397 && clickCard->PozitionX + clickCard->size / 1.397 >  cells[2].PozitionX &&
            clickCard->PozitionY < cells[6].PozitionY; ++i)
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

                    if(clickCard->Colum > -1 && cardCombinations[clickCard->Colum].size() > 1 && !cardCombinations[clickCard->Colum][cardCombinations[clickCard->Colum].size() - 2]->faceShow)
                    {
                        cardCombinations[clickCard->Colum][cardCombinations[clickCard->Colum].size() - 2]->faceShow = true;
                        cardCombinations[clickCard->Colum][cardCombinations[clickCard->Colum].size() - 2]->PozitionZ = -6;
                        cardCombinations[clickCard->Colum].pop_back();
                        numbersOpenCardInCardCombinations[clickCard->Colum] = cardCombinations[clickCard->Colum].size() - 1;
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

                        //cardCombinations[7].back() = cardsInColode[cardsInColode.size() - sizeOfColode];
                        cardCombinations[7].pop_back();
                    }

                    clickCard->size -= 10;
                    SetDeltaBetweenCards(clickCard->Colum, true);
                    clickCard->Colum = -i;
                    clickCard = nullptr;

                    update();
                    return;
                }
                else
                {
                    clickCard->PozitionX += 5;
                    clickCard->PozitionY += 5;
                    clickCard->size -= 10;

                    returnCardBack.push_back(clickCard);
                    clickCard = nullptr;
                    update();
                    return;
                }
            }
        }

        for(size_t i = 0; i < 7 && clickCard->PozitionY > cells[6].PozitionY; ++i)
        {
            //Если выделенную карту взяли из колоды
            if((cardCombinations[i].size() > 0 && clickCard->Colum == -1) && clickCard->Colum != i &&
                 IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY - cells[6 + i].DeltaBetweenCards, clickCard->size) &&
                 cardCombinations[i].back()->isRed == !clickCard->isRed && cardCombinations[i].back()->Value == clickCard->Value + 1)
            {
                SetDeltaBetweenCards(i);

                clickCard->size -= 10;
                clickCard->OldPozitionX = clickCard->PozitionX = cardCombinations[i].back()->PozitionX;
                clickCard->OldPozitionY = clickCard->PozitionY = cardCombinations[i].back()->PozitionY + cells[6 + i].DeltaBetweenCards;
                clickCard->OldPozitionZ = clickCard->PozitionZ = cardCombinations[i].back()->PozitionZ + 1;

                auto it = cardsInColode.begin();
                std::advance(it, cardsInColode.size() - sizeOfColode);
                cardsInColode.erase(it);
                --sizeOfColode;

                //cardCombinations[7].back() = cardsInColode[cardsInColode.size() - sizeOfColode];
                cardCombinations[7].pop_back();
                //AddCardInColode(cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, cardCombinations[i].back()->PozitionZ, cells[6 + i].DeltaBetweenCards, cardCombinations[i].size() - 1);
                cardCombinations[i].push_back(clickCard);
                clickCard->Colum = i;

                flag = false;
                break;
            }
            //Если выделенную карту переместили на другую карту
            else if(columForMove == -1 && cardCombinations[i].size() > 0 && cardCombinations[i].back() != clickCard &&
               IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, clickCard->size) &&
               cardCombinations[i].back()->isRed == !clickCard->isRed && cardCombinations[i].back()->Value == clickCard->Value + 1)
            {               
//                if(cardCombinations[i].back()->PozitionY + cardCombinations[i].back()->size + cells[6 + i].DeltaBetweenCards > 578)
//                {
//                    cells[6 + i].DeltaBetweenCards = (578 - cardCombinations[i][numbersOpenCardInCardCombinations[i]]->PozitionY - cardCombinations[i].back()->size) / (cardCombinations[i].size() - numbersOpenCardInCardCombinations[i]);

//                    for(size_t j = numbersOpenCardInCardCombinations[i]; j < cardCombinations[i].size(); ++j)
//                    {
//                        cardCombinations[i][j]->PozitionY = cardCombinations[i][j - 1]->PozitionY + cells[6 + i].DeltaBetweenCards;
//                        cardCombinations[i][j]->OldPozitionY = cardCombinations[i][j - 1]->OldPozitionY + cells[6 + i].DeltaBetweenCards;
//                    }
//                }
                SetDeltaBetweenCards(i);
                int oldColum = clickCard->Colum;
                AddCardInCombination(cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, cardCombinations[i].back()->PozitionZ, cells[6 + i].DeltaBetweenCards, cardCombinations[clickCard->Colum].size() - 2, i);
                cardCombinations[i].push_back(clickCard);

                SetDeltaBetweenCards(oldColum, true);

                flag = false;
                break;
            }
            //Если несколько карт переместили на другую карту
            else if(columForMove != -1 && cardCombinations[i].size() > 0 && clickCard->Colum != i &&
               IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, clickCard->size) &&
               cardCombinations[i].back()->isRed == !clickCard->isRed && cardCombinations[i].back()->Value == clickCard->Value + 1)
            {
                int oldColum = clickCard->Colum;
                int x = cardCombinations[i].back()->PozitionX;
                int y = cardCombinations[i].back()->PozitionY;
                int z = cardCombinations[i].back()->PozitionZ;

                cardCombinations[i].insert(cardCombinations[i].end(), cardCombinations[clickCard->Colum].begin() + indexForMove, cardCombinations[clickCard->Colum].end());
                AddCardInCombination(x, y, z, cells[6 + i].DeltaBetweenCards, indexForMove - 1, i);

//                if(cardCombinations[i].back()->PozitionY + cardCombinations[i].back()->size + cells[6 + i].DeltaBetweenCards > 578)
//                {
//                    cells[6 + i].DeltaBetweenCards = (578 - cardCombinations[i][numbersOpenCardInCardCombinations[i]]->PozitionY - cardCombinations[i].back()->size) / (cardCombinations[i].size() - numbersOpenCardInCardCombinations[i]);

//                    for(size_t j = numbersOpenCardInCardCombinations[i]; j < cardCombinations[i].size(); ++j)
//                    {
//                        cardCombinations[i][j]->PozitionY = cardCombinations[i][j - 1]->PozitionY + cells[6 + i].DeltaBetweenCards;
//                        cardCombinations[i][j]->OldPozitionY = cardCombinations[i][j - 1]->OldPozitionY + cells[6 + i].DeltaBetweenCards;
//                    }
//                }
                SetDeltaBetweenCards(i);
                SetDeltaBetweenCards(oldColum, true);
//                if(cells[6 + oldColum].DeltaBetweenCards < 20)
//                {
//                    cells[6 + oldColum].DeltaBetweenCards = (578 - cardCombinations[oldColum][numbersOpenCardInCardCombinations[oldColum]]->PozitionY - cardCombinations[oldColum].back()->size) / (cardCombinations[oldColum].size() - 1 - numbersOpenCardInCardCombinations[oldColum]);

//                    if(cells[6 + oldColum].DeltaBetweenCards > 20)
//                        cells[6 + oldColum].DeltaBetweenCards = 20;

//                    for(size_t j = numbersOpenCardInCardCombinations[oldColum]; j < cardCombinations[oldColum].size(); ++j)
//                    {
//                        cardCombinations[oldColum][j]->PozitionY = cardCombinations[oldColum][j - 1]->PozitionY + cells[6 + oldColum].DeltaBetweenCards;
//                        cardCombinations[oldColum][j]->OldPozitionY = cardCombinations[oldColum][j - 1]->OldPozitionY + cells[6 + oldColum].DeltaBetweenCards;
//                    }
//                }

                //clickCard->Colum = i;
                columForMove = -1;
                indexForMove = -1;
                flag = false;
                break;
            }
            //Если выделенную карту(король) переместили на пустую ячейку
            else if(columForMove == -1 && clickCard->Colum != i && (cardCombinations[i].size() == 0 && clickCard->Value == 12) &&
                    IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cells[i + 6].PozitionX, cells[i + 6].PozitionY - cells[6 + i].DeltaBetweenCards, clickCard->size))
            {
                if(clickCard->Colum == -1)
                {
                    clickCard->size -= 10;
                    clickCard->OldPozitionX = clickCard->PozitionX = cells[i + 6].PozitionX;
                    clickCard->OldPozitionY = clickCard->PozitionY = cells[i + 6].PozitionY;
                    clickCard->OldPozitionZ = clickCard->PozitionZ = -7;

                    auto it = cardsInColode.begin();
                    std::advance(it, cardsInColode.size() - sizeOfColode);
                    cardsInColode.erase(it);
                    --sizeOfColode;

                    //cardCombinations[7].back() = cardsInColode[cardsInColode.size() - sizeOfColode];
                    cardCombinations[7].pop_back();
                    //AddCardInColode(cardCombinations[i].back()->PozitionX, cardCombinations[i].back()->PozitionY, cardCombinations[i].back()->PozitionZ, cells[6 + i].DeltaBetweenCards, cardCombinations[i].size() - 1);
                    cardCombinations[i].push_back(clickCard);
                    clickCard->Colum = i;
                }
                else
                {
                    //int oldColum = clickCard->Colum;
                    AddCardInCombination(cells[i + 6].PozitionX, cells[i + 6].PozitionY - cells[6 +i].DeltaBetweenCards, -7, cells[6 + i].DeltaBetweenCards, cardCombinations[clickCard->Colum].size() - 2, i);
                    cardCombinations[i].push_back(clickCard);
                }
                numbersOpenCardInCardCombinations[i] = 0;
                //SetDeltaBetweenCards(clickCard->Colum, true);
                flag = false;
                break;
            }
            //Если несколько карт(король) переместили на пустую ячейку
            else if(columForMove != -1 && (cardCombinations[i].size() == 0 && clickCard->Value == 12) &&
                    IsCardOnCard(clickCard->PozitionX, clickCard->PozitionY, cells[i + 6].PozitionX, cells[i + 6].PozitionY - cells[6 + i].DeltaBetweenCards, clickCard->size))
            {
                int oldColum = clickCard->Colum;
//                int x = cardCombinations[i].back()->PozitionX;
//                int y = cardCombinations[i].back()->PozitionY;
//                int z = cardCombinations[i].back()->PozitionZ;

                cardCombinations[i].insert(cardCombinations[i].end(), cardCombinations[clickCard->Colum].begin() + indexForMove, cardCombinations[clickCard->Colum].end());
                AddCardInCombination(cells[i + 6].PozitionX, cells[i + 6].PozitionY - cells[6 +i].DeltaBetweenCards, -7, cells[6 + i].DeltaBetweenCards, indexForMove - 1, i);

                SetDeltaBetweenCards(oldColum, true);
                numbersOpenCardInCardCombinations[i] = 0;
                columForMove = -1;
                indexForMove = -1;
                flag = false;
                break;
            }
        }

        if(flag)
        {
            if(columForMove == -1)
            {
                clickCard->PozitionX += 5;
                clickCard->PozitionY += 5;
                clickCard->size -= 10;
                returnCardBack.push_back(clickCard);
            }
            else
            {
                for(size_t i = 0; i < cardCombinations[columForMove].size() - indexForMove ; ++i)
                {
                    cardCombinations[columForMove][indexForMove + i]->PozitionX += 5;
                    cardCombinations[columForMove][indexForMove + i]->PozitionY += 5;
                    cardCombinations[columForMove][indexForMove + i]->size -= 10;

                    returnCardBack.push_back(cardCombinations[columForMove][indexForMove + i]);
                }
                columForMove = -1;
            }
        }
    }
    clickCard = nullptr;
    update();
}


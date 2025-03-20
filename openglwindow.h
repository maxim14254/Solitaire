#include <QtOpenGL/QtOpenGL>
#include <QOpenGLFunctions>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class Card;
class Cell;

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
private:
    Q_OBJECT
public:
    explicit OpenGLWindow(QWidget *parent = nullptr);
    ~OpenGLWindow();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void newGame();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void SetClickCard(Card* card, int x, int y);
    void AddCardInCombination(int x, int y, int z, int deltaY, int index, int numbColum);
    void SetDeltaBetweenCards(int i);
    void SetDeltaBetweenCards(int i, bool);
    void AddCardInVictoryConmbination(int i, Card* card);
    void AnalysisCardsForVictoryConmbination(size_t countI);

private:
    Card* cards;
    Cell* cells;
    std::vector<std::vector<Card*>> cardCombinations;
    std::vector<std::list<Card*>> cardCombinationsForVictory;
    std::vector<Card*> cardsInColode;
    std::vector<int> numbersOpenCardInCardCombinations;
    std::vector<Card*> returnCardBack;
    std::allocator<Cell> AllocCells;
    std::allocator<Card> AllocCard;
    size_t vec[52];
    QMediaPlayer* player;
    QMediaPlaylist* playerList;
    Card* clickCard = nullptr;
    Card* doubleClickCard = nullptr;
    Card* newOpenCard = nullptr;
    double animTime = 0.0;
    int deltaX = 0;
    int deltaY = 0;
    int speedAnim = 0;
    int columForMove = -1;
    size_t indexForMove = -1;
    size_t sizeOfColode = 0;
    bool gameNew = true;
    bool Anim = false;
    bool flag = false;
    bool pressRightButton = false;
};

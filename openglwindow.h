#include <QtOpenGL/QtOpenGL>
#include <QOpenGLFunctions>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

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
};

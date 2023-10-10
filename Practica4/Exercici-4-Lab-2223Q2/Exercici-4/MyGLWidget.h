// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformMorty();
    virtual void modelTransformDiscoBall();
    virtual void modelTransformFantasma(float posX);
    virtual void paintGL();
    virtual void initializeGL();
    virtual void carregaShaders();

    void llums();
    void trattimer();

    GLuint posFocusILoc, posFocus0Loc, posFocus1Loc, posFocus2Loc, posFocus3Loc;
    GLuint colorFocusILoc, colorFocus0Loc, colorFocus1Loc, colorFocus2Loc, colorFocus3Loc;
    GLuint transBolaLoc;

    float alfaBall = 0;
    float alfaPers = 0;

    bool Il = true;
    bool luces[4] = {true, true, true, true};
    int i = 0;

    QTimer *temp = new QTimer(this);

    glm::vec3 colorFocusI;
    glm::vec3 colorFocus0;
    glm::vec3 colorFocus1;
    glm::vec3 colorFocus2;
    glm::vec3 colorFocus3;

  private:
    int printOglError(const char file[], int line, const char func[]);
};

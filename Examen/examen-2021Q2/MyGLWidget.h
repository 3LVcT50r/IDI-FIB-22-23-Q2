#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void modelTransformPatricio ();
    virtual void enviaPosFocus ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    
    void enviaNewPosFocus();
    
    bool fpress = false;
    float angleP = 0;
    int num = 0;
    int nu_ant = 0;
  
  public slots:
    void presp();
    void ortho();
    void movePatricio(const int &nu);

  signals:
    void changePatricio(const int &);
    void resetSlider(const int &);
    void resetOrtho();
    void resetPersp();

  private:
    int printOglError(const char file[], int line, const char func[]);
};

#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void perspe();
    void ortho();

    void cub1();
    void cub2();
    void cub3();

  protected:
    //virtual void initializeGL ( );
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    GLuint colorFocusLoc;
    glm::vec3 colorFocus;

    float angleP = 0;
    float angleC = 0;
    bool vpress = false;

    bool fpress = false;

  private:
    int printOglError(const char file[], int line, const char func[]);
};

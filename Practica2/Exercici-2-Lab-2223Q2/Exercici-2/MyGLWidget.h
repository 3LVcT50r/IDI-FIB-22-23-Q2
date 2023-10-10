#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:

    virtual void iniEscena();
    virtual void iniCamera();
    virtual void display();
    virtual void paintGL();
    virtual void carregaShaders();
    virtual void viewTransform();
    virtual void viewTransform2();
    virtual void projectTransform2();
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void RickTransform();
    virtual void MortyTransform();
    virtual void VideoCameraTransform();

    GLuint cameraLoc;
    GLuint camuflajeLoc;

    float xmax, xmin;
    float ymax, ymin;
    float zmax, zmin;

    bool camara = false;
    bool camuflajel = false;

    float fov2, znear2, zfar2;

    float dist;
    float theta, psi;
    float vcangle = 45;

    glm::vec3 obs2;
    glm::vec3 vrp2;
    glm::vec3 up2;

    glm::vec3 ctierra = glm::vec3(0.8, 0.7, 1.0)*glm::vec3(0,1,0);
    glm:: vec3 cnada = glm::vec3(1,1,1);

  private:
    int printOglError(const char file[], int line, const char func[]);
};

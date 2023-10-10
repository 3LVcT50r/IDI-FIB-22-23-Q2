// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersAvio();
  creaBuffersHangar();
  creaBuffersTerra();

  centreEsc = glm::vec3 (15, 1.25, 12);
  radiEsc = sqrt(15*15+12*12+1.25*1.25);
}

void MyGLWidget::iniMaterialTerra()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.2,0.1,0.2);
  diff = glm::vec3(0.2,0.6,0.6);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 500;
}

void MyGLWidget::enviaPosFocus()
{
  if (not fpress) ExamGLWidget::enviaPosFocus();
  else {
    posFoc = glm::vec3((capsaAvioMin.x + capsaAvioMax.x)/2,(capsaAvioMin.y + capsaAvioMax.y)/2,capsaAvioMin.z);
    glm::vec4 posFoci = View * TGAvio * glm::vec4(posFoc, 1.0);
    posFoc = glm::vec3(posFoci.x, posFoci.y, posFoci.z);
  }
  glUniform3fv (posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera1ap() {
  camera2 = true;
  ra = float(width())/height();
  fov = glm::radians(90.0f);
  zn = 0.5;
  zf = radiEsc*3;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.5;
  angleX = -0.5;
  camera2 = false;
  ra = float(width())/height();
  fov = float(M_PI/3.0);
  zn = radiEsc;
  zf = radiEsc*3;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  ExamGLWidget::paintGL();
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3(15, 0, 12));
  TG = glm::rotate(TG, -float(M_PI)/2, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escalaPat, 2*escalaPat, 2*escalaPat));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformAvio ()
{
  TGAvio = glm::mat4(1.f);
  TGAvio = glm::translate(TGAvio, glm::vec3 (15, altura, 12));
  TGAvio = glm::rotate(TGAvio, angleA, glm::vec3(0,1,0));
  TGAvio = glm::translate(TGAvio, glm::vec3(10, 0, 0));
  TGAvio = glm::scale(TGAvio, glm::vec3(1.5,1.5,1.5));
  TGAvio = glm::scale(TGAvio, glm::vec3 (escalaAvio, escalaAvio, escalaAvio));
  TGAvio = glm::translate(TGAvio, -centreBaseAvio);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGAvio[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camera2)
  {  
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    View = glm::lookAt(glm::vec3(15, 3, 12), glm::vec3(14, 3, 12), glm::vec3(0,1,0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camera2)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;
    Proj = glm::perspective(fov, ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
    if (altura <= 5) ++altura;
    emit changeAlt(altura);
    modelTransformAvio();
    enviaPosFocus();
    break;
	}
  case Qt::Key_Down: {
    if (altura > 0) --altura;
    emit changeAlt(altura);
    modelTransformAvio();
    //enviaPosFocus();
    break;
	}
  case Qt::Key_Right: {
    angleA -= (2*float(M_PI))/32;
    modelTransformAvio();
    //enviaPosFocus();
    break;
	}
  case Qt::Key_F: {
    fpress = not fpress;
    enviaPosFocus();
    break;
	}
  case Qt::Key_C: {
    camera2 = not camera2;
    if (not camera2) {iniCamera(); emit s_c1();}
    else {iniCamera1ap(); emit s_c3();}
    break;
	}
  case Qt::Key_R: {
    altura = 0;
    angleA = 0;
    camera2 = false;
    iniCamera();
    fpress = false;
    enviaPosFocus();
    emit changeAlt(0);
    emit s_c1();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camera2)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (yClick - e->y()) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::cam1() {
  makeCurrent();
  iniCamera();
  update();
}

void MyGLWidget::cam3() {
  makeCurrent();
  iniCamera1ap();
  update();
}

void MyGLWidget::altu(const int &nu) {
  altura = nu;
  update();
}

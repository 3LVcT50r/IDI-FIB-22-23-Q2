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

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (not vpress) {
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0, 0.0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO_Cub);
    modelTransformCub (5.0, (2*float(M_PI))/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO_Cub);
    modelTransformCub (6.0, 2*(2*float(M_PI))/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  } 
  else {
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }

  glUniform3fv(colorFocusLoc, 1, &colorFocus[0]);
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle + angleC, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angleP + angleC, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5.0,0,0));
  TG = glm::scale(TG, glm::vec3(2*escala, 2*escala, 2*escala));
  TG = glm::rotate(TG, -float(M_PI)/2, glm::vec3(0,1,0));
  //TG = glm::rotate(TG, -float(M_PI)/2, glm::vec3(1,0,0));
  TG = glm::translate(TG, -centreBasePat);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI)/2, glm::vec3(1, 0, 0));
    View = glm::rotate(View, float(M_PI)/2, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::perspe() {
  makeCurrent();
  camPlanta = false;
  viewTransform();
  update();
}

void MyGLWidget::ortho() {
  makeCurrent();
  camPlanta = true;
  viewTransform();
  update();
}

void MyGLWidget::cub1() {
  angleP = 0;
  update();
}

void MyGLWidget::cub2() {
  angleP = (2*float(M_PI))/3;
  update();
}

void MyGLWidget::cub3() {
  angleP = 2*(2*float(M_PI))/3;
  update();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
    vpress = not vpress;
    break;
	}
  case Qt::Key_1: {
    angleP = 0;
    break;
	}
  case Qt::Key_2: {
    angleP = (2*float(M_PI))/3;
    break;
	}
  case Qt::Key_3: {
    angleP = 2*(2*float(M_PI))/3;
    break;
	}
  case Qt::Key_F: {
    fpress = not fpress;
    if (fpress) colFoc = glm::vec3(1,1,0);
    else colFoc = glm::vec3(1,1,1);
    enviaColFocus();
    break;
	}
  case Qt::Key_C: {
    camPlanta = not camPlanta;
    viewTransform();
    projectTransform();
    break;
	}
  case Qt::Key_Right: {
    angleC += (2*float(M_PI))/3;
    break;
	}
  case Qt::Key_Left: {
    angleC -= (2*float(M_PI))/3;
    break;
	}
  case Qt::Key_R: {
    iniCamera();
    angleC = 0;
    vpress = false;
    fpress = false;
    camPlanta = false;
    angleP = 0;
    colFoc = glm::vec3(1,1,1);
    enviaColFocus();
    viewTransform();
    projectTransform();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


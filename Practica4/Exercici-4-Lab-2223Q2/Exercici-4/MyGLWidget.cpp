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

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY -= (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      alfaPers -= float(M_PI)/4.0f;
    break;
	}
  case Qt::Key_D: {
      alfaPers += float(M_PI)/4.0f;
    break;
	}
  case Qt::Key_E: {
      Il = not Il;
    break;
	}
  case Qt::Key_B: {
      i %= 4;
      luces[i] = not luces[i];
      ++i;
    break;
	}	
  case Qt::Key_R: {
      alfaBall += float(M_PI)/36.0f;
    break;
	}
  case Qt::Key_S: {
      if (temp->isActive()) temp->stop();
      else temp->start(100);
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::llums() {

  if (Il) colorFocusI = glm::vec3(0.4,0.4,0.4);
  else colorFocusI = glm::vec3(0,0,0);

  if (luces[0]) colorFocus0 = glm::vec3(0.4,0.0,0.0);
  else colorFocus0 = glm::vec3(0.0,0.0,0.0);

  if (luces[1]) colorFocus1 = glm::vec3(0.0,0.4,0.0);
  else colorFocus1 = glm::vec3(0.0,0.0,0.0);

  if (luces [2]) colorFocus2 = glm::vec3(0.0,0.0,0.4);
  else colorFocus2 = glm::vec3(0.0,0.0,0.0);

  if (luces[3]) colorFocus3 = glm::vec3(0.4,0.4,0.0);
  else colorFocus3 = glm::vec3(0.0,0.0,0.0);

  glUniform3fv(colorFocusILoc, 1, &colorFocusI[0]);
  glUniform3fv(colorFocus0Loc, 1, &colorFocus0[0]);
  glUniform3fv(colorFocus1Loc, 1, &colorFocus1[0]);
  glUniform3fv(colorFocus2Loc, 1, &colorFocus2[0]);
  glUniform3fv(colorFocus3Loc, 1, &colorFocus3[0]);
}

void MyGLWidget::modelTransformMorty() {
  LL4GLWidget::modelTransformMorty();
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::rotate(TG, alfaPers, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX)
{
  LL4GLWidget::modelTransformFantasma(posX);
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::rotate(TG, alfaPers, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformDiscoBall() {
  LL4GLWidget::modelTransformDiscoBall();
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::rotate(TG, alfaBall, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  glUniformMatrix4fv (transBolaLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL() {
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);

  llums();
  
  glBindVertexArray(0);
}

void MyGLWidget::trattimer() {
  alfaPers -= float(M_PI)/4.0f;
  i %= 4;
  luces[i] = not luces[i];
  ++i;
  
  update();
}

void MyGLWidget::initializeGL() {
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersDiscoBall();
  creaBuffersTerraIParets();

  glm::vec3 posFocusI = glm::vec3(5,10,5);
  glm::vec3 posFocus0 = glm::vec3(1.2,0,0);
  glm::vec3 posFocus1 = glm::vec3(-1.2,0,0);
  glm::vec3 posFocus2 = glm::vec3(0,0,1.2);
  glm::vec3 posFocus3 = glm::vec3(0,0,-1.2);

  glUniform3fv(posFocusILoc, 1, &posFocusI[0]);
  glUniform3fv(posFocus0Loc, 1, &posFocus0[0]);
  glUniform3fv(posFocus1Loc, 1, &posFocus1[0]);
  glUniform3fv(posFocus2Loc, 1, &posFocus2[0]);
  glUniform3fv(posFocus3Loc, 1, &posFocus3[0]);

  iniEscena();
  iniCamera();

  connect(temp, &QTimer::timeout, this, &MyGLWidget::trattimer);
}

void MyGLWidget::carregaShaders() {
  LL4GLWidget::carregaShaders();
  
  posFocusILoc = glGetUniformLocation (program->programId(), "posFocusI");
  posFocus0Loc = glGetUniformLocation (program->programId(), "posFocus0");
  posFocus1Loc = glGetUniformLocation (program->programId(), "posFocus1");
  posFocus2Loc = glGetUniformLocation (program->programId(), "posFocus2");
  posFocus3Loc = glGetUniformLocation (program->programId(), "posFocus3");

  colorFocusILoc = glGetUniformLocation (program->programId(), "colorFocusI");
  colorFocus0Loc = glGetUniformLocation (program->programId(), "colorFocus0");
  colorFocus1Loc = glGetUniformLocation (program->programId(), "colorFocus1");
  colorFocus2Loc = glGetUniformLocation (program->programId(), "colorFocus2");
  colorFocus3Loc = glGetUniformLocation (program->programId(), "colorFocus3");
  
  transBolaLoc = glGetUniformLocation (program->programId(), "TGB");

}
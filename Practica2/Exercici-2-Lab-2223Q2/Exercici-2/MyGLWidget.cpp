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

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::display() {

    //RICK
    if (camuflajel) glUniform3fv(camuflajeLoc, 1, (const GLfloat*) &ctierra);
    else glUniform3fv(camuflajeLoc, 1, (const GLfloat*) &cnada);

    glBindVertexArray(VAO_models[RICK]);
    RickTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);
    glUniform3fv(camuflajeLoc, 1, (const GLfloat*) &cnada);

    //MORTY
    glBindVertexArray(VAO_models[MORTY]);
    MortyTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);

    //VIDEOCAMERA
    glBindVertexArray(VAO_models[VIDEOCAMERA]);
    VideoCameraTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);

    //TIERRA
    glBindVertexArray(VAO_Terra);
    identTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //PARET
    ParetTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MyGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    LL2GLWidget::projectTransform();
    viewTransform();
    display();

    if (camara) {
        glViewport(ample-(ample/4), alt-(alt/4), ample/4, alt/4);
        ra = float(ample/4)/float(alt/4);
        //CAMBIO
        viewTransform2();
        projectTransform2();
        display();
    }

    glBindVertexArray(0);
}

void MyGLWidget::viewTransform() {

    glm::mat4 View(1.0);
    View = glm::translate(View, glm::vec3(0,0,-dist));
    View = glm::rotate(View, glm::radians(theta+factorAngleY), glm::vec3(1,0,0));
    View = glm::rotate(View, glm::radians(-psi+factorAngleX), glm::vec3(0,1,0));
    View = glm::translate(View, -vrp);
    glUniform1i(cameraLoc, 0);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::viewTransform2() {

    glm::mat4 View(1.0);
    vrp2 = obs2 + glm::vec3(sin(glm::radians(vcangle)), -1, cos(glm::radians(vcangle)));
    View = glm::lookAt(obs2, vrp2, up2);
    glUniform1i(cameraLoc, 1);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform2() {
    glm::mat4 Proj(1.0);
    Proj = glm::perspective(fov2, ra, znear2, zfar2);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {

    makeCurrent();
    
    factorAngleX += (e->x() - xClick)*M_PI/10;
    factorAngleY += (e->y() - yClick)*M_PI/10;

    viewTransform();

    xClick = e->x();
    yClick = e->y();

    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {

    makeCurrent();
    switch (e->key()) {
        case Qt::Key_Up: {
            posMorty += glm::vec3(sin(glm::radians(angleMorty)), 0, cos(glm::radians(angleMorty)))*glm::vec3(0.1);
            break;
        }
        case Qt::Key_Down: {
            posMorty -= glm::vec3(sin(glm::radians(angleMorty)), 0, cos(glm::radians(angleMorty)))*glm::vec3(0.1);
            break;
        }
        case Qt::Key_Right: {
            angleMorty -= 45;
            if (angleMorty == -45) angleMorty = 315;
            break;
        }
        case Qt::Key_Left: {
            angleMorty += 45;
            if (angleMorty == 360) angleMorty = 0;
            break;
        }
        case Qt::Key_Q: {
            if (vcangle < 90) vcangle += 1;
            break;
        }
        case Qt::Key_E: {
            if (vcangle > 0) vcangle -= 1;
            break;
        }
        case Qt::Key_C: {
            camara = not camara;
            break;
        }
        case Qt::Key_G: {
            camuflajel = true;
            break;
        }
        case Qt::Key_R: {
            camuflajel = false;
            camara = false;
            posMorty = glm::vec3(1,0,0);
            angleMorty = 0;
            vcangle = 45;
            factorAngleX = M_PI/alt;
            factorAngleY = M_PI/ample;
            break;
        }
        default: e->ignore();
        break;
    }
    update();
}

void MyGLWidget::RickTransform() {

    glm::mat4 TG(1.0);
    TG = glm::translate(TG, glm::vec3(-2, 0, -2));
    TG = glm::rotate(TG, glm::radians(float(180.0)), glm::vec3(0,1,0));
    TG = glm::scale(TG, glm:: vec3(escalaModels[RICK]));
    TG = glm::translate(TG, -centreBaseModels[RICK]);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform() {

    glm::mat4 TG(1.0);
    TG = glm::translate(TG, posMorty);
    TG = glm::rotate(TG, glm::radians(angleMorty), glm::vec3(0,1,0));
    TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
    TG = glm::translate(TG, -centreBaseModels[MORTY]);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::VideoCameraTransform() {

    glm::mat4 TG(1.0);
    TG = glm::translate(TG, glm::vec3(-4,3,-4));
    TG = glm::rotate(TG, glm::radians(vcangle), glm::vec3(0,1,0));
    TG = glm::rotate(TG, glm::radians(float(45.0)), glm::vec3(1,0,0));
    TG = glm::scale(TG, glm::vec3(escalaModels[VIDEOCAMERA]));
    TG = glm::translate(TG, -centreBaseModels[VIDEOCAMERA]);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniEscena() {

    xmax = 4; xmin = -4;
    ymax = 3; ymin = 0;
    zmax = 3; zmin = -4;

    centreEscena = glm::vec3((xmax+xmin)/2, (ymax+ymin)/2, (zmax+zmin)/2);
    radiEscena = distance(glm::vec3(zmin, ymin, zmin), glm::vec3(xmax, ymax, zmax))/2;
    dist = 2*radiEscena;
}

void MyGLWidget::iniCamera() {

    vrp = vrp2 = centreEscena;

    float alpha = asinf(radiEscena/dist);
    fov = 2*alpha;

    znear = dist-radiEscena;
    zfar = dist+radiEscena;

    theta = 45;
    psi = 0;

    obs2 = glm::vec3(-4,3,-3);
    up2 = glm::vec3(0,1,0);
    fov2 = glm::radians(float(60));
    znear2 = 0.25;
    zfar2 = dist;
}

void MyGLWidget::carregaShaders() {

    LL2GLWidget::carregaShaders();
    cameraLoc = glGetUniformLocation (program->programId(), "camera");
    camuflajeLoc = glGetUniformLocation (program->programId(), "camuflaje");
}
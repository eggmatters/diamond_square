/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>
#include <QWheelEvent>

#ifdef WIN32
    #include <GL/glext.h>
    PFNGLACTIVETEXTUREPROC pGlActiveTexture = NULL;
    #define glActiveTexture pGlActiveTexture
#endif //WIN32

GlWidget::GlWidget(QWidget *parent)
    : QGLWidget(QGLFormat(/* Additional format options */), parent)
{
    alpha = -0.0;
    beta = -0;
    distance = -0.0;       
    fly = false;
    gridmode = false;
    world = new World(64, 1);
}


GlWidget::~GlWidget()
{
}

QSize GlWidget::sizeHint() const
{
    return QSize(640, 480);
}


void GlWidget::initializeGL()
{
    #ifdef WIN32
        glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress((LPCSTR) "glActiveTexture");
    #endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    const float bgColor[4] = {.7, .8, 1.0, 1.0};
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);

    camera = world->initCamera();

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/vertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/fragmentShader.fsh");

    shaderProgram.link();
//    qDebug("%s", qPrintable(log));
    texture = bindTexture(QPixmap(":/textures/grassb512.bmp"));
}

void GlWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}

void GlWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    if (!fly) {
        float yPosition = world->getYPosition(camera);
        camera.setY(-yPosition + 1.0f);
    }

    vMatrix.rotate(180.0f, 0.0f, 0.0f, 1.0f);
    vMatrix.rotate(yRot, 0.0f, 1.0f, 0.0f);
    vMatrix.translate(camera);

    shaderProgram.bind();

    shaderProgram.setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);
    shaderProgram.setUniformValue("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(0);

    //shaderProgram.setUniformValue("color", QColor(Qt::black));

    shaderProgram.setAttributeArray("vertex", world->vertices.constData());
    shaderProgram.enableAttributeArray("vertex");

    shaderProgram.setAttributeArray("textureCoordinate", world->textures.constData());
    shaderProgram.enableAttributeArray("textureCoordinate");

    QString log = shaderProgram.log();
    if (!log.isEmpty()) {
        qDebug("%s", qPrintable(log));
    }
    //    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, world->vertices.size());

    shaderProgram.disableAttributeArray("vertex");
    shaderProgram.disableAttributeArray("textureCoordinate");
    shaderProgram.release();

}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
    updateGL();
    event->accept();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -90) {
            beta = -90;
        }
        if (beta > 90) {
            beta = 90;
        }

        updateGL();
    }

    lastMousePosition = event->pos();

    event->accept();
}

void GlWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1;
        } else if (delta > 0) {
            distance *= 0.9;
        }

        updateGL();
    }

    event->accept();
}

void GlWidget::keyPressEvent(QKeyEvent* event) {

    switch(event->key()) {
    case Qt::Key_A:
        camera.setX( camera.x() - (float)cos (yRot * DEGREES_TO_RADIANS) * 0.1f);
        camera.setZ( camera.z() - (float)sin (yRot * DEGREES_TO_RADIANS) * 0.1f);
        break;
    case Qt::Key_D:
        camera.setX( camera.x() + (float)cos (yRot * DEGREES_TO_RADIANS) * 0.1f);
        camera.setZ( camera.z() + (float)sin (yRot * DEGREES_TO_RADIANS) * 0.1f);
        break;
    case Qt::Key_W:
        camera.setX( camera.x() - (float)sin (yRot * DEGREES_TO_RADIANS) * 0.1f);
        camera.setZ( camera.z() + (float)cos (yRot * DEGREES_TO_RADIANS) * 0.1f);
        break;
    case Qt::Key_S:
        camera.setX( camera.x() + (float)sin (yRot * DEGREES_TO_RADIANS) * 0.1f);
        camera.setZ( camera.z() - (float)cos (yRot * DEGREES_TO_RADIANS) * 0.1f);
        break;
    case Qt::Key_Up:
        camera.setY( camera.y() + 0.1f);
        break;
    case Qt::Key_Down:
        camera.setY( camera.y() - 0.1f);
        break;
    case Qt::Key_Right:
        if (yRot >= 360)
            yRot = 0;
        yRot -= 2.3f;
        break;
    case Qt::Key_Left:
        if (yRot < -360)
            yRot = 0;
        yRot += 2.3f;
        break;
    case Qt::Key_Escape:
        if (fly) {
            fly = false;
        } else {
            fly = true;
        }
        break;
    case Qt::Key_F1:
        if (gridmode) { gridmode = false; }
        else { gridmode = true; }
    }
    updateGL();
    event->accept();
}





#ifndef GLWIDGET_H
#define GLWIDGET_H

#define EYE_LEVEL               -0.3f
#define DEGREES_TO_RADIANS      .017453292F
#define RADIANS_TO_DEGREES      57.29577951F

#include <QGLWidget>
#include <QGLShaderProgram>
#include "world.h"

class GlWidget : public QGLWidget
{
    Q_OBJECT

public:
    GlWidget(QWidget *parent = 0);
    ~GlWidget();
    QSize sizeHint() const;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* event);

private:
    World * world;
    QMatrix4x4 pMatrix;
    QGLShaderProgram shaderProgram;

    QVector3D camera;

    GLuint texture;

    double alpha;
    double beta;
    double distance;
    double xPos;
    double yPos;
    double zPos;
    double yRot;
    QPoint lastMousePosition;
    bool fly;
    bool gridmode;
};

#endif // GLWIDGET_H

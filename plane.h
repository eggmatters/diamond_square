#ifndef PLANE_H
#define PLANE_H

#ifndef QVECTOR_H
#include <QVector>
#endif

#ifndef QVECTOR3D_H
#include <QVector3D>
#endif

#ifndef MATH_H
#include <math.h>
#endif

class Plane
{
public:
    Plane();      
    void setPlane(QVector3D vertexP, QVector3D vertexQ, QVector3D vertexR);
    float getYPosition(QVector3D camera);
private:
    QVector<QVector3D> plane;
    QVector3D normal;
    void debugPlane(float yValue, QVector3D camera);
};

#endif // PLANE_H

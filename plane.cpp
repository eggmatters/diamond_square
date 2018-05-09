#include "plane.h"

Plane::Plane()
{

}

void Plane::setPlane(const QVector3D vertexP, const QVector3D vertexQ, const QVector3D vertexR) {
    //set absolute values of plane objects so we have a y relative to our drawing grid:
    //We will need to make an adjustment if we have one or two vertices which cross the y axis though.
    QVector3D p = QVector3D(fabs(vertexP.x()), fabs(vertexP.y()), fabs(vertexP.z()));
    QVector3D q = QVector3D(fabs(vertexQ.x()), fabs(vertexQ.y()), fabs(vertexQ.z()));
    QVector3D r = QVector3D(fabs(vertexR.x()), fabs(vertexR.y()), fabs(vertexR.z()));
    plane << p << q << r;
    //set the normal by cross product of vectors PQ X PR.
    QVector3D pq = q - p;
    QVector3D pr = r - p;
    normal = QVector3D::crossProduct(pq,pr);
}

float Plane::getYPosition(QVector3D camera) {
    //Absolute the camera:
    QVector3D absCam = QVector3D(fabs(camera.x()), fabs(camera.y()), fabs(camera.z()));
    //Find constant d and sovle for plane eq.
    QVector3D vectorP = QVector3D(plane.at(0));
    float d =  -((normal.x() * vectorP.x()) + (normal.y()*vectorP.y()) + (normal.z() * vectorP.z()));
    if (normal.y() == 0.00000f) {
        qDebug("It's normal y = %f", normal.y());
        return camera.y();
    }
    float yValue = ( -(normal.x()*absCam.x()) - (normal.z()*absCam.z()) - d ) / normal.y();
    //debugPlane(yValue, camera);
    return yValue;
}

void Plane::debugPlane(float yValue, QVector3D camera) {
    QVector3D pq = plane[1] - plane[0];
    QVector3D pr = plane[2] - plane[0];
    QVector3D absCam = QVector3D(fabs(camera.x()), fabs(camera.y()), fabs(camera.z()));
    qDebug("\n***************");
    qDebug("Camera:\nCx = %f Cy = %f Cz = %f", absCam.x(), absCam.y(), absCam.z());
    qDebug("Plane P:\nPx = %f Py = %f Pz = %f", plane[0].x(), plane[0].y(), plane[0].z());
    qDebug("Plane Q:\nQx = %f Qy = %f Qz = %f", plane[1].x(), plane[1].y(), plane[1].z());
    qDebug("Plane R:\nRx = %f Ry = %f Rz = %f", plane[2].x(), plane[2].y(), plane[2].z());
    qDebug("P to Q:\nPQx = %f PQy = %f PQz = %f", pq.x(), pq.y(), pq.z());
    qDebug("P to R:\nPRx = %f PRy = %f PRz = %f", pr.x(), pr.y(), pr.z());
    qDebug("\nNormal:\n Nx = %f Ny = %f Nz = %f", normal.x(), normal.y(), normal.z());
    qDebug("yValue = %f", yValue);

}

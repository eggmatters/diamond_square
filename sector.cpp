#include "sector.h"


Sector::Sector()
{
}

Sector::Sector(const int & sectorId,
               const int & size,
               const int & polyDepth,
               const QPoint origin,
               const QVector<QVector<float > > &terrain) :
                        sectorId(sectorId),
                        size(size),
                        polyDepth(polyDepth),
                        origin(origin),
                        Terrain(terrain)
{    
    setSector(origin);
    isRendered = false;
    toBeRendered = false;
}

/**
 * Sets QVector3D objects for each vertex as well as a plane object
 * which computes normals and points on the plane.
 * @brief Sector::setSector
 * @param origin
 */
void Sector::setSector(QPoint origin) {
    qDebug("Origin is: %d, %d", origin.x(), origin.y());
    for (int z = origin.y(); z < (size + origin.y()); z++) {
        for (int x = origin.x(); x < (size + origin.x()); x++) {
            Plane *plane1   = new Plane();
            Plane *plane2   = new Plane();
            qreal yValueP_1 = Terrain[x][z + 1];
            qreal yValueQ_1 = Terrain[x][z];
            qreal yValueR_1 = Terrain[x + 1][z + 1];
            qreal yValueP_2 = Terrain[x + 1][z];
            qreal yValueQ_2 = Terrain[x + 1][z + 1];
            qreal yValueR_2 = Terrain[x][z];
            vertices << QVector3D(x, yValueP_1, z + 1.0f)
                     << QVector3D(x, yValueQ_1, z)
                     << QVector3D(x + 1.0f, yValueR_1, z + 1.0f);
            plane1->setPlane(QVector3D(x, yValueP_1, z + 1.0f),
                             QVector3D(x, yValueQ_1, z),
                             QVector3D(x + 1.0f, yValueR_1, z+ 1.0f));
            vertices << QVector3D(x + 1.0f, yValueP_2, z)
                     << QVector3D(x + 1.0f, yValueQ_2, z + 1.0f)
                     << QVector3D(x, yValueR_2, z);
            plane2->setPlane(QVector3D(x + 1.0f, yValueP_2, z),
                             QVector3D(x + 1.0f, yValueQ_2, z + 1.0f),
                             QVector3D(x, yValueR_2, z));
            planes.append(plane1);
            planes.append(plane2);
            textures << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1)
                     << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0);
        }
    }

}

void Sector::setSectorId(int id) {
    this->sectorId = id;
}

int Sector::getSectorId() {
    return this->sectorId;
}

int Sector::terrainSize() {
    return size;
}

QPoint Sector::getOrigin() {
    return this->origin;
}

void Sector::setSectorRendered() {
    if (!isRendered) {
        isRendered = true;
    }
}

void Sector::setSectorToRender() {
    if (!toBeRendered) {
        toBeRendered = true;
    }
}

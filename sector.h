#ifndef SECTOR_H
#define SECTOR_H

#ifndef QVECTOR_H
#include <QVector>
#endif

#ifndef QVECTOR3D_H
#include <QVector3D>
#endif

#ifndef QVECTOR2D_H
#include <QVector2D>
#endif

#ifndef QPOINT_H
#include <QPoint>
#endif

#include "terraingen.h"
#include "plane.h"

class Sector
{
public:
    Sector();
    Sector(const int &sectorId,
           const int &size,
           const int &polyDepth,
           const QPoint origin,
           const QVector<QVector<float> > &terrain);
    QVector<QVector3D> vertices;
    QVector<QVector2D> textures;
    QVector<Plane*> planes;
    bool isRendered;
    bool toBeRendered;

    void setSectorId(int id);
    void setSectorRendered();
    void setSectorToRender();
    int getSectorId();
    int terrainSize();
    QPoint getOrigin();

private:
    int sectorId;
    int size;
    int polyDepth;
    QVector<QVector<float> > Terrain;
    QPoint origin;
    void setSector(QPoint origin);
};

#endif // SECTOR_H
